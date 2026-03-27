#pragma once

#include "cubic_interp.hpp"
#include "slopes.hpp"
#include <cstddef>


namespace cip {


template <typename T, std::size_t N=1>
class MonotonicCubicInterp1D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
public:
    MonotonicCubicInterp1D(const Vector &x, const Vector &f)
    : CubicInterpND<T, N>(x, f)
    {
        this->build(f);
    }

    ~MonotonicCubicInterp1D() {}

    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return monotonic_slopes<T>(x, f);
    }

};


template <typename T, std::size_t N=1>
class MakimaCubicInterp1D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
public:
    MakimaCubicInterp1D(const Vector &x, const Vector &f)
    : CubicInterpND<T, N>(x, f)
    {
        this->build(f);
    }

    ~MakimaCubicInterp1D() {}
    
    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return makima_slopes<T>(x, f);
    }
    
};


template <typename T, std::size_t N=1, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Natural>
class NaturalCubicInterp1D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
public:
    NaturalCubicInterp1D(const Vector &x, const Vector &f)
    : CubicInterpND<T, N>(x, f)
    {
        this->build(f);
    }
    
    ~NaturalCubicInterp1D() {}

    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return natural_spline_slopes<T, BC>(x, f);
    }

};


template <typename T, std::size_t N=1, cip::BoundaryConditionType BC=cip::BoundaryConditionType::Periodic>
class NaturalPeriodicCubicInterp1D : public CubicInterpND<T, N>
{
    using Vector = std::vector<T>;
public:
    NaturalPeriodicCubicInterp1D(const Vector &x, const Vector &f)
    : CubicInterpND<T, N>(x, f)
    {
        this->build(f);
    }
    
    ~NaturalPeriodicCubicInterp1D() {}

    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return natural_spline_slopes<T, BC>(x, f);
    }

};


} // namespace cip
