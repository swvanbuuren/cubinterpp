#pragma once

#include "cubic_spline.hpp"
#include "slopes.hpp"


namespace cip {


template <typename T>
class MonotonicSpline1D : public CubicInterp1D<T>
{
    using Vector = std::vector<T>;
public:
    MonotonicSpline1D(const Vector &x, const Vector &f)
    : CubicInterp1D<T>(x, f)
    {
        this->build(f);
    }

    ~MonotonicSpline1D() {}

    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return monotonic_slopes<T>(x, f);
    }

};


template <typename T>
class AkimaSpline1D : public CubicInterp1D<T>
{
    using Vector = std::vector<T>;
public:
    AkimaSpline1D(const Vector &x, const Vector &f)
    : CubicInterp1D<T>(x, f)
    {
        this->build(f);
    }

    ~AkimaSpline1D() {}
    
    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return akima_slopes<T>(x, f);
    }
    
};


template <typename T>
class NaturalSpline1D : public CubicInterp1D<T>
{
    using Vector = std::vector<T>;
public:
    NaturalSpline1D(const Vector &x, const Vector &f)
    : CubicInterp1D<T>(x, f)
    {
        this->build(f);
    }
    
    ~NaturalSpline1D() {}

    Vector calc_slopes(const Vector &x, const Vector &f) const override {
        return natural_spline_slopes<T>(x, f);
    }

};


} // namespace cip
