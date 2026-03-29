#pragma once

#include "cubic_interp.hpp"
#include "slopes.hpp"
#include <cstddef>


namespace cip {


enum class SlopeMethod { Monotonic, Makima, Natural };

template <SlopeMethod Method, BoundaryConditionType BC = BoundaryConditionType::Natural>
struct SlopePolicy {
    template <typename T, typename Tx, typename Tf>
    static std::vector<T> calc(const Tx& x, const Tf& f) {
        if constexpr (Method == SlopeMethod::Monotonic) {
            return monotonic_slopes<T>(x, f);
        } else if constexpr (Method == SlopeMethod::Makima) {
            return makima_slopes<T>(x, f);
        } else if constexpr (Method == SlopeMethod::Natural) {
            return natural_spline_slopes<T, BC>(x, f);
        } else {
            static_assert(sizeof(T) == 0,
                "Unhandled SlopeMethod enumerator in SlopePolicy::calc — "
                "add a corresponding branch.");
        }
    }
};


template <typename T, typename SlopePolicy, std::size_t N>
class CubicInterp : public CubicInterpND<T, N>
{
    using Vector   = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN  = cip::VectorN<T, N>;
public:
    // 1-D constructor: (x, f)
    CubicInterp(const Vector& x, const Vector& f)
        requires (N == 1)
        : CubicInterpND<T, 1>(x, f)
    {
        this->build(f);
    }

    // 2-D constructor: (x, y, f)
    CubicInterp(const Vector& x0, const Vector& x1, const VectorN& f)
        requires (N == 2)
        : CubicInterpND<T, N>(f, x0, x1)
    {
        this->build(f, x0, x1);
    }

    // 3-D constructor: (x, y, z, f)
    CubicInterp(const Vector& x0, const Vector& x1, const Vector& x2, const VectorN& f)
        requires (N == 3)
        : CubicInterpND<T, N>(f, x0, x1, x2)
    {
        this->build(f, x0, x1, x2);
    }

    ~CubicInterp() = default;

    Vector calc_slopes(const Vector& x, const Mdspan1D& f) const override {
        return SlopePolicy::template calc<T>(x, f);
    }
};


// ===== Backward-Compatible Type Aliases =====

// --- 1-D ---
template <typename T, std::size_t /*N*/ = 1>
using MonotonicCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Monotonic>, 1>;

template <typename T, std::size_t /*N*/ = 1>
using MakimaCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Makima>, 1>;

template <typename T, std::size_t /*N*/ = 1, BoundaryConditionType BC = BoundaryConditionType::Natural>
using NaturalCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, 1>;

template <typename T, std::size_t /*N*/ = 1, BoundaryConditionType BC = BoundaryConditionType::Periodic>
using NaturalPeriodicCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, 1>;

// --- 2-D ---

template <typename T, std::size_t N = 2>
using MonotonicCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Monotonic>, N>;

template <typename T, std::size_t N = 2>
using MakimaCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Makima>, N>;

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Natural, std::size_t N = 2>
using NaturalCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, N>;

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Periodic, std::size_t N = 2>
using NaturalPeriodicCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, N>;

// --- 3-D ---

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Natural, std::size_t N = 3>
using NaturalCubicInterp3D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, N>;


} // namespace cip
