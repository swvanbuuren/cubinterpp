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
    static constexpr std::size_t order = 4;
    using Array = std::array<T, order>;
    using Alpha = std::array<Array, order>;
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


template<typename T, std::size_t N=2>
class CubicCell2D
{
    static constexpr std::size_t order = 4;
    static constexpr std::size_t numCorners = 1 << N;
    using Array = std::array<T, order>;
    using Array2 = std::array<Array, order>;
    using Alphas = std::array<Array2, N>;
    using Span = std::span<const T>;
    using Mdspan = std::mdspan<const T, std::dextents<std::size_t, 2*N>, std::layout_stride>;
public:
    explicit CubicCell2D(const Span &x, const Span &y, const Mdspan &F)
      : coeffs(calc_coeffs(x, y, F))
    {
    }
    ~CubicCell2D() = default;

    const T eval(const T x0, const T x1) const
    {
        // Evaluate the inner polynomial for each row in x1
        const Array x1_coeffs {
            eval_poly(coeffs[0], x1),
            eval_poly(coeffs[1], x1),
            eval_poly(coeffs[2], x1),
            eval_poly(coeffs[3], x1)
        };
        // Evaluate the outer polynomial in x0
        return eval_poly(x1_coeffs, x0);
    }

private:
    const Array2 coeffs;

    const Array2 calc_alphas(const Span &xi) const 
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

    const Array2 calc_coeffs(const Span &x0, const Span &x1, const Mdspan &F) const
    {
        const T h0 = x0[1] - x0[0];
        const T h1 = x1[1] - x1[0];
        const T h3 = h0*h0*h0*h1*h1*h1;
        const Alphas alphas = {calc_alphas(x0), calc_alphas(x1)};
        Array2 coeffs = {};
        for (std::size_t k = 0; k < order; ++k)
        {
            for (std::size_t l = 0; l < order; ++l)
            {
                for (std::size_t i = 0; i < numCorners; ++i)
                {
                    std::size_t i1 = (i >> 0) & 1;
                    std::size_t i2 = (i >> 1) & 1;
                    for (std::size_t j = 0; j < numCorners; ++j)
                    {
                        std::size_t j1 = (j >> 0) & 1;
                        std::size_t j2 = (j >> 1) & 1;
                        std::size_t alpha_index1 = (i1 << 1) | j1;
                        std::size_t alpha_index2 = (i2 << 1) | j2;
                        const T prod_h = (j1 ? h0 : 1.0)*(j2 ? h1 : 1.0);
                        coeffs[k][l] += prod_h*F(i1,i2,j1,j2)*alphas[0][alpha_index1][k]*alphas[1][alpha_index2][l];
                    }
                }
                coeffs[k][l] /= h3;
            }
        }
        return coeffs;
    }

    const T eval_poly(const Array &c, const T xi) const
    {
        return c[0] + xi*(c[1] + xi*(c[2] + xi*c[3]));
    }


};



template <typename T, std::size_t N=2>
class CubicInterp2D
{
    using Vector = std::vector<T>;
    using Cell = CubicCell2D<T>;
    using Cells = cip::VectorN<Cell, N>;
    using Span = std::span<const T>;
    using Mdspan = std::mdspan<T, std::dextents<std::size_t, N>, std::layout_stride>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
    using VectorN2 = cip::VectorN<T, 2*N>;
    using Pr = std::pair<std::size_t, std::size_t>;
public:
CubicInterp2D(const Vector &_x, const Vector &_y, const VectorN &_f)
      : x(_x),
        y(_y),
        x_indexer(_x),
        y_indexer(_y),
        F(T{}, {x.size(), y.size(), 2, 2}),
        cells({x.size()-1, y.size()-1})
    {
    }
    virtual ~CubicInterp2D() { }

    virtual Vector calc_slopes(const Vector &x, const Mdspan1D &f) const = 0;

    void build(const VectorN &f)
    {
        populate_F(f);
        const std::size_t nx = x.size();
        const std::size_t ny = y.size();
        for (auto i = 0; i < nx-1; ++i)
        {
            for (auto j = 0; j < ny-1; ++j)
            {
                cells.emplace_back(
                    Span(&x[i], 2), Span(&y[j], 2),
                    F.submdspan(Pr{i, i+1}, Pr{j, j+1}, std::full_extent, std::full_extent));
            }
            
        }
    }

    T eval(const T xi, const T yi) const
    {
        return cells(x_indexer.sort_index(xi), 
                     y_indexer.sort_index(yi)).eval(xi, yi);
    };


private:
    const Vector x;
    const Vector y;
    const cip::Indexer<T> x_indexer;
    const cip::Indexer<T> y_indexer;
    Cells cells;
    VectorN2 F;

    void populate_F(VectorN f) { // pass by value, to create a copy, which will be moved into F
        const std::size_t nx = x.size();
        const std::size_t ny = y.size();
        // First fill in the normal values of f
        F.move_into_submdspan(std::move(f), std::full_extent, std::full_extent, 0, 0);
        // Now fill in the slopes
        // df/dx
        for (std::size_t i = 0; i < ny; ++i)
        {
            auto fx = F.submdspan_1d(std::full_extent, i, 0, 0);
            F.move_into_submdspan(calc_slopes(x, fx), std::full_extent, i, 1, 0);

        }
        // df/dy
        for (std::size_t i = 0; i < nx; ++i)
        {
            auto fy = F.submdspan_1d(i, std::full_extent, 0, 0);
            F.move_into_submdspan(calc_slopes(y, fy), i, std::full_extent, 0, 1);
        }
        // d2f/dxdy
        for (std::size_t i = 0; i < nx; ++i)
        {
            auto dfdx = F.submdspan_1d(i, std::full_extent, 1, 0);
            F.move_into_submdspan(calc_slopes(y, dfdx), i, std::full_extent, 1, 1);
        }
    }

};


} // namespace cip
