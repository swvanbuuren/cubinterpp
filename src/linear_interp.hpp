#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
#include <mdspan/mdspan.hpp>
#include <utility>
#include "vectorn.hpp"


namespace lns {

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
class Indexer {
    using Vector = std::vector<T>;
public:
    Indexer(const Vector &_x)
    : x(_x),
      index_back(x.size()-2),
      x_front(x[index_front]),
      x_back(x[x.size()-1]),
      x_delta((x_back-x_front)/(x.size()-1))
    {
    }
    ~Indexer() { }

    const size_t cell_index(const T xi) const
    {
        return
        (xi < x_back) ?
            ((xi < x_front) ?
                index_front :
                (size_t)((xi-x_front)/x_delta)) :
            index_back;
    }

    const size_t sort_index(const T xi) const
    {
        if (xi < x_front)
        {
            return index_front;
        }
        if (xi >= x_back)
        {
            return index_back;
        }
        return std::distance(x.begin(), std::upper_bound(x.begin(), x.end(), xi)) - 1;
    }

private:
    const Vector x;
    const size_t index_front = 0;
    const size_t index_back;
    const double x_front;
    const double x_back;
    const double x_delta;
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
    const Indexer<T> indexer;
    std::vector<Cell> cells;
}; // class LinearInterp1D



template <typename T>
class LinearCell2D {
    using Span = std::span<const T>;
    using Mdspan = std::mdspan<const T, std::dextents<std::size_t, 2>, std::layout_stride>;
public:
    explicit LinearCell2D(const Span &_x1, const Span &_x2, const Mdspan &_f)
    : x1(_x1), x2(_x2), f(_f), H(1.0 / ((x1[1] - x1[0]) * (x2[1] - x2[0])))
    {
    }

    T eval(const T x1i, const T x2i) const
    {
        const T x1i_x10 = x1i - x1[0];
        const T x11_x1i = x1[1] - x1i;
        const T x2i_x20 = x2i - x2[0];
        const T x21_x2i = x2[1] - x2i;
        return H*(f(0, 0)*x11_x1i*x21_x2i
                + f(0, 1)*x11_x1i*x2i_x20
                + f(1, 0)*x1i_x10*x21_x2i
                + f(1, 1)*x1i_x10*x2i_x20);
    }

private:
    const Span x1;
    const Span x2;
    const Mdspan f;
    const T H;

}; // class LinearCell2D


template <typename T>
class LinearInterp2D {
    using Vector = std::vector<T>;
    using Vector2 = std::vector<std::vector<T>>;
    using VectorN2 = vec::VectorN<T, 2>;
    using Cell = LinearCell2D<T>;
    using Span = std::span<const T>;
    using Pr = std::pair<size_t, size_t>;
    using Indexers = std::vector<Indexer<T>>;
public:
    LinearInterp2D(const Vector &_x, const Vector &_y, const Vector2 &_f)
    : x(_x), y(_y), f(_f)
    {
        create_indexers(x, y);
        build();
    }
    ~LinearInterp2D() { }

    void build() {
        cells.reserve(x.size() - 1);
        for (size_t i = 0; i < x.size()-1; ++i) {
            std::vector<Cell> row;
            row.reserve(y.size() - 1);
            for (size_t j = 0; j < y.size()-1; ++j) {
                row.emplace_back(Span(&x[i], 2),
                                 Span(&y[j], 2),
                                 f.submdspan(Pr{i, i + 1}, Pr{j, j + 1})
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
    const Vector x;
    const Vector y;
    const VectorN2 f;
    Indexers indexers;
    std::vector<std::vector<Cell>> cells;

    template<typename... Args>
    void create_indexers(const Args & ... args) {
        // loop through args and indexers simultaneously and pupulate indexers with args
        for (const auto &arg : {args...}) {
            indexers.emplace_back(arg);
        }
    }
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


} // namespace lns
