#pragma once

#include "cubic_spline.hpp"
#include "slopes.hpp"


namespace cip {


template <typename T, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Natural, std::size_t N=3>
class NaturalSpline3D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN = cip::VectorN<T, N>;
public:
    NaturalSpline3D(const Vector &_x, const Vector &_y, const Vector &_z, const VectorN &_f)
    : CubicInterpND<T, N>(_f, _x, _y, _z)
    {
        this->build(_f, _x, _y, _z);
    }
    ~NaturalSpline3D() {}

    Vector calc_slopes(const Vector &x, const Mdspan1D &f) const override
    {
        return natural_spline_slopes<T, BC>(x, f);
    }
};


} // namespace cip
