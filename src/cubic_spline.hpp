#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
#include <mdspan/mdspan.hpp>
#include "utils.hpp"
#include "vectorn.hpp"


namespace cip {


template<typename T>
class CubicCell1D
{
    using Array = std::array<T, 4>;
    using Alpha = std::array<Array, 4>;
    using Span = std::span<const T>;
    using Mdspan = std::mdspan<const T, std::dextents<std::size_t, 2>, std::layout_stride>;
public:
    explicit CubicCell1D(const Span &x, const Mdspan &F)
      : coeffs(calc_coeffs(x, F))
    {
    }
    ~CubicCell1D() = default;

    const T eval(const T x) const
    {
        return coeffs[0] + (coeffs[1] + (coeffs[2] + coeffs[3]*x)*x)*x;
    }

private:
    const Array coeffs;

    const Alpha calc_alphas(const Span &x) const 
    {
        const T x0 = x[0];
        const T x1 = x[1];
        const T x02 = x0*x0;
        const T x12 = x1*x1;
        return {{{x12*(x1 - 3.0*x0), +6.0*x0*x1,           -3.0*(x0 + x1), +2.0},
                 {x02*(3.0*x1 - x0), -6.0*x0*x1,           +3.0*(x0 + x1), -2.0},
                 {-x0*x12,           x1*(2.0*x0 + x1),     -(x0 + 2.0*x1), +1.0},
                 {-x1*x02,           x0*(x0 + 2.0*x1),     -(2.0*x0 + x1), +1.0}}};
    }

    const Array calc_coeffs_old(const Span &x, const Mdspan &F) const
    {
        const T h = x[1] - x[0];
        const T h2 = h*h;
        const T h3 = h2*h;
        const Alpha alpha = calc_alphas(x);
        Array coefficients;
        std::size_t i = 0;
        for (auto &coeff : coefficients) {
            // note the scaling of df, which arises due to differentiation with respect to x
            // (which is scaled by h)
            coeff =  (F(0,0)*alpha[0][i]
                  +   F(1,0)*alpha[1][i]
                  + h*F(0,1)*alpha[2][i]
                  + h*F(1,1)*alpha[3][i++])/h3;
        }
        return coefficients;
    }

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
class CubicInterp1D
{
    using Vector = std::vector<T>;
    using Cell = CubicCell1D<T>;
    using Cells = std::vector<Cell>;
    using Span = std::span<const T>;
    using VectorN2 = cip::VectorN<T, 2>;
    using Pr = std::pair<std::size_t, std::size_t>;
public:
    CubicInterp1D(const Vector &_x, const Vector &_f)
      : x(_x),
        indexer(_x),
        F(T{}, {x.size(), 2})
    {
        assert(x.size() == _f.size());
    }
    virtual ~CubicInterp1D() { }

    virtual Vector calc_slopes(const Vector &x, const Vector &f) const = 0;

    void build(Vector f) // don't pass by reference but by value (to create a copy)!
    {
        const std::size_t n = x.size();
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
