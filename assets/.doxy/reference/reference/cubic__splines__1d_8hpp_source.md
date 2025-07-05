

# File cubic\_splines\_1d.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**cubic\_splines\_1d.hpp**](cubic__splines__1d_8hpp.md)

[Go to the documentation of this file](cubic__splines__1d_8hpp.md)


```C++
#pragma once

#include "cubic_spline.hpp"
#include "slopes.hpp"
#include <cstddef>


namespace cip {


template <typename T, std::size_t N=1>
class MonotonicSpline1D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
public:
    MonotonicSpline1D(const Vector &x, const Vector &f)
    : CubicInterpND<T, N>(x, f)
    {
        this->build(f);
    }

    ~MonotonicSpline1D() {}

    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return monotonic_slopes<T>(x, f);
    }

};


template <typename T, std::size_t N=1>
class AkimaSpline1D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
public:
    AkimaSpline1D(const Vector &x, const Vector &f)
    : CubicInterpND<T, N>(x, f)
    {
        this->build(f);
    }

    ~AkimaSpline1D() {}
    
    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return akima_slopes<T>(x, f);
    }
    
};


template <typename T, std::size_t N=1, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Natural>
class NaturalSpline1D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
public:
    NaturalSpline1D(const Vector &x, const Vector &f)
    : CubicInterpND<T, N>(x, f)
    {
        this->build(f);
    }
    
    ~NaturalSpline1D() {}

    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return natural_spline_slopes<T, BC>(x, f);
    }

};


} // namespace cip
```


