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
        const T x0x1 = x0*x1;
        const T x0_p_x1 = x0 + x1;
        const T two_x0_p_x1 = 2.0*x0 + x1;
        const T x0_p_two_x1 = x0 + 2.0*x1;
        return {{{x12*(x1 - 3.0*x0), +6.0*x0x1,          -3.0*x0_p_x1,  +2.0},
                 {-x0*x12,           x1*two_x0_p_x1,     -x0_p_two_x1,  +1.0},
                 {x02*(3.0*x1 - x0), -6.0*x0x1,          +3.0*x0_p_x1,  -2.0},
                 {-x1*x02,           x0*x0_p_two_x1,     -two_x0_p_x1,  +1.0}}};
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
        const T inv_h3 = T{1} / calc_h3(h);
        const Delta delta = calc_delta(x);

        // term[k][m][ij] = h_factor * delta[k][ij][m];  h_factor = (j_k == 0) ? 1 : h[k]
        std::array<std::array<std::array<T, 4>, 4>, N> term{};
        for (std::size_t k = 0; k < N; ++k)
            for (std::size_t ij = 0; ij < 4; ++ij) {
                const T hf = (ij & 1) ? h[k] : T{1};
                for (std::size_t m = 0; m < 4; ++m)
                    term[k][m][ij] = hf * delta[k][ij][m];
            }

        // Flat buffer with interleaved bit layout: for dim k, i_k at bit 2*(N-1-k)+1, j_k at bit 2*(N-1-k).
        std::array<std::size_t, 2 * N> idx{};
        CoeffsArray buf{};
        for (std::size_t flat = 0; flat < numCoeffs; ++flat) {
            for (std::size_t k = 0; k < N; ++k) {
                const std::size_t sh = 2 * (N - 1 - k);
                idx[k]     = (flat >> (sh + 1)) & 1;   // i_k
                idx[k + N] = (flat >> sh) & 1;         // j_k
            }
            buf[flat] = F(idx);
        }

        // N sequential 1-D transforms: contract each (i_k, j_k) bit-pair into m_k.
        for (std::size_t k = 0; k < N; ++k) {
            const std::size_t shift = 2 * (N - 1 - k);
            const std::size_t mask_lo = (std::size_t{1} << shift) - 1;
            const auto &term_k = term[k];
            CoeffsArray tmp{};
            for (std::size_t outer = 0; outer < (numCoeffs >> 2); ++outer) {
                const std::size_t lo = outer & mask_lo;
                const std::size_t base = ((outer - lo) << 2) | lo;
                for (std::size_t m = 0; m < 4; ++m) {
                    T acc{};
                    for (std::size_t ij = 0; ij < 4; ++ij)
                        acc += term_k[m][ij] * buf[base | (ij << shift)];
                    tmp[base | (m << shift)] = acc;
                }
            }
            std::swap(buf, tmp);
        }

        for (auto &c : buf) c *= inv_h3;
        return buf;
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
        indexers(std::apply([](const auto&... v) -> Indexers {
            return Indexers{ cip::Indexer<T, IM>(std::span<const T>(v))... };
        }, xi)),
        F(T{}, {_xi.size()..., ((void)_xi, size_t_two)...}),
        cells(std::array<std::size_t, N>{(_xi.size()-1)...})
    {
    }
    virtual ~CubicInterpND() { }

    CubicInterpND(CubicInterpND&&)            = delete;
    CubicInterpND& operator=(CubicInterpND&&) = delete;

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
