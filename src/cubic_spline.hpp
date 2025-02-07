#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
#include <mdspan/mdspan.hpp>
#include "utils.hpp"


namespace cip {


template<typename T>
class CubicCell1D
{
    using Array = std::array<T, 4>;
    using Alpha = std::array<Array, 4>;
    using Span = std::span<const T>;
public:
    explicit CubicCell1D(Span x, Span f, Span df)
      : coeffs(calc_coeffs(x, f, df))
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

    const Array calc_coeffs_old(const Span &x, const Span &f, const Span &df) const
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
            coeff =   (f[0]*alpha[0][i]
                  +    f[1]*alpha[1][i]
                  + h*df[0]*alpha[2][i]
                  + h*df[1]*alpha[3][i++])/h3;
        }
        return coefficients;
    }

    constexpr Array calc_coeffs(const Span &x, const Span &f, const Span &df) noexcept {
        const T x0 = x[0];
        const T x1 = x[1];
        const T h  = x1 - x0;
        const T h3 = h*h*h;
        const T x02 = x0*x0;
        const T x12 = x1*x1;
        const T diff = f[0] - f[1];
        return {
            ( f[0]*x12*(x1 - 3.0*x0)
            + f[1]*x02*(3.0*x1 - x0)
            - h*x0*x1*(df[0]*x1 + df[1]* x0))/h3,
            (6.0*x0*x1*diff
            + h*( df[0]*x1*(2.0*x0 + x1)
                + df[1]*x0*(x0 + 2.0*x1)))/h3,
            (-3.0*(x0 + x1)*diff
            - h*( df[0]*(x0 + 2.0*x1) + df[1]*(2.0*x0 + x1)))/h3,
            (2.0*diff
            + h*(df[0] + df[1]))/h3
        };
    }

};


template <typename T>
class BaseSpline
{
    using Vector = std::vector<T>;
    using Cell = CubicCell1D<T>;
    using Cells = std::vector<Cell>;
    using Span = std::span<const T>;
public:
    BaseSpline(const Vector &_x, const Vector &_f)
      : x(_x),
        f(_f),
        indexer(_x)
    {
        assert(_x.size() == _f.size());
    }
    virtual ~BaseSpline() { }

    virtual const Vector calc_slopes(const Vector &x, const Vector &y) const = 0;

