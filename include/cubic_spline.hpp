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



template <typename T, std::size_t N>
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
    using VectorN = cip::VectorN<T, N>;
    using VectorN2 = cip::VectorN<T, 2*N>;
    using Pr = std::pair<std::size_t, std::size_t>;
    using Indexers = std::array<cip::Indexer<T>, N>;
public:
    template <typename... Args>    
    CubicInterpND(const VectorN &_f, const Args & ... _xi)
      : xi{_xi...},
        indexers{cip::Indexer<T>(_xi)...},
        F(T{}, {_xi.size()..., ((void)_xi, size_t_two)...}),
        cells({(_xi.size()-1)...})
    {
    }
    virtual ~CubicInterpND() { }

    virtual Vector calc_slopes(const Vector &x, const Mdspan1D &f) const = 0;

    template <typename... Args>
    void build(const VectorN &f, const Args & ... _xi)
    {
        populate_F(f, _xi...);
        build_cell(cells);
    }

    template <typename... Args>
    T eval(const Args&... args) const
    {
        std::size_t dim = 0;
        std::array<size_t, N> indices = { indexers[dim++].sort_index(args)... };
        return cells(indices).eval(args...);
    }


private:
    const Array xi;
    const Vector x;
    const Vector y;
    const Indexers indexers;
    Cells cells;
    VectorN2 F;

    template <typename... Args>
    void populate_F(VectorN f, const Args & ... _xi) { // NOTE: pass f by value, which will be moved into F
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


template<typename T>
class CubicCellND<T, 1>
{
    static constexpr std::size_t order = 4;
    using Array = std::array<T, order>;
    using Alpha = std::array<Array, order>;
    using Span = std::span<const T>;
    using Mdspan = std::mdspan<const T, std::dextents<std::size_t, 2>, std::layout_stride>;
public:
    explicit CubicCellND(const Span &x, const Mdspan &F)
      : coeffs(calc_coeffs(x, F))
    {
    }
    ~CubicCellND() = default;

    const T eval(const T x) const
    {
        return coeffs[0] + (coeffs[1] + (coeffs[2] + coeffs[3]*x)*x)*x;
    }

private:
    const Array coeffs;

    constexpr Array calc_coeffs(const Span &x, const Mdspan &F) noexcept {
        const T x0 = x[0];
        const T x1 = x[1];
        const T h  = x1 - x0;
        const T h3 = h*h*h;
        const T x02 = x0*x0;
        const T x12 = x1*x1;
        const T f0 = F(0,0);
        const T f1 = F(1,0);
        const T df0 = F(0,1);
        const T df1 = F(1,1);
        const T diff = f0 - f1;
        return {
            (f0*x12*(x1 - 3.0*x0) + f1*x02*(3.0*x1 - x0) - h*x0*x1*(df0*x1 + df1* x0))/h3,
            (+6.0*x0*x1*diff + h*( df0*x1*(2.0*x0 + x1) + df1*x0*(x0 + 2.0*x1)))/h3,
            (-3.0*(x0 + x1)*diff - h*( df0*(x0 + 2.0*x1) + df1*(2.0*x0 + x1)))/h3,
            (+2.0*diff + h*(df0 + df1))/h3
        };
    }

};


template <typename T>
class CubicInterpND<T, 1>
{
    using Vector = std::vector<T>;
    using Cell = CubicCellND<T, 1>;
    using Cells = std::vector<Cell>;
    using Span = std::span<const T>;
    using VectorN2 = cip::VectorN<T, 2>;
    using Pr = std::pair<std::size_t, std::size_t>;
public:
    CubicInterpND(const Vector &_x, const Vector &_f)
      : x(_x),
        indexer(_x),
        F(T{}, {x.size(), 2})
    {
        assert(x.size() == _f.size());
    }
    virtual ~CubicInterpND() { }

    virtual Vector calc_slopes(const Vector &x, const Vector &f) const = 0;

    void build(Vector f) // don't pass by reference but by value (to create a copy)!
    {
        const std::size_t n = x.size() - 1;
        F.move_into_submdspan(std::move(f), std::full_extent, 0);
        F.move_into_submdspan(calc_slopes(x, f), std::full_extent, 1);
        cells.reserve(n);
        for (auto i = 0; i < n; ++i)
        {
            cells.emplace_back(Span(&x[i], 2), F.submdspan(Pr{i, i+1}, std::full_extent));
        }
    }

    T eval(const T xi) const
    {
        return cells[indexer.sort_index(xi)].eval(xi);
    };

    Vector evaln(const Vector &xi) const
    {
        auto xi_iter = xi.begin();
        Vector yi(xi.size());
        for (auto &yi_i : yi)
        {
            yi_i = eval(*xi_iter++);
        }
        return yi;
    }

private:
    const Vector x;
    const cip::Indexer<T> indexer;
    Cells cells;
    VectorN2 F;

};


} // namespace cip
