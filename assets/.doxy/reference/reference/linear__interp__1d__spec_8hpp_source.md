

# File linear\_interp\_1d\_spec.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**old**](dir_3ccffc210a036acba650f09ebe71329d.md) **>** [**linear\_interp\_1d\_spec.hpp**](linear__interp__1d__spec_8hpp.md)

[Go to the documentation of this file](linear__interp__1d__spec_8hpp.md)


```C++
// N=1 specializations of LinearCellND and LinearInterpND.
//
// These are kept for reference. The general N-dimensional templates in
// linear_interp.hpp handle N=1 correctly, producing identical results
// more efficiently through compile-time unrolling.
//
// This file is intentionally NOT included by cubinterpp.hpp.

#pragma once

#include <cassert>
#include <cstddef>
#include <vector>
#include <mdspan/mdspan.hpp>
#include "linear_interp.hpp"


namespace cip {


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


template <typename T, IndexMethod IM>
class LinearInterpND<T, 1, IM> {
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
        return cells[indexer.index(xi)].eval(xi);
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
    const cip::Indexer<T, IM> indexer;
    std::vector<Cell> cells;
}; // class LinearInterpND<T, 1, IM>


} // namespace cip
```


