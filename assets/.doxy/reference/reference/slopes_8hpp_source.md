

# File slopes.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**slopes.hpp**](slopes_8hpp.md)

[Go to the documentation of this file](slopes_8hpp.md)


```C++
#pragma once

#include <cmath>
#include <vector>


namespace cip {


template <typename T, typename Tx, typename Tf>
std::vector<T> monotonic_slopes(const Tx x, const Tf f)
{

    // See https://en.wikipedia.org/wiki/Monotone_cubic_interpolation
    const auto nx = x.size();

    std::vector<T> secants(nx-1);
    std::vector<T> tangents(nx);

    for (auto k = 0; k < nx-1; ++k)
    {
        //secants[k] = (*(f_begin+k+1) - *(f_begin+k)) / (*(x_begin+k+1) - *(x_begin+k));
        secants[k] = (f[k+1] - f[k]) / (x[k+1] - x[k]);
    }

    tangents[0] = secants[0];
    for (auto k = 1; k < nx-1; ++k)
    {
        tangents[k] = 0.5*(secants[k-1] + secants[k]);
    }
    tangents[nx-1] = secants[nx-2];

    for (auto k = 0; k < nx-1; ++k)
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


template <typename T, typename Tx, typename Tf>
std::vector<T> akima_slopes(const Tx x, const Tf f)
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

    const auto nx = x.size();

    std::vector<T> delta(nx-1);
    for (auto i = 0; i < delta.size(); ++i)
    {
        delta[i] = (f[i+1] - f[i])/(x[i+1] - x[i]);
    }

    T delta_0 = 2.0*delta[0] - delta[1];
    T delta_m1 = 2.0*delta_0 - delta[0];
    T delta_n  = 2.0*delta[nx-2] - delta[nx-3];
    T delta_n1 = 2.0*delta_n - delta[nx-2];

    std::vector<T> delta_new(delta.size() + 4);
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

    std::vector<T> weights(delta_new.size() - 1);
    for (auto i = 0; i < weights.size(); ++i)
    {
        weights[i] = std::abs(delta_new[i+1] - delta_new[i]) + std::abs(delta_new[i] + delta_new[i+1])/2.0;
    }

    std::vector<T> s(nx);

    for (auto i = 0; i < nx; ++i)
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


template <typename T>
void thomas_algorithm(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &c, std::vector<T> &d)
{
    // See https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm#Method
    
    auto nd = d.size();

    c[0] /= b[0];
    d[0] /= b[0];

    nd--;
    for (auto i = 1; i < nd; i++) {
        c[i] /= b[i] - a[i]*c[i-1];
        d[i] = (d[i] - a[i]*d[i-1]) / (b[i] - a[i]*c[i-1]);
    }

    d[nd] = (d[nd] - a[nd]*d[nd-1]) / (b[nd] - a[nd]*c[nd-1]);

    for (auto i = nd; i-- > 0;) {
        d[i] -= c[i]*d[i+1];
    }
}


enum class BoundaryConditionType {
    Natural,
    Clamped,
    NotAKnot,
    Periodic
};


template <BoundaryConditionType BC, typename T, typename Tx, typename Tf>
struct setNaturalSplineBoundaryCondition;


template <typename T, typename Tx, typename Tf>
struct setNaturalSplineBoundaryCondition<BoundaryConditionType::Natural, T, Tx, Tf> {
    using Vector = std::vector<T>;
    constexpr void operator()(const Tx& x, const Tf& f, Vector& a, Vector& b, Vector& c, Vector& d) const {
        T dx1 = x[1] - x[0];
        b[0] = 2.0/dx1;
        c[0] = 1.0/dx1;
        d[0] = 3.0*(f[1] - f[0])/(dx1*dx1);

        const auto nx = x.size();
        T dxN = x[nx-1] - x[nx-2];
        a[nx-1] = 1.0/dxN;
        b[nx-1] = 2.0/dxN;
        d[nx-1] = 3.0*(f[nx-1] - f[nx-2])/(dxN*dxN);

        thomas_algorithm<T>(a, b, c, d);
    }
};


template <typename T, typename Tx, typename Tf>
struct setNaturalSplineBoundaryCondition<BoundaryConditionType::NotAKnot, T, Tx, Tf> {
    using Vector = std::vector<T>;
    constexpr void operator()(const Tx& x, const Tf& f, Vector& a, Vector& b, Vector& c, Vector& d) const {
        T dx1 = x[1] - x[0];
        T dx2 = x[2] - x[1];
        b[0] = 1.0/(dx1*dx1);
        c[0] = b[0] - 1.0/(dx2*dx2);
        d[0] = 2.0*((f[1] - f[0])/(dx1*dx1*dx1) - (f[2] - f[1])/(dx2*dx2*dx2));

        // necessary conversion to maintain a tridiagonal matrix
        b[0] += a[1]/dx2;
        c[0] += b[1]/dx2;
        d[0] += d[1]/dx2;

        const auto nx = x.size();
        T dxN1 = x[nx-1] - x[nx-2];
        T dxN2 = x[nx-2] - x[nx-3];
        a[nx-1] = 1.0/(dxN1*dxN1) - 1.0/(dxN2*dxN2);
        b[nx-1] = -1.0/(dxN2*dxN2);
        d[nx-1] = 2.0*((f[nx-2] - f[nx-3])/(dxN2*dxN2*dxN2) - (f[nx-1] - f[nx-2])/(dxN1*dxN1*dxN1));

        // necessary conversion to maintain a tridiagonal matrix
        a[nx-1] -= b[nx-2]/dxN2;
        b[nx-1] -= c[nx-2]/dxN2;
        d[nx-1] -= d[nx-2]/dxN2;

        thomas_algorithm<T>(a, b, c, d);
    }
};


template <typename T, typename Tx, typename Tf>
struct setNaturalSplineBoundaryCondition<BoundaryConditionType::Clamped, T, Tx, Tf> {
    using Vector = std::vector<T>;
    constexpr void operator()(const Tx& x, const Tf& f, Vector& a, Vector& b, Vector& c, Vector& d) const {
        // Demand the slopes to be zero at the boundaries
        b[0] = T{1};
        const auto nx = x.size();
        b[nx-1] = T{1};

        thomas_algorithm<T>(a, b, c, d);
    }
};


template <typename T>
void cyclic_thomas_algorithm(const std::vector<T> &a, const std::vector<T> &b, const std::vector<T> &c, std::vector<T> &d, const std::size_t exclude_rows=0)
{
    // See https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm#Variants
    
    const auto nx = d.size() - exclude_rows; // last exclude_rows rows are not used
    
    std::vector<T> cmod(nx);
    std::vector<T> v(nx);
    
    cmod[1] = c[1]/b[1];
    v[1] = -a[1]/b[1];
    d[1] = d[1]/b[1];

    for (auto ix = 2; ix < nx-1; ++ix)
    {
        const T m = T(1.0)/(b[ix] - a[ix]*cmod[ix-1]);
        cmod[ix] = m*c[ix];
        v[ix] = (T(0.0) - a[ix]*v[ix-1]) * m;
        d[ix] = (d[ix] - a[ix]*d[ix-1]) * m;
    }

    const T m = T(1.0)/(b[nx-1] - a[nx-1]*cmod[nx-2]);
    cmod[nx-1] = m*c[nx-1];
    v[nx-1] = m*(-c[0] - a[nx-1]*v[nx-2]);
    d[nx-1] = m*(d[nx-1] - a[nx-1]*d[nx-2]);

    for (auto ix = nx - 2; ix >= 1; --ix)
    {
        v[ix] -= cmod[ix]*v[ix+1];
        d[ix] -= cmod[ix]*d[ix+1];
    }

    d[0] = (d[0] - a[0]*d[nx-1] - c[0]*d[1])/(b[0] + a[0]*v[nx-1] + c[0]*v[1]);

    for (auto ix = 1; ix < nx; ++ix)
    {
        d[ix] += d[0]*v[ix];
    }
}


template <typename T, typename Tx, typename Tf>
struct setNaturalSplineBoundaryCondition<BoundaryConditionType::Periodic, T, Tx, Tf> {
    using Vector = std::vector<T>;
    constexpr void operator()(const Tx& x, const Tf& f, Vector& a, Vector& b, Vector& c, Vector& d) const {

        // Periodic boundary conditions, it is assumed that f[0] == f[nx-1] !

        // last equation is for periodicity and not used in cyclic thomas algorithm
        const auto nsys = d.size() - 1;

        // Row 0
        {
            T dxL = x.back() - x[nsys-1];
            T dxR = x[1] - x[0];

            a[0] = 1.0/dxL;   // lower-left corner
            b[0] = 2.0*(1.0/dxL + 1.0/dxR);
            c[0] = 1.0/dxR;
            d[0] = 3.0*((f[0] - f[nsys-1])/(dxL*dxL) + (f[1] - f[0])/(dxR*dxR));
        }

        // Last row (nsys-1)
        {
            T dxL = x[nsys-1] - x[nsys-2];
            T dxR = x.back() - x[nsys-1];

            a[nsys-1] = 1.0/dxL;
            b[nsys-1] = 2.0*(1.0/dxL + 1.0/dxR);
            c[nsys-1] = 1.0/dxR;  // upper-right corner
            d[nsys-1] = 3.0*((f[nsys-1] - f[nsys-2])/(dxL*dxL) + (f[0] - f[nsys-1])/(dxR*dxR));
        }

        cyclic_thomas_algorithm<T>(a, b, c, d, 1); // exclude last row

        d.back() = d.front(); // duplicate endpoint slope for periodic BC
    }
};


template <typename T, BoundaryConditionType BC=BoundaryConditionType::Natural, typename Tx, typename Tf>
std::vector<T> natural_spline_slopes(const Tx x, const Tf f)
{
    // https://en.wikipedia.org/wiki/Spline_interpolation

    const auto nx = x.size();

    // vectors that fill up the tridiagonal matrix
    std::vector<T> a(nx, T{0}); // first value of a is only used for periodic BC
    std::vector<T> b(nx, T{0});
    std::vector<T> c(nx, T{0}); // last value of c is only used for periodic BC
    // right hand side
    std::vector<T> d(nx, T{0});

    // For Periodic BD the last row is not used
    constexpr bool is_periodic = (BC == BoundaryConditionType::Periodic);

    for (auto i = 1; i < (is_periodic ? nx-2 : nx-1); ++i)
    {
        T dxL = x[i] - x[i-1];
        T dxR = x[i+1] - x[i];
        a[i] = 1.0/dxL;
        b[i] = 2.0*(1.0/dxL + 1.0/dxR);
        c[i] = 1.0/dxR;
        d[i] = 3.0*((f[i] - f[i-1])/(dxL*dxL) + (f[i+1] - f[i])/(dxR*dxR));
    }

    setNaturalSplineBoundaryCondition<BC, T, Tx, Tf>{}(x, f, a, b, c, d);

    return d;
}


} // namespace cip
```


