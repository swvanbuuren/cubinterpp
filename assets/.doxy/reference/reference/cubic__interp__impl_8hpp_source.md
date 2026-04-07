

# File cubic\_interp\_impl.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**cubic\_interp\_impl.hpp**](cubic__interp__impl_8hpp.md)

[Go to the documentation of this file](cubic__interp__impl_8hpp.md)


```C++
#pragma once

#include "cubic_interp.hpp"
#include "slopes.hpp"
#include <cstddef>


namespace cip {


template <typename T, typename SlopePolicy, std::size_t N, IndexMethod IM = IndexMethod::BinarySearch>
class CubicInterp : public CubicInterpND<T, N, IM>
{
    using Vector   = std::vector<T>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
    using VectorN  = cip::VectorN<T, N>;
public:
    // 1-D constructor: (x, f)
    CubicInterp(const Vector& x, const Vector& f)
        requires (N == 1)
        : CubicInterpND<T, 1, IM>(f, x)  // general base: _f unused in ctor; _xi = x
    {
        this->build(f, x);
    }

    // 2-D constructor: (x, y, f)
    CubicInterp(const Vector& x0, const Vector& x1, const VectorN& f)
        requires (N == 2)
        : CubicInterpND<T, N, IM>(f, x0, x1)
    {
        this->build(f, x0, x1);
    }

    // 3-D constructor: (x, y, z, f)
    CubicInterp(const Vector& x0, const Vector& x1, const Vector& x2, const VectorN& f)
        requires (N == 3)
        : CubicInterpND<T, N, IM>(f, x0, x1, x2)
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
template <typename T, IndexMethod IM = IndexMethod::BinarySearch>
using MonotonicCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Monotonic>, 1, IM>;

template <typename T, IndexMethod IM = IndexMethod::BinarySearch>
using MakimaCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Makima>, 1, IM>;

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Natural, IndexMethod IM = IndexMethod::BinarySearch>
using NaturalCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, 1, IM>;

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Periodic, IndexMethod IM = IndexMethod::BinarySearch>
using NaturalPeriodicCubicInterp1D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, 1, IM>;

// --- 2-D ---

template <typename T, std::size_t N = 2, IndexMethod IM = IndexMethod::BinarySearch>
using MonotonicCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Monotonic>, N, IM>;

template <typename T, std::size_t N = 2, IndexMethod IM = IndexMethod::BinarySearch>
using MakimaCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Makima>, N, IM>;

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Natural, std::size_t N = 2, IndexMethod IM = IndexMethod::BinarySearch>
using NaturalCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, N, IM>;

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Periodic, std::size_t N = 2, IndexMethod IM = IndexMethod::BinarySearch>
using NaturalPeriodicCubicInterp2D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, N, IM>;

// --- 3-D ---

template <typename T, BoundaryConditionType BC = BoundaryConditionType::Natural, std::size_t N = 3, IndexMethod IM = IndexMethod::BinarySearch>
using NaturalCubicInterp3D = CubicInterp<T, SlopePolicy<SlopeMethod::Natural, BC>, N, IM>;


} // namespace cip
```


