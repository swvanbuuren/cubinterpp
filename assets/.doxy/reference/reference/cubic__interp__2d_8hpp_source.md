

# File cubic\_interp\_2d.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**cubic\_interp\_2d.hpp**](cubic__interp__2d_8hpp.md)

[Go to the documentation of this file](cubic__interp__2d_8hpp.md)


```C++
#pragma once

#include "cubic_interp.hpp"
#include "slopes.hpp"


namespace cip {


template <typename T, std::size_t N=2>
class MonotonicCubicInterp2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
MonotonicCubicInterp2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~MonotonicCubicInterp2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return monotonic_slopes<T>(x, f);
    }
};


template <typename T, std::size_t N=2>
class MakimaCubicInterp2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    MakimaCubicInterp2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~MakimaCubicInterp2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return makima_slopes<T>(x, f);
    }
};



template <typename T, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Natural, std::size_t N=2>
class NaturalCubicInterp2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    NaturalCubicInterp2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~NaturalCubicInterp2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return natural_spline_slopes<T, BC>(x, f);
    }
};


template <typename T, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Periodic, std::size_t N=2>
class NaturalPeriodicCubicInterp2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    NaturalPeriodicCubicInterp2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~NaturalPeriodicCubicInterp2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return natural_spline_slopes<T, BC>(x, f);
    }
};


} // namespace cip
```


