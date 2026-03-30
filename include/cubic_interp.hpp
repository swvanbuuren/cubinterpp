#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <tuple>
#include <vector>
#include <mdspan/mdspan.hpp>
#include "utils.hpp"
#include "vectorn.hpp"
#include "derivatives.hpp"


namespace cip {


template<typename T, std::size_t N>
class CubicCellND
{
    static constexpr std::size_t order = 4;
    static constexpr std::size_t numCorners = 1 << N;
    static constexpr std::size_t numCoeffs = 1 << (2 * N);
    using CoeffsArray = std::array<T, numCoeffs>;
    using Array = std::array<T, order>;
    using Array2 = std::array<Array, order>;
    using ArrayN = std::array<T, N>;
    using Delta = std::array<Array2, N>;
    using Span = std::span<const T>;
    using Spans = std::array<Span, N>;
    using Mdspan = std::mdspan<const T, std::dextents<std::size_t, 2*N>, std::layout_stride>;
public:
    
    explicit CubicCellND(const Mdspan &F, const Spans &x)
      : coeffs(calc_coeffs(x, F))
    {
    }
    ~CubicCellND() = default;

    template <typename... Args>
    requires (sizeof...(Args) == N)
    T eval(Args&&... xi) const
    {
        return eval_poly<0>(0, {std::forward<Args>(xi)...});
    }

private:
    const CoeffsArray coeffs;

    const ArrayN calc_h(const Spans &x) const
    {
        return std::apply(
            [](const auto&... xi) { return ArrayN{(xi[1] - xi[0])...}; },
            x
        );
    }

    const T calc_h3(const ArrayN &h) const
    {
        T prod_hi = std::apply(
            [](const auto&... hi) { return (hi * ...); },
            h
        );
        return prod_hi*prod_hi*prod_hi;
    }

    const Array2 calc_delta_ij(const Span &xi) const 
    {
        const T x0 = xi[0];
        const T x1 = xi[1];
        const T x02 = x0*x0;
        const T x12 = x1*x1;
        return {{{x12*(x1 - 3.0*x0), +6.0*x0*x1,           -3.0*(x0 + x1), +2.0},
                 {-x0*x12,           x1*(2.0*x0 + x1),     -(x0 + 2.0*x1), +1.0},
                 {x02*(3.0*x1 - x0), -6.0*x0*x1,           +3.0*(x0 + x1), -2.0},
                 {-x1*x02,           x0*(x0 + 2.0*x1),     -(2.0*x0 + x1), +1.0}}};
    }

    const Delta calc_delta(const Spans &x) const 
    {
        return std::apply(
            [this](const auto&... xi) { return Delta{calc_delta_ij(xi)...}; },
            x
        );
    }

    const CoeffsArray calc_coeffs(const Spans &x, const Mdspan &F) const {
        const ArrayN h = calc_h(x);
        const T h3 = calc_h3(h);
        const Delta delta = calc_delta(x);
        CoeffsArray _coeffs = {};
        std::array<std::size_t, 2*N> indices = {};
        for (std::size_t m_idx = 0; m_idx < numCoeffs; ++m_idx) {
            for (std::size_t i = 0; i < numCorners; ++i) {
                for (std::size_t j = 0; j < numCorners; ++j) {
                    T product = T{1.0};
                    for (std::size_t k = 0; k < N; ++k) {
                        std::size_t i_k = (i >> k) & 1;
                        std::size_t j_k = (j >> k) & 1;
                        std::size_t ij_k = (i_k << 1) | j_k;
                        std::size_t m_k = (m_idx >> ((N-1-k)*2)) & 3;
                        indices[k]   = i_k;
                        indices[k+N] = j_k;
                        T h_factor = (j_k == 0) ? 1.0 : h[k];
                        product *= h_factor * delta[k][ij_k][m_k];
                    }
                    _coeffs[m_idx] += F(indices)*product;
                }
            }
            _coeffs[m_idx] /= h3;
        }
        return _coeffs;
    }

