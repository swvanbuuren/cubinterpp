// Unit tests for cip::CubicInterpND<T, N> (accessed through cip::CubicInterp).
//
// CubicInterpND is the full pipeline:
//   grid knots → slope computation → mixed-derivative filling → cell assembly → eval
//
// Theory reference: docs/theory/cubic.md

#include <gtest/gtest.h>
#include <cubinterpp.hpp>
#include "assertion_helpers.hpp"

static constexpr double TOL = 1e-10;

// Concrete interpolator type aliases used throughout these tests.
using Monotonic1D = cip::MonotonicCubicInterp1D<double>;
using Monotonic2D = cip::MonotonicCubicInterp2D<double>;
using Monotonic3D = cip::MonotonicCubicInterp2D<double, 3>;  // MonotonicCubicInterp2D<T,N> is the N-D alias; N=3 here
using NaturalBC1D    = cip::NaturalCubicInterp1D<double, cip::BoundaryConditionType::Natural>;
using NotAKnot1D     = cip::NaturalCubicInterp1D<double, cip::BoundaryConditionType::NotAKnot>;
using Clamped1D      = cip::NaturalCubicInterp1D<double, cip::BoundaryConditionType::Clamped>;

// ─── CubicInterpND 1D ────────────────────────────────────────────────────────

// eval at every knot must exactly reproduce the prescribed f values.
// This is the fundamental interpolation property and must hold for all slope methods.
TEST(CubicInterpND_1D, KnotsReturnExactValues)
{
    cip::Vector x = {0.0, 1.0, 2.0, 3.0};
    cip::Vector f = {0.0, 1.0, 4.0, 9.0};   // f = x²
    Monotonic1D interp(x, f);

    for (std::size_t i = 0; i < x.size(); ++i)
        EXPECT_NEAR(interp.eval(x[i]), f[i], TOL) << "knot index " << i;
}

// evaln(xs) must produce the same value as repeated eval(x) calls
// element-by-element: the batch interface must be consistent with the scalar one.
TEST(CubicInterpND_1D, EvalnMatchesEval)
{
    cip::Vector x = {0.0, 1.0, 2.0, 3.0};
    cip::Vector f = {1.0, 2.0, 0.5, 3.0};
    Monotonic1D interp(x, f);

    cip::Vector xq = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
    auto fq = interp.evaln(xq);

    ASSERT_EQ(fq.size(), xq.size());
    for (std::size_t i = 0; i < xq.size(); ++i)
        EXPECT_NEAR(fq[i], interp.eval(xq[i]), 1e-14) << "query index " << i;
}

// A two-knot grid (one cell, the minimum grid size) must build without error
// and reproduce both endpoint values exactly.
TEST(CubicInterpND_1D, MinimalGrid_TwoKnots)
{
    cip::Vector x = {0.0, 1.0};
    cip::Vector f = {3.0, 7.0};
    Monotonic1D interp(x, f);

    EXPECT_NEAR(interp.eval(0.0), 3.0, TOL);
    EXPECT_NEAR(interp.eval(1.0), 7.0, TOL);
}

// ─── CubicInterpND 2D ────────────────────────────────────────────────────────

// eval at every 2-D grid node must exactly reproduce the prescribed f values.
TEST(CubicInterpND_2D, KnotsReturnExactValues)
{
    cip::Vector  x = {0.0, 1.0, 2.0};
    cip::Vector  y = {0.0, 1.0, 2.0};
    // f(x,y) = x + y
    cip::Vector2 f = {
        {0.0, 1.0, 2.0},   // x=0
        {1.0, 2.0, 3.0},   // x=1
        {2.0, 3.0, 4.0}    // x=2
    };
    Monotonic2D interp(x, y, f);

    for (std::size_t i = 0; i < x.size(); ++i)
        for (std::size_t j = 0; j < y.size(); ++j)
            EXPECT_NEAR(interp.eval(x[i], y[j]), f[i][j], TOL)
                << "knot (" << i << ", " << j << ")";
}

// evaln(xs, ys) must produce the same values as repeated scalar eval(x, y) calls.
TEST(CubicInterpND_2D, EvalnMatchesEval)
{
    cip::Vector  x = {0.0, 1.0, 2.0};
    cip::Vector  y = {0.0, 1.0, 2.0};
    cip::Vector2 f = {
        {0.0, 1.0, 2.0},
        {1.0, 2.0, 3.0},
        {2.0, 3.0, 4.0}
    };
    Monotonic2D interp(x, y, f);

    cip::Vector xq = {0.0, 0.5, 1.0, 1.5, 2.0};
    cip::Vector yq = {0.5, 1.0, 1.5, 0.5, 2.0};   // not a Cartesian product
    auto fq = interp.evaln(xq, yq);

    ASSERT_EQ(fq.size(), xq.size());
    for (std::size_t i = 0; i < xq.size(); ++i)
        EXPECT_NEAR(fq[i], interp.eval(xq[i], yq[i]), 1e-14) << "query index " << i;
}

