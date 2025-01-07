#pragma once

#include <algorithm>
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


template <typename T>
class LinearCell1D {
    using Span = std::span<const T>;
public:
    explicit LinearCell1D(Span x, Span f)
    : x0(x[0]), b0(f[0]), a0((f[1]-f[0])/(x[1]-x[0]))
    {
    }
    ~LinearCell1D() { }

    T eval(const T &xi) const
    {
        return b0 + a0 * (xi - x0);
    }

private:
    const T x0;
    const T a0;
    const T b0;
};


template <typename T>
class LinearInterp1D {
    using Cell = LinearCell1D<T>;
    using Vector = std::vector<T>;
    using Span = std::span<const T>;
public:
    LinearInterp1D(const Vector &_x, const Vector &_f)
      : indexer(_x)
    {
        assert(_x.size() == _f.size());
        build(_x, _f);
    }
    ~LinearInterp1D() { }

    void build(const Vector &x, const Vector &f)
    {
        cells.reserve(x.size()-1);
        for (int i = 0; i < x.size()-1; ++i)
        {
            cells.push_back(Cell(Span(&x[i], 2), Span(&f[i], 2)));
        }
    }

    T eval(const T xi) const
    {
        return cells[indexer.sort_index(xi)].eval(xi);
    }

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
    const cip::Indexer<T> indexer;
    std::vector<Cell> cells;
}; // class LinearInterp1D



template <typename T, std::size_t N=2>
class LinearCell2D {
    using Span = std::span<const T>;
    using Spans = std::array<Span, N>;
    using Mdspan = std::mdspan<const T, std::dextents<std::size_t, N>, std::layout_stride>;
public:
    template <typename... Args>
    explicit LinearCell2D(const Mdspan &_f, Args && ... _x)
    : x{std::forward<Args>(_x) ...},
      f(_f),
      H(std::transform_reduce(x.begin(), x.end(), T{1}, std::multiplies<>{},
        [](const Span& xi) { return xi[1] - xi[0]; }))
    {
    }

    template <typename... Args>
    T eval(Args && ... xi) const {
        std::array<T, N> xi_array{std::forward<Args>(xi) ...};
        T result = 0.0;
        for (std::size_t corner = 0; corner < (1 << N); ++corner) {
            result += eval_corner(corner, xi_array);
        }
        return result/H;
    }

private:
    const Spans x;
    const Mdspan f;
    const T H;


    const T eval_corner(const std::size_t corner, const std::array<T, N> &xi) const {
        T weight = 1.0;
        std::array<std::size_t, N> indices;
        for (std::size_t dim = 0; dim < N; ++dim) {
            const bool is_upper = corner & (1 << dim);
            weight *= is_upper ? xi[dim] - x[dim][0] : x[dim][1] - xi[dim];
            indices[dim] = is_upper ? 1 : 0;
        }
        return weight*f(indices);
    }

}; // class LinearCell2D


template <typename T, std::size_t N=2>
class LinearInterp2D {
    using Array = std::array<std::vector<T>, N>;
    using Vector = std::vector<T>;
    using VectorN2 = cip::VectorN<T, N>;
    using Cell = LinearCell2D<T>;
    using Span = std::span<const T>;
    using Pr = std::pair<size_t, size_t>;
    using IndexerT = cip::Indexer<T>;
    using Indexers = std::array<cip::Indexer<T>, N>;
public:
    LinearInterp2D(const Array &_xi, const VectorN2 &_f)
    : xi(_xi),
      f(_f),
      indexers(std::apply([](const auto &...x) {return Indexers{IndexerT(x)...};},xi))
    {
        build();
    }
    ~LinearInterp2D() { }

    void build() {
        cells.reserve(xi[0].size() - 1);
        for (size_t i = 0; i < xi[0].size()-1; ++i) {
            std::vector<Cell> row;
            row.reserve(xi[1].size() - 1);
            for (size_t j = 0; j < xi[1].size()-1; ++j) {
                row.emplace_back(f.submdspan(Pr{i, i + 1}, Pr{j, j + 1}),
                                 Span(&xi[0][i], 2),
                                 Span(&xi[1][j], 2)
                );
            }
            cells.emplace_back(std::move(row));
        }
    }

    T eval(const T xi, const T yi) const
    {
        size_t x_index = indexers[0].sort_index(xi);
        size_t y_index = indexers[1].sort_index(yi);
        return cells[x_index][y_index].eval(xi, yi);
    }

    Vector evaln(const Vector &x, const Vector &y) const
    {
        auto xi_iter = x.begin();
        auto yi_iter = y.begin();
        Vector z(x.size());
        for (auto &zi : z)
        {
            zi = eval(*xi_iter++, *yi_iter++);
        }
        return z;
    }


private:
    const Array xi;
    const VectorN2 f;
    const Indexers indexers;
    std::vector<std::vector<Cell>> cells;

};


// template <typename T, std::size_t N, typename... Vars>
// class LinearCellND {
//     using VectorN = vec::VectorN<T, N>;
//     using Vector2 = std::array<std::vector<T>, N>;
// public:
//     explicit LinearCellND(const Vars&... vars, const VectorN &_f)
//     : f(_f)
//     {
//         (x_i.push_back(vars), ...);
//     }

// private:
//     const VectorN f;
//     Vector2 x_i;

// }; // class LinearInterpND


} // namespace cip