    void build()
    {
        df = calc_slopes(x, f);
        cells.reserve(x.size()-1);
        for (int i = 0; i < x.size()-1; ++i)
        {
            cells.emplace_back(Span(&x[i], 2), Span(&f[i], 2), Span(&df[i], 2));
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
    const Vector f;
    const cip::Indexer<T> indexer;
    Cells cells;
    Vector df;

};


template <typename T>
class MonotonicSpline1D : public BaseSpline<T>
{
    using Vector = std::vector<T>;
public:
    MonotonicSpline1D(const Vector &x, const Vector &y)
    : BaseSpline<T>(x, y)

    {
        this->build();
    }
    ~MonotonicSpline1D() { }
    const Vector calc_slopes(const Vector &x, const Vector &y) const override
    {
        // See https://en.wikipedia.org/wiki/Monotone_cubic_interpolation
        auto N = x.size();

        Vector secants(N-1);
        Vector tangents(N);

        for (auto k = 0; k < N-1; ++k)
        {
            secants[k] = (y[k+1] - y[k]) / (x[k+1] - x[k]);
        }

        tangents[0] = secants[0];
        for (auto k = 1; k < N-1; ++k)
        {
            tangents[k] = 0.5*(secants[k-1] + secants[k]);
        }
        tangents[N-1] = secants[N-2];

        for (auto k = 0; k < N-1; ++k)
        {
            if (secants[k] == 0.0)
            {
                tangents[k] = 0.0;
                tangents[k+1] = 0.0;
            } else {
                T alpha = tangents[k] / secants[k];
                T beta = tangents[k + 1] / secants[k];
                T h = std::hypot(alpha, beta);
                if (h > 3.0)
                {
                    tangents[k] = 3.0/h*alpha*secants[k];
                    tangents[k+1] = 3.0/h*beta*secants[k];
                }
            }
        }
        return tangents;
    }
};


template <typename T>
class AkimaSpline1D : public BaseSpline<T>
{
    using Vector = std::vector<T>;
public:
    AkimaSpline1D(const Vector &x, const Vector &y)
    : BaseSpline<T>(x, y)

    {
        this->build();
    }
    ~AkimaSpline1D() { }
    const Vector calc_slopes(const Vector &x, const Vector &y) const override
    {
        /*
        Derivative values for Akima cubic Hermite interpolation

        Akima's derivative estimate at grid node x(i) requires the four finite
        differences corresponding to the five grid nodes x(i-2:i+2).
        For boundary grid nodes x(1:2) and x(n-1:n), append finite differences
        which would correspond to x(-1:0) and x(n+1:n+2) by using the following
        uncentered difference formula correspondin to quadratic extrapolation
        using the quadratic polynomial defined by data at x(1:3)
        (section 2.3 in Akima's paper):
        */

        Vector delta(x.size()-1);
        for (auto i = 0; i < delta.size(); ++i)
        {
            delta[i] = (y[i+1] - y[i])/(x[i+1] - x[i]);
        }

        auto n = x.size();
        T delta_0 = 2.0*delta[0] - delta[1];
        T delta_m1 = 2.0*delta_0 - delta[0];
        T delta_n  = 2.0*delta[n-2] - delta[n-3];
        T delta_n1 = 2.0*delta_n - delta[n-2];

        Vector delta_new(delta.size() + 4);
        delta_new[0] = delta_m1;
        delta_new[1] = delta_0;
        delta_new[delta_new.size()-2] = delta_n;
        delta_new[delta_new.size()-1] = delta_n1;
        for (auto i = 0; i < delta.size(); ++i)
        {
            delta_new[i+2] = delta[i];
        }

        /*
        Akima's derivative estimate formula (equation (1) in the paper):

                H. Akima, "A New Method of Interpolation and Smooth Curve Fitting
                Based on Local Procedures", JACM, v. 17-4, p.589-602, 1970.

            s(i) = (|d(i+1)-d(i)| * d(i-1) + |d(i-1)-d(i-2)| * d(i))
                / (|d(i+1)-d(i)|          + |d(i-1)-d(i-2)|)
        */

        Vector weights(delta_new.size() - 1);
        for (auto i = 0; i < weights.size(); ++i)
        {
            weights[i] = std::abs(delta_new[i+1] - delta_new[i]) + std::abs(delta_new[i] + delta_new[i+1])/2.0;
            //weights(i) = std::abs(delta_new(i+1) - delta_new(i));
        }

        Vector s(n);

        for (auto i = 0; i < n; ++i)
        {
            T weights1 = weights[i];   // |d(i-1)-d(i-2)|
            T weights2 = weights[i+2]; // |d(i+1)-d(i)|
            T delta1 = delta_new[i+1];     // d(i-1)
            T delta2 = delta_new[i+2];     // d(i)
            T weights12 = weights1 + weights2;
            if (weights12 == 0.0)
            {
                // To avoid 0/0, Akima proposed to average the divided differences d(i-1)
                // and d(i) for the edge case of d(i-2) = d(i-1) and d(i) = d(i+1):
                s[i] = 0.5*(delta1 + delta2);
            } else {
                s[i] = (weights2*delta1 + weights1*delta2)/weights12;
            }
        }
        return s;
    }
};


template <typename T>
class NaturalSpline1D : public BaseSpline<T>
{
    using Vector = std::vector<T>;
public:
    NaturalSpline1D(const Vector &x, const Vector &y)
    : BaseSpline<T>(x, y)

    {
        this->build();
    }
    ~NaturalSpline1D() { }

    const Vector calc_slopes(const Vector &x, const Vector &y) const override
    {
        assert(x.size() == y.size());
        auto N = x.size();

        // vectors that fill up the tridiagonal matrix
        Vector a(N); // first value of a is not used
        Vector b(N);
        Vector c(N); // last value of c is not used
        // right hand side
        Vector d(N);

        // first row, 0
        double dx1 = x[1] - x[0];
        b[0] = 2.0/dx1;
        c[0] = 1.0/dx1;
        d[0] = 3.0*(y[1] - y[0])/(dx1*dx1);

        // rows i = 1, ..., N-2
        for (auto i = 1; i < N-1; ++i)
        {
            double dxi = x[i] - x[i-1];
            double dxi1 = x[i+1] - x[i];
            a[i] = 1.0/dxi;
            b[i] = 2.0*(1.0/dxi + 1.0/dxi1);
            c[i] = 1.0/dxi1;
            d[i] = 3.0*((y[i] - y[i-1])/(dxi*dxi) + (y[i+1] - y[i])/(dxi1*dxi1));
        }

        // last row, N-1
        double dxN = x[N-1] - x[N-2];
        a[N-1] = 1.0/dxN;
        b[N-1] = 2.0/dxN;
        d[N-1] = 3.0*(y[N-1] - y[N-2])/(dxN*dxN);

        thomas_algorithm(a, b, c, d);

        return d;
    }

private:

    void thomas_algorithm(const Vector& a, const Vector& b, Vector& c, Vector& d) const
    {
        auto N = d.size();

        c[0] /= b[0];
        d[0] /= b[0];

        N--;
        for (auto i = 1; i < N; i++) {
            c[i] /= b[i] - a[i]*c[i-1];
            d[i] = (d[i] - a[i]*d[i-1]) / (b[i] - a[i]*c[i-1]);
        }

        d[N] = (d[N] - a[N]*d[N-1]) / (b[N] - a[N]*c[N-1]);

        for (auto i = N; i-- > 0;) {
            d[i] -= c[i]*d[i+1];
        }
    }

};


} // namespace cip
