#pragma once

#include "linear_interp.hpp"
#include <cstddef>


namespace cip {


template <typename T, std::size_t N, IndexMethod IM = IndexMethod::BinarySearch>
class LinearInterp : public LinearInterpND<T, N, IM>
{
    using Vector  = std::vector<T>;
    using VectorN = cip::VectorN<T, N>;
public:
    // 1-D constructor: (x, f)
    LinearInterp(const Vector& x, const Vector& f)
        requires (N == 1)
        : LinearInterpND<T, 1, IM>(f, x)
    {}

    // 2-D constructor: (x, y, f)
    LinearInterp(const Vector& x0, const Vector& x1, const VectorN& f)
        requires (N == 2)
        : LinearInterpND<T, 2, IM>(f, x0, x1)
    {}

    // 3-D constructor: (x, y, z, f)
    LinearInterp(const Vector& x0, const Vector& x1, const Vector& x2, const VectorN& f)
        requires (N == 3)
        : LinearInterpND<T, 3, IM>(f, x0, x1, x2)
    {}

    // 4-D constructor: (x, y, z, w, f)
    LinearInterp(const Vector& x0, const Vector& x1, const Vector& x2, const Vector& x3, const VectorN& f)
        requires (N == 4)
        : LinearInterpND<T, 4, IM>(f, x0, x1, x2, x3)
    {}

    ~LinearInterp() = default;
};


// ===== Backward-Compatible Type Aliases =====

template <typename T, IndexMethod IM = IndexMethod::BinarySearch>
using LinearInterp1D = LinearInterp<T, 1, IM>;

template <typename T, IndexMethod IM = IndexMethod::BinarySearch>
using LinearInterp2D = LinearInterp<T, 2, IM>;

template <typename T, IndexMethod IM = IndexMethod::BinarySearch>
using LinearInterp3D = LinearInterp<T, 3, IM>;

template <typename T, IndexMethod IM = IndexMethod::BinarySearch>
using LinearInterp4D = LinearInterp<T, 4, IM>;


} // namespace cip
