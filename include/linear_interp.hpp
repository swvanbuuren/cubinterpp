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
    : x(_x),
      f(_f),
      H(std::transform_reduce(x.begin(), x.end(), T{1}, std::multiplies<>{},
        [](const Span& xi) { return xi[1] - xi[0]; })),
      c(compute_coefficients())
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
    const Spans x;
    const Mdspan f;
    const T H;
    const NomArray c;

    NomArray compute_coefficients() const {
        NomArray c;
        for (std::size_t J = 0; J < numCorners; ++J) {
            c[J] = compute_c_J(J);
        }
        return c;
    }

    T compute_c_J(std::size_t J) const {
        T c_J = T{0};
        for (std::size_t mask = 0; mask < numCorners; ++mask) {
            T prod = T{1};
            std::array<std::size_t, N> indices{};
            for (std::size_t k = 0; k < N; ++k) {
                indices[k] = (mask & (1u << k)) ? 1 : 0;
                prod *= gamma(J, indices[k], k);
            }
            c_J += f(indices) * prod;
        }
        return c_J / H;
    }

    T gamma(std::size_t J, std::size_t i, std::size_t k) const {
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



template <typename T>
class LinearCellND<T, 1> {
    using Span = std::span<const T>;
public:
    explicit LinearCellND(Span x, Span f)
    : x0(x[0]),
      b0((x[1]*f[0]-x[0]*f[1])/(x[1]-x[0])),
      a0((f[1]-f[0])/(x[1]-x[0]))
    {
    }
    ~LinearCellND() { }

    T eval(const T &xi) const
    {
        return b0 + a0*xi;
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
    explicit LinearInterp1D(const Vector &x, const Vector &f)
    : LinearInterpND<T, 1>(f, x)
    {}

    ~LinearInterp1D() { }
};



template <typename T>
class LinearInterp2D : public LinearInterpND<T, 2> {
    using Vector = std::vector<T>;
    using Vector2 = cip::VectorN<T, 2>;
public:
    explicit LinearInterp2D(const Vector &x, const Vector &y, const Vector2 &f)
    : LinearInterpND<T, 2>(f, x, y)
    {}

    ~LinearInterp2D() { }
};


template <typename T>
class LinearInterp3D : public LinearInterpND<T, 3> {
    using Vector = std::vector<T>;
    using Vector3 = cip::VectorN<T, 3>;
public:
    explicit LinearInterp3D(const Vector &x, const Vector &y, const Vector &z, const Vector3 &f)
    : LinearInterpND<T, 3>(f, x, y, z)
    {}

    ~LinearInterp3D() { }
};



} // namespace cip