    template <std::size_t D>
    constexpr T eval_poly(std::size_t offset, const std::array<T, N>& x) const
    {
        if constexpr (D == N)
        {
            return coeffs[offset];
        } else {
            constexpr std::size_t stride = cip::power<order, N-D-1>();
            T c0 = eval_poly<D+1>(offset, x);
            T c1 = eval_poly<D+1>(offset + stride, x);
            T c2 = eval_poly<D+1>(offset + 2*stride, x);
            T c3 = eval_poly<D+1>(offset + 3*stride, x);
            return c0 + x[D]*(c1 + x[D]*(c2 + x[D]*c3));
        }
    }

};



template <typename T, std::size_t N, IndexMethod IM = IndexMethod::BinarySearch>
class CubicInterpND
{
    static constexpr std::size_t size_t_two = 2;
    using Vector = std::vector<T>;
    using Array = std::array<Vector, N>;
    using Cell = CubicCellND<T, N>;
    using Cells = cip::VectorN<Cell, N>;
    using Span = std::span<const T>;
    using Mdspan = std::mdspan<T, std::dextents<std::size_t, N>, std::layout_stride>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using Ff = cip::VectorN<T, N>;
    using Ff2 = cip::VectorN<T, 2*N>;
    using Pr = std::pair<std::size_t, std::size_t>;
    using Indexers = std::array<cip::Indexer<T, IM>, N>;
public:
    template <typename... Args>    
    CubicInterpND(const Ff &_f, const Args & ... _xi)
      : xi{_xi...},
        indexers{cip::Indexer<T, IM>(_xi)...},
        F(T{}, {_xi.size()..., ((void)_xi, size_t_two)...}),
        cells(std::array<std::size_t, N>{(_xi.size()-1)...})
    {
    }
    virtual ~CubicInterpND() { }

    virtual Vector calc_slopes(const Vector &x, const Mdspan1D &f) const = 0;

    template <typename... Args>
    void build(const Ff &f, const Args & ... _xi)
    {
        populate_F(f, _xi...);
        build_cell(cells);
    }

    template <typename... Args>
    T eval(const Args&... args) const
    {
        std::size_t dim = 0;
        std::array<size_t, N> indices = { indexers[dim++].index(args)... };
        return cells(indices).eval(args...);
    }

    template <typename... Vectors>
    Vector evaln(const Vectors & ... inputs) const
    {
        static_assert(sizeof...(inputs) > 0, "At least one input vector is required");
        const std::size_t size = std::get<0>(std::tuple<const Vectors&...>(inputs...)).size();
        Vector z(size);
        for (std::size_t i = 0; i < size; ++i) {
            z[i] = eval(inputs[i]...);
        }
        return z;
    }


private:
    const Array xi;
    const Vector x;
    const Vector y;
    const Indexers indexers;
    Cells cells;
    Ff2 F;

    template <typename... Args>
    void populate_F(Ff f, const Args & ... _xi) { // NOTE: pass f by value, which will be moved into F
        F.move_into_submdspan(std::move(f), ((void)_xi, std::full_extent)..., ((void)_xi, 0)...);
        auto slopesLambda = [this](const Vector &x, const Mdspan1D &f_slice) -> Vector {
            return this->calc_slopes(x, f_slice);
        };
        cip::compute_mixed_derivatives<N>(F, xi, slopesLambda);
    }

    template <typename... Indices>
    void build_cell(Cells &_cells, Indices... indices) const {
        if constexpr (sizeof...(Indices) == N) {
            std::size_t index = 0;
            std::array<Span, N> spans = { Span(&xi[index++][indices], 2)... };
            _cells.emplace_back(
                F.submdspan(Pr{indices, indices+1}..., ((void)indices, std::full_extent)...),
                spans
            );
        } else {
            for (std::size_t i = 0; i < xi[sizeof...(indices)].size() - 1; ++i) {
                build_cell(_cells, indices..., i);
            }
        }
    }

};


} // namespace cip
