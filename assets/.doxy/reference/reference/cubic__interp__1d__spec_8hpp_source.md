

# File cubic\_interp\_1d\_spec.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**old**](dir_3ccffc210a036acba650f09ebe71329d.md) **>** [**cubic\_interp\_1d\_spec.hpp**](cubic__interp__1d__spec_8hpp.md)

[Go to the documentation of this file](cubic__interp__1d__spec_8hpp.md)


```C++
// N=1 specializations of CubicCellND and CubicInterpND.
//
// These are kept for reference. The general N-dimensional templates in
// cubic_interp.hpp handle N=1 correctly, producing identical results
// through compile-time unrolling of the coefficient and evaluation loops.
//
// This file is intentionally NOT included by cubinterpp.hpp.

#pragma once

#include <cassert>
#include <cstddef>
#include <vector>
#include <mdspan/mdspan.hpp>
#include "cubic_interp.hpp"


namespace cip {


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


template <typename T, IndexMethod IM>
class CubicInterpND<T, 1, IM>
{
    using Vector = std::vector<T>;
    using Cell = CubicCellND<T, 1>;
    using Cells = std::vector<Cell>;
    using Span = std::span<const T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
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

    virtual Vector calc_slopes(const Vector &x, const Mdspan1D &f) const = 0;

    void build(Vector f) // don't pass by reference but by value (to create a copy)!
    {
        const std::size_t n = x.size() - 1;
        F.move_into_submdspan(std::move(f), std::full_extent, 0);
        F.move_into_submdspan(calc_slopes(x, F.submdspan_1d(std::full_extent, 0)), std::full_extent, 1);
        cells.reserve(n);
        for (auto i = 0; i < n; ++i)
        {
            cells.emplace_back(Span(&x[i], 2), F.submdspan(Pr{i, i+1}, std::full_extent));
        }
    }

    T eval(const T xi) const
    {
        return cells[indexer.index(xi)].eval(xi);
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
    const cip::Indexer<T, IM> indexer;
    Cells cells;
    VectorN2 F;

}; // class CubicInterpND<T, 1, IM>


} // namespace cip
```


