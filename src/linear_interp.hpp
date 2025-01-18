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
public:
    template <typename... Args>
    explicit LinearCellND(const Mdspan &_f, const Spans &_x)
    : x(_x),
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

}; // class LinearCellND


template <typename T, std::size_t N>
class LinearInterpND {
    using Vector = std::vector<T>;
    using Array = std::array<Vector, N>;
    using VectorN = cip::VectorN<T, N>;
    using Cell = LinearCellND<T, N>;
    using Cells = cip::VectorN<Cell, N>;
    using Span = std::span<const T>;
    using Pr = std::pair<std::size_t, std::size_t>;
    using Indexers = std::array<cip::Indexer<T>, N>;
public:
    template <typename... Args>
    LinearInterpND(const VectorN &_f, const Args & ... _xi)
    : xi{_xi...}, 
      f(_f), 
      indexers{cip::Indexer<T>(_xi)...}, 
      cells(build(_xi...))
    {
    }

    ~LinearInterpND() { }

    template <typename... Args>
    T eval(const Args&... args) const
    {
        std::size_t dim = 0;
        std::array<size_t, N> indices = { indexers[dim++].sort_index(args)... };
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
    const VectorN f;
    const Indexers indexers;
    const Cells cells;

    template <typename... Args>
    const Cells build(const Args & ... _xi) const
    {
        Cells _cells({(_xi.size()-1)...});
        build_cell(_cells);
        return _cells;
    }


    template <typename... Indices>
    typename std::enable_if<(sizeof...(Indices) == N), void>::type
    build_cell(Cells & _cells, Indices... indices) const
    {
        std::size_t index = 0;
        std::array<Span, N> spans = {Span(&xi[index++][indices], 2)...};
        _cells.emplace_back(
            f.submdspan(Pr{indices, indices+1}...),
            spans
        );
    }

    template <typename... Indices>
    typename std::enable_if<(sizeof...(Indices) < N), void>::type
    build_cell(Cells &_cells, Indices... indices) const
    {
        for (std::size_t i = 0; i < xi[sizeof...(indices)].size()-1; ++i)
        {
            build_cell(_cells, indices..., i);
        }
    }

}; // class LinearInterpND



template <typename T>
class LinearCellND<T, 1> {
    using Span = std::span<const T>;
public:
    explicit LinearCellND(Span x, Span f)
    : x0(x[0]), b0(f[0]), a0((f[1]-f[0])/(x[1]-x[0]))
    {
    }
    ~LinearCellND() { }

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
class LinearInterpND<T, 1> {
    using Cell = LinearCellND<T, 1>;
    using Vector = std::vector<T>;
    using Span = std::span<const T>;
public:
    LinearInterpND(const Vector &_f, const Vector &_x)
      : indexer(_x)
    {
        assert(_x.size() == _f.size());
        build(_x, _f);
    }
    ~LinearInterpND() { }

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
}; // class LinearInterpND case N=1


template <typename T>
class LinearInterp1D : public LinearInterpND<T, 1> {
    using Vector = std::vector<T>;
    using Vector2 = cip::VectorN<T, 1>;
public:
    LinearInterp1D(const Vector &x, const Vector &f)
    : LinearInterpND<T, 1>(f, x)
    {}

    ~LinearInterp1D() { }
};



template <typename T>
class LinearInterp2D : public LinearInterpND<T, 2> {
    using Vector = std::vector<T>;
    using Vector2 = cip::VectorN<T, 2>;
public:
    LinearInterp2D(const Vector &x, const Vector &y, const Vector2 &f)
    : LinearInterpND<T, 2>(f, x, y)
    {}

    ~LinearInterp2D() { }
};


} // namespace cip
