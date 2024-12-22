#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <vector>


namespace cip {


constexpr inline int factorial(int n)
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}


constexpr inline size_t binomial(size_t n, size_t k) noexcept
{
    return
      (        k> n  )? 0 :          // out of range
      (k==0 || k==n  )? 1 :          // edge
      (k==1 || k==n-1)? n :          // first
      (     k+k < n  )?              // recursive:
      (binomial(n-1,k-1) * n)/k :    //  path to k=1   is faster
      (binomial(n-1,k) * n)/(n-k);   //  path to k=n-1 is faster
}


template<typename T>
class CubicCell1D
{
    using Array = std::array<T, 4>;
public:
    explicit CubicCell1D(T x0, T x1, T f0, T f1, T df0, T df1)
      : a(calculate_coefficients(x0, x1-x0, f0, f1, df0, df1))
    {
    }
    ~CubicCell1D() = default;

    const T eval(const T x) const
    {
        return a[0] + (a[1] + (a[2] + a[3]*x)*x)*x;
    }

private:
    const Array alpha_00 {1.0, 0.0, -3.0, +2.0};
    const Array alpha_01 {0.0, 1.0, -2.0, +1.0};
    const Array alpha_10 {0.0, 0.0, +3.0, -2.0};
    const Array alpha_11 {0.0, 0.0, -1.0, +1.0};
    const size_t alpha_size {4};
    const Array a;

    T binomial_power_coefficient(const T y, const int n, const int k) const
    {
        return binomial(n, k)*std::pow(y, n-k);
    }

    void scale_coefficients(std::array<T, 4> &a, const T x0, const T h) const
    {
        using Array = std::array<T, 4>;
        Array dummy {0.0, 0.0, 0.0, 0.0};

        auto i = 0;
        T h_power_i {1.0};
        for (auto &a_i : a)
        {
            auto j = 0;
            for (auto &dummy_j : dummy)
            {
                dummy_j += binomial_power_coefficient(-x0, i, j++)*a_i/h_power_i;
            }
            h_power_i *= h;
            ++i;
        }

        a.swap(dummy);
    }

    /** Calculates the coefficients for piecewise cubic interpolation cell
        \param x0 x offset
        \param h x scaling factor
        \param f0 value at node 0 (left)
        \param f1 value at node 1 (right)
        \param df0 derivative at node 0 (left)
        \param df1 derivative at node 1 (right
    */
    const Array calculate_coefficients(const T x0, const T h, const T f0, const T f1, const T df0, const T df1) const
    {
        Array coefficients {0.0, 0.0, 0.0, 0.0};
        for (size_t i = 0; i < alpha_size; ++i)
        {
            // note the scaling of df0 and df1, which arises due to differentiation with
            // respect to x (which is scaled by h)
            coefficients[i] += f0*alpha_00[i] + f1*alpha_10[i] + df0*h*alpha_01[i] + df1*h*alpha_11[i];
        }
        scale_coefficients(coefficients, x0, h);
        return coefficients;
    }
};


template <typename T>
class BaseSpline
{
private:
    const T x;
    const size_t index_front = 0;
    const size_t index_back;
    const double x_front;
    const double x_back;
    const double x_delta;
    std::vector<CubicCell1D<double>> splines;
    const size_t cell_index(const double xi) const
    {
        return
        (xi < x_back) ?
            ((xi < x_front) ?
                index_front :
                (size_t)((xi-x_front)/x_delta)) :
            index_back;
    }
    const size_t sort_index(const double xi) const
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
    const size_t get_index(const double xi) const
    {
        return sort_index(xi);
    }

public:
    BaseSpline(const T &_x, const T &_y)
      : x(_x),
        index_back(x.size()-2),
        x_front(x[index_front]),
        x_back(x[x.size()-1]),
        x_delta((x_back-x_front)/(x.size()-1))
    {
        assert(_x.size() == _y.size());
    }
    virtual ~BaseSpline() { }
    virtual const T calc_slopes(const T &x, const T &y) const = 0;
    void build(const T &x, const T &y)
    {
        const T slopes = calc_slopes(x, y);
        splines.reserve(x.size()-1);
        for (int i = 0; i < x.size()-1; ++i)
        {
            splines.push_back(CubicCell1D<double>(x[i], x[i+1], y[i], y[i+1], slopes[i], slopes[i+1]));
        }
    }
    double eval(const double xi) const
    {
        return splines[get_index(xi)].eval(xi);
    };
    T evaln(const T &xi) const
    {
        auto xi_iter = xi.begin();
        T yi(xi.size());
        for (auto &yi_i : yi)
        {
            yi_i = eval(*xi_iter++);
        }
        return yi;
    }
};


template <typename T>
class MonotonicSpline1D : public BaseSpline<T>
{
public:
    MonotonicSpline1D(const T &x, const T &y)
      : BaseSpline<T>(x, y)

    {
        this->build(x, y);
    }
    ~MonotonicSpline1D() { }
    const T calc_slopes(const T &x, const T &y) const override
    {
        // See https://en.wikipedia.org/wiki/Monotone_cubic_interpolation
        auto N = x.size();

        T secants(N-1);
        T tangents(N);

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
                double alpha = tangents[k] / secants[k];
                double beta = tangents[k + 1] / secants[k];
                double h = std::hypot(alpha, beta);
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
public:
    AkimaSpline1D(const T &x, const T &y)
      : BaseSpline<T>(x, y)

    {
        this->build(x, y);
    }
    ~AkimaSpline1D() { }
    const T calc_slopes(const T &x, const T &y) const override
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

        T delta(x.size()-1);
        for (auto i = 0; i < delta.size(); ++i)
        {
            delta[i] = (y[i+1] - y[i])/(x[i+1] - x[i]);
        }

        auto n = x.size();
        double delta_0 = 2.0*delta[0] - delta[1];
        double delta_m1 = 2.0*delta_0 - delta[0];
        double delta_n  = 2.0*delta[n-2] - delta[n-3];
        double delta_n1 = 2.0*delta_n - delta[n-2];

        T delta_new(delta.size() + 4);
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

        T weights(delta_new.size() - 1);
        for (auto i = 0; i < weights.size(); ++i)
        {
            weights[i] = std::abs(delta_new[i+1] - delta_new[i]) + std::abs(delta_new[i] + delta_new[i+1])/2.0;
            //weights(i) = std::abs(delta_new(i+1) - delta_new(i));
        }

        T s(n);

        for (auto i = 0; i < n; ++i)
        {
            double weights1 = weights[i];   // |d(i-1)-d(i-2)|
            double weights2 = weights[i+2]; // |d(i+1)-d(i)|
            double delta1 = delta_new[i+1];     // d(i-1)
            double delta2 = delta_new[i+2];     // d(i)
            double weights12 = weights1 + weights2;
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
private:
    void thomas_algorithm(const T& a, const T& b, T& c, T& d) const
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
public:
    NaturalSpline1D(const T &x, const T &y)
      : BaseSpline<T>(x, y)

    {
        this->build(x, y);
    }
    ~NaturalSpline1D() { }
    const T calc_slopes(const T &x, const T &y) const override
    {
        assert(x.size() == y.size());
        auto N = x.size();

        // vectors that fill up the tridiagonal matrix
        T a(N); // first value of a is not used
        T b(N);
        T c(N); // last value of c is not used
        // right hand side
        T d(N);

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
};


} // namespace cip
