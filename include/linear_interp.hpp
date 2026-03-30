#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
#include <mdspan/mdspan.hpp>
#include <utility>
#include <numeric>
#include <tuple>
#include "vectorn.hpp"
#include "utils.hpp"


namespace cip {


template <typename T, std::size_t N>
class LinearCellND {
    using Span = std::span<const T>;
    using Spans = std::array<Span, N>;
    using Mdspan = std::mdspan<const T, std::dextents<std::size_t, N>, std::layout_stride>;
    using NomArray = std::array<T, (1 << N)>;
    static constexpr std::size_t numCorners = 1 << N;
public:
    explicit LinearCellND(const Mdspan &_f, const Spans &_x)
    : c(compute_coefficients(_f, _x))
    {
    }

    template <typename... Args>
    requires (sizeof...(Args) == N)
    T eval(Args&&... args) const
    {
        return gather_corners(
            {std::forward<Args>(args)...},
            std::make_index_sequence<numCorners>{}
        );
    }


private:
    const NomArray c;

    static NomArray compute_coefficients(const Mdspan &f, const Spans &x) {
        const T H = std::transform_reduce(x.begin(), x.end(), T{1}, std::multiplies<>{},
            [](const Span& xi) { return xi[1] - xi[0]; });
        NomArray c;
        for (std::size_t J = 0; J < numCorners; ++J) {
            c[J] = compute_c_J(f, x, H, J);
        }
        return c;
    }

    static T compute_c_J(const Mdspan &f, const Spans &x, const T H, std::size_t J) {
        T c_J = T{0};
        for (std::size_t mask = 0; mask < numCorners; ++mask) {
            T prod = T{1};
            std::array<std::size_t, N> indices{};
            for (std::size_t k = 0; k < N; ++k) {
                indices[k] = (mask & (1u << k)) ? 1 : 0;
                prod *= gamma(x, J, indices[k], k);
            }
            c_J += f(indices) * prod;
        }
        return c_J / H;
    }

    static T gamma(const Spans &x, std::size_t J, std::size_t i, std::size_t k) {
        return (J & (1u << k))
            ? (i == 0 ? -T{1} : T{1})
            : (i == 0 ? x[k][1] : -x[k][0]);
    }

    template <std::size_t... Js>
    T gather_corners(const std::array<T, N>& xs, std::index_sequence<Js...>) const {
        return ( ... + compute_corner<Js>(xs, std::make_index_sequence<N>{}) );
    }

    template <std::size_t J, std::size_t... I>
    T compute_corner(const std::array<T, N>& xs, std::index_sequence<I...>) const {
        return c[J] * (T{1} * ... * ((J & (1u << I)) ? xs[I] : T{1}));
    }

}; // class LinearCellND


template <typename T, std::size_t N, IndexMethod IM = IndexMethod::BinarySearch>
class LinearInterpND {
    using Vector = std::vector<T>;
    using Array = std::array<Vector, N>;
    using F = cip::VectorN<T, N>;
    using Cell = LinearCellND<T, N>;
    using Cells = cip::VectorN<Cell, N>;
    using Span = std::span<const T>;
    using Pr = std::pair<std::size_t, std::size_t>;
    using Indexers = std::array<cip::Indexer<T, IM>, N>;
public:
    template <typename... Args>
    LinearInterpND(const F &_f, const Args & ... _xi)
    : xi{_xi...}, 
      f(_f), 
      indexers{cip::Indexer<T, IM>(_xi)...}, 
      cells(build(_xi...))
    {
    }

    ~LinearInterpND() { }

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
        if (!((inputs.size() == size) && ...)) {
            throw std::invalid_argument("All input vectors must have the same size");
        }

        Vector z(size); // Output vector
        for (std::size_t i = 0; i < size; ++i) {
            z[i] = eval(inputs[i]...);
        }
        return z;
    }


private:
    const Array xi;
    const F f;
    const Indexers indexers;
    const Cells cells;

    template <typename... Args>
    const Cells build(const Args & ... _xi) const
    {
        const std::array<std::size_t, N> dims = {(_xi.size()-1)...};
        Cells _cells(dims);
        build_cell(_cells);
        return _cells;
    }

    template <typename... Indices>
    void build_cell(Cells &_cells, Indices... indices) const {
        if constexpr (sizeof...(Indices) == N) {
            std::size_t index = 0;
            std::array<Span, N> spans = {Span(&xi[index++][indices], 2)...};
            _cells.emplace_back(
                f.submdspan(Pr{indices, indices+1}...),
                spans
            );
        } else {
            for (std::size_t i = 0; i < xi[sizeof...(indices)].size()-1; ++i) {
                build_cell(_cells, indices..., i);
            }
        }
    }

}; // class LinearInterpND


} // namespace cip
