#pragma once

#include "cubic_spline.hpp"
#include "slopes.hpp"


namespace cip {


template <typename T, std::size_t N=2>
class MonotonicSpline2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
MonotonicSpline2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~MonotonicSpline2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return monotonic_slopes<T>(x, f);
    }
};


template <typename T, std::size_t N=2>
class AkimaSpline2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    AkimaSpline2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~AkimaSpline2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return akima_slopes<T>(x, f);
    }
};



template <typename T, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Natural, std::size_t N=2>
class NaturalSpline2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    NaturalSpline2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~NaturalSpline2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return natural_spline_slopes<T, BC>(x, f);
    }
};


template <typename T, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Periodic, std::size_t N=2>
class NaturalPeriodicSpline2D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    NaturalPeriodicSpline2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y)
    {
        this->build(_f, _x, _y);
    }
    ~NaturalPeriodicSpline2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return natural_spline_slopes<T, BC>(x, f);
    }
};


} // namespace cip