// ─── CubicInterpND 3D ────────────────────────────────────────────────────────

// eval at every 3-D grid node must exactly reproduce the prescribed f values.
TEST(CubicInterpND_3D, KnotsReturnExactValues)
{
    cip::Vector  x = {0.0, 1.0, 2.0};
    cip::Vector  y = {0.0, 1.0, 2.0};
    cip::Vector  z = {0.0, 1.0, 2.0};
    // f(x,y,z) = x + y + z
    cip::Vector3 f = {
        {{0.0, 1.0, 2.0}, {1.0, 2.0, 3.0}, {2.0, 3.0, 4.0}},  // x=0
        {{1.0, 2.0, 3.0}, {2.0, 3.0, 4.0}, {3.0, 4.0, 5.0}},  // x=1
        {{2.0, 3.0, 4.0}, {3.0, 4.0, 5.0}, {4.0, 5.0, 6.0}}   // x=2
    };
    Monotonic3D interp(x, y, z, f);

    for (std::size_t i = 0; i < x.size(); ++i)
        for (std::size_t j = 0; j < y.size(); ++j)
            for (std::size_t k = 0; k < z.size(); ++k)
                EXPECT_NEAR(interp.eval(x[i], y[j], z[k]), f[i][j][k], TOL)
                    << "knot (" << i << ", " << j << ", " << k << ")";
}

// ─── Natural boundary condition — knot interpolation condition ──────────────
// The Hermite condition (eval at knot = prescribed f) must hold regardless of
// which boundary condition drives the global tridiagonal solve for slopes.

// Natural BC: f''(x0) = f''(xn) = 0
TEST(CubicInterpND_NaturalBC, KnotsReturnExactValues_Natural)
{
    cip::Vector x = {0.0, 1.0, 2.0, 3.0, 4.0};
    cip::Vector f = {0.0, 1.0, 4.0, 9.0, 16.0};   // f = x²
    NaturalBC1D interp(x, f);

    for (std::size_t i = 0; i < x.size(); ++i)
        EXPECT_NEAR(interp.eval(x[i]), f[i], TOL) << "knot index " << i;
}

// Not-a-knot BC: third derivative continuous across first and last interior knots
TEST(CubicInterpND_NaturalBC, KnotsReturnExactValues_NotAKnot)
{
    cip::Vector x = {0.0, 1.0, 2.0, 3.0, 4.0};
    cip::Vector f = {0.0, 1.0, 4.0, 9.0, 16.0};   // f = x²
    NotAKnot1D interp(x, f);

    for (std::size_t i = 0; i < x.size(); ++i)
        EXPECT_NEAR(interp.eval(x[i]), f[i], TOL) << "knot index " << i;
}

// Clamped BC: f'(x0) = f'(xn) = 0
TEST(CubicInterpND_NaturalBC, KnotsReturnExactValues_Clamped)
{
    cip::Vector x = {0.0, 1.0, 2.0, 3.0, 4.0};
    cip::Vector f = {0.0, 1.0, 4.0, 9.0, 16.0};   // f = x²
    Clamped1D interp(x, f);

    for (std::size_t i = 0; i < x.size(); ++i)
        EXPECT_NEAR(interp.eval(x[i]), f[i], TOL) << "knot index " << i;
}

// ─── IndexMethod equivalence ─────────────────────────────────────────────────

// On a uniform grid, BinarySearch and Uniform index methods must produce
// identical eval results at arbitrary query points.
TEST(CubicInterpND_Misc, IndexMethodEquivalence)
{
    using BS1D = cip::CubicInterp<double,
                                  cip::SlopePolicy<cip::SlopeMethod::Monotonic>,
                                  1,
                                  cip::IndexMethod::BinarySearch>;
    using U1D  = cip::CubicInterp<double,
                                  cip::SlopePolicy<cip::SlopeMethod::Monotonic>,
                                  1,
                                  cip::IndexMethod::Uniform>;

    cip::Vector x = {0.0, 1.0, 2.0, 3.0, 4.0};
    cip::Vector f = {0.0, 1.0, 4.0, 9.0, 16.0};   // f = x²
    BS1D bs(x, f);
    U1D  us(x, f);

    cip::Vector xq = {0.25, 0.75, 1.5, 2.3, 3.8};
    for (double xi : xq)
        EXPECT_NEAR(bs.eval(xi), us.eval(xi), 1e-14) << "at x=" << xi;
}
