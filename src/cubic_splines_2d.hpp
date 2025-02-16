#pragma once

#include "cubic_spline.hpp"
#include "slopes.hpp"


namespace cip {


template <typename T, std::size_t N=2>
class MonotonicSpline2D : public CubicInterp2D<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
MonotonicSpline2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterp2D<T>(_x, _y, _f)
    {
        this->build(_f);
    }
    ~MonotonicSpline2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return monotonic_slopes<T>(x, f);
    }
};


template <typename T, std::size_t N=2>
class AkimaSpline2D : public CubicInterp2D<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    AkimaSpline2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterp2D<T>(_x, _y, _f)
    {
        this->build(_f);
    }
    ~AkimaSpline2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return akima_slopes<T>(x, f);
    }
};



template <typename T, std::size_t N=2>
class NaturalSpline2D : public CubicInterp2D<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    NaturalSpline2D(const Vector &_x, const Vector &_y, const VectorN &_f)
    : CubicInterp2D<T>(_x, _y, _f)
    {
        this->build(_f);
    }
    ~NaturalSpline2D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return natural_spline_slopes<T>(x, f);
    }
};


} // namespace cip
