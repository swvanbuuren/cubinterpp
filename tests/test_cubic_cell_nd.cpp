// Unit tests for cip::CubicCellND<T, N>
//
// The cell implements N-dimensional piecewise cubic Hermite interpolation over
// a single cell [x0, x1]^N.  It is constructed from:
//   - an mdspan F(i_0,...,i_{N-1}, j_0,...,j_{N-1}) storing function values and
//     first partial derivatives at the 2^N corners, and
//   - an array of spans, one per dimension, each holding the two endpoint values.
//
// Theory reference: docs/theory/cubic.md
//
// Data layout for the mdspan (row-major over 2*N dimensions of size 2):
//   F(i_0,...,i_{N-1}, j_0,...,j_{N-1})
//   i_k ∈ {0,1} : corner index for dimension k  (0 = lower bound, 1 = upper bound)
//   j_k ∈ {0,1} : derivative order for dimension k (0 = f value, 1 = first partial)
//
// Strides (row-major, all extents = 2):
//   N=1 → strides {2, 1}                           (4 values)
//   N=2 → strides {8, 4, 2, 1}                     (16 values)
//   N=3 → strides {32, 16, 8, 4, 2, 1}             (64 values)

#include <gtest/gtest.h>
#include <cubinterpp.hpp>
#include <array>
#include <cmath>
#include <span>

static constexpr double TOL = 1e-10;

// ─── Construction helpers ─────────────────────────────────────────────────────

static cip::CubicCellND<double, 1>
make_cell_1d(const std::array<double, 4>& data, const std::array<double, 2>& xv)
{
    using Ext = std::dextents<std::size_t, 2>;
    using Map = std::layout_stride::mapping<Ext>;
    using MS  = std::mdspan<const double, Ext, std::layout_stride>;
    MS F(data.data(), Map{Ext{2, 2}, std::array<std::size_t, 2>{2, 1}});
    std::array<std::span<const double>, 1> spans{std::span<const double>(xv)};
    return cip::CubicCellND<double, 1>(F, spans);
}

static cip::CubicCellND<double, 2>
make_cell_2d(const std::array<double, 16>& data,
             const std::array<double, 2>& xv,
             const std::array<double, 2>& yv)
{
    using Ext = std::dextents<std::size_t, 4>;
    using Map = std::layout_stride::mapping<Ext>;
    using MS  = std::mdspan<const double, Ext, std::layout_stride>;
    MS F(data.data(), Map{Ext{2, 2, 2, 2}, std::array<std::size_t, 4>{8, 4, 2, 1}});
    std::array<std::span<const double>, 2> spans{
        std::span<const double>(xv), std::span<const double>(yv)};
    return cip::CubicCellND<double, 2>(F, spans);
}

static cip::CubicCellND<double, 3>
make_cell_3d(const std::array<double, 64>& data,
             const std::array<double, 2>& xv,
             const std::array<double, 2>& yv,
             const std::array<double, 2>& zv)
{
    using Ext = std::dextents<std::size_t, 6>;
    using Map = std::layout_stride::mapping<Ext>;
    using MS  = std::mdspan<const double, Ext, std::layout_stride>;
    MS F(data.data(), Map{Ext{2,2,2,2,2,2}, std::array<std::size_t, 6>{32,16,8,4,2,1}});
    std::array<std::span<const double>, 3> spans{
        std::span<const double>(xv),
        std::span<const double>(yv),
        std::span<const double>(zv)};
    return cip::CubicCellND<double, 3>(F, spans);
}

// ─── CubicCell1D ─────────────────────────────────────────────────────────────

// A constant function f(x)=C with zero slopes must be reproduced exactly at
// every point, including at endpoints and an interior point.
TEST(CubicCell1D, ConstantFunction)
{
    // F(i,j) where i=corner(0=x0,1=x1), j=deriv-order(0=f,1=f')
    std::array<double, 4> data = {5.0, 0.0, 5.0, 0.0};   // f=5, f'=0 at both ends
    std::array<double, 2> xv   = {1.0, 3.0};
    auto cell = make_cell_1d(data, xv);

    EXPECT_NEAR(cell.eval(1.0), 5.0, TOL);   // left endpoint
    EXPECT_NEAR(cell.eval(2.0), 5.0, TOL);   // interior
    EXPECT_NEAR(cell.eval(3.0), 5.0, TOL);   // right endpoint
}

// f(x)=x³ with exact Hermite derivatives must be reproduced exactly.
// Verifies that calc_coeffs correctly inverts the 4×4 Hermite change-of-basis
// for a cubic polynomial (the highest degree the cell can represent).
TEST(CubicCell1D, CubicExactReproduction)
{
    // f=x³, f'=3x²  on [1, 3]
    // F(0,0)=f(1)=1, F(0,1)=f'(1)=3, F(1,0)=f(3)=27, F(1,1)=f'(3)=27
    std::array<double, 4> data = {1.0, 3.0, 27.0, 27.0};
    std::array<double, 2> xv   = {1.0, 3.0};
    auto cell = make_cell_1d(data, xv);

    EXPECT_NEAR(cell.eval(1.5),  3.375, TOL);    // 1.5³
    EXPECT_NEAR(cell.eval(2.0),  8.0,   TOL);    // 2.0³
    EXPECT_NEAR(cell.eval(2.5), 15.625, TOL);    // 2.5³
}

// eval at both cell endpoints returns the prescribed function values,
// regardless of the slope values (Hermite interpolation condition).
TEST(CubicCell1D, CornerInterpolation)
{
    // arbitrary function values and slopes
    std::array<double, 4> data = {7.3, 1.2, -3.1, 0.7};
    std::array<double, 2> xv   = {0.5, 2.5};
    auto cell = make_cell_1d(data, xv);

    EXPECT_NEAR(cell.eval(0.5),  7.3, TOL);
    EXPECT_NEAR(cell.eval(2.5), -3.1, TOL);
}

// ─── CubicCell2D ─────────────────────────────────────────────────────────────
//
// Column order within each corner block (varying j in binary order):
//   [f, ∂f/∂y, ∂f/∂x, ∂²f/∂x∂y]
// i.e. data[ix*8 + iy*4 + jx*2 + jy] = F(ix, iy, jx, jy)

// eval at all four cell corners must return the prescribed f values,
// independent of the partial-derivative inputs.
TEST(CubicCell2D, CornerInterpolation)
{
    // f(x0,y0)=1, f(x0,y1)=2, f(x1,y0)=3, f(x1,y1)=4; all partial derivatives = 0
    std::array<double, 16> data = {
        1.0, 0.0, 0.0, 0.0,    // ix=0, iy=0
        2.0, 0.0, 0.0, 0.0,    // ix=0, iy=1
        3.0, 0.0, 0.0, 0.0,    // ix=1, iy=0
        4.0, 0.0, 0.0, 0.0     // ix=1, iy=1
    };
    std::array<double, 2> xv = {0.0, 1.0};
    std::array<double, 2> yv = {0.0, 2.0};
    auto cell = make_cell_2d(data, xv, yv);

    EXPECT_NEAR(cell.eval(0.0, 0.0), 1.0, TOL);
    EXPECT_NEAR(cell.eval(0.0, 2.0), 2.0, TOL);
    EXPECT_NEAR(cell.eval(1.0, 0.0), 3.0, TOL);
    EXPECT_NEAR(cell.eval(1.0, 2.0), 4.0, TOL);
}

// f(x,y)=x·y with exact partial derivatives must be reproduced exactly at
// interior points, exercising the mixed-derivative term ∂²f/∂x∂y.
TEST(CubicCell2D, BilinearExactReproduction)
{
    // f=xy,  ∂f/∂x=y,  ∂f/∂y=x,  ∂²f/∂x∂y=1  on [0,1]²
    std::array<double, 16> data = {
    //  f      ∂f/∂y  ∂f/∂x  ∂²/∂x∂y
        0.0,   0.0,   0.0,   1.0,    // ix=0(x=0), iy=0(y=0)
        0.0,   0.0,   1.0,   1.0,    // ix=0(x=0), iy=1(y=1): ∂f/∂x = y = 1
        0.0,   1.0,   0.0,   1.0,    // ix=1(x=1), iy=0(y=0): ∂f/∂y = x = 1
        1.0,   1.0,   1.0,   1.0     // ix=1(x=1), iy=1(y=1)
    };
    std::array<double, 2> xv = {0.0, 1.0};
    std::array<double, 2> yv = {0.0, 1.0};
    auto cell = make_cell_2d(data, xv, yv);

    EXPECT_NEAR(cell.eval(0.5, 0.5), 0.25, TOL);
    EXPECT_NEAR(cell.eval(0.3, 0.7), 0.21, TOL);
    EXPECT_NEAR(cell.eval(0.8, 0.2), 0.16, TOL);
}

// f(x,y)=x³·y³ with exact Hermite data must be reproduced exactly.
// This is the maximum-degree polynomial the bicubic Hermite cell can represent
// (degree-3 independently in each variable).
TEST(CubicCell2D, CubicProductExactReproduction)
{
    // f=x³y³,  ∂f/∂x=3x²y³,  ∂f/∂y=3x³y²,  ∂²f/∂x∂y=9x²y²  on [1,2]²
    //
    //  Corner (x=1,y=1): f=1,  ∂f/∂x=3,   ∂f/∂y=3,   ∂²f/∂x∂y=9
    //  Corner (x=1,y=2): f=8,  ∂f/∂x=24,  ∂f/∂y=12,  ∂²f/∂x∂y=36
    //  Corner (x=2,y=1): f=8,  ∂f/∂x=12,  ∂f/∂y=24,  ∂²f/∂x∂y=36
    //  Corner (x=2,y=2): f=64, ∂f/∂x=96,  ∂f/∂y=96,  ∂²f/∂x∂y=144
    std::array<double, 16> data = {
    //  f       ∂f/∂y   ∂f/∂x   ∂²f/∂x∂y
         1.0,    3.0,    3.0,     9.0,    // ix=0(x=1), iy=0(y=1)
         8.0,   12.0,   24.0,    36.0,    // ix=0(x=1), iy=1(y=2)
         8.0,   24.0,   12.0,    36.0,    // ix=1(x=2), iy=0(y=1)
        64.0,   96.0,   96.0,   144.0     // ix=1(x=2), iy=1(y=2)
    };
    std::array<double, 2> xv = {1.0, 2.0};
    std::array<double, 2> yv = {1.0, 2.0};
    auto cell = make_cell_2d(data, xv, yv);

    EXPECT_NEAR(cell.eval(1.5, 1.5), 11.390625, TOL);   // (1.5³)·(1.5³)
    EXPECT_NEAR(cell.eval(1.2, 1.8), std::pow(1.2, 3.0) * std::pow(1.8, 3.0), TOL);
    EXPECT_NEAR(cell.eval(2.0, 1.0),  8.0, TOL);        // corner check
}

// ─── CubicCell3D ─────────────────────────────────────────────────────────────
//
// Column order within each corner block (varying j in binary order):
//   [f, ∂f/∂z, ∂f/∂y, ∂²f/∂y∂z, ∂f/∂x, ∂²f/∂x∂z, ∂²f/∂x∂y, ∂³f/∂x∂y∂z]
// i.e. data[ix*32 + iy*16 + iz*8 + jx*4 + jy*2 + jz] = F(ix,iy,iz,jx,jy,jz)

// eval at all eight corners of a 3-D cell must return the prescribed f values.
TEST(CubicCell3D, CornerInterpolation)
{
    // f(x,y,z) = x + 2y + 3z  on [0,1]³
    // ∂f/∂x=1, ∂f/∂y=2, ∂f/∂z=3;  all mixed partial derivatives = 0
    //
    // Each corner block:  [f, 3, 2, 0, 1, 0, 0, 0]
    std::array<double, 64> data = {
        0,3,2,0,1,0,0,0,  // corner (0,0,0): f = 0
        3,3,2,0,1,0,0,0,  // corner (0,0,1): f = 3
        2,3,2,0,1,0,0,0,  // corner (0,1,0): f = 2
        5,3,2,0,1,0,0,0,  // corner (0,1,1): f = 5
        1,3,2,0,1,0,0,0,  // corner (1,0,0): f = 1
        4,3,2,0,1,0,0,0,  // corner (1,0,1): f = 4
        3,3,2,0,1,0,0,0,  // corner (1,1,0): f = 3
        6,3,2,0,1,0,0,0   // corner (1,1,1): f = 6
    };
    std::array<double, 2> xv = {0.0, 1.0};
    std::array<double, 2> yv = {0.0, 1.0};
    std::array<double, 2> zv = {0.0, 1.0};
    auto cell = make_cell_3d(data, xv, yv, zv);

    EXPECT_NEAR(cell.eval(0.0, 0.0, 0.0), 0.0, TOL);
    EXPECT_NEAR(cell.eval(0.0, 0.0, 1.0), 3.0, TOL);
    EXPECT_NEAR(cell.eval(0.0, 1.0, 0.0), 2.0, TOL);
    EXPECT_NEAR(cell.eval(0.0, 1.0, 1.0), 5.0, TOL);
    EXPECT_NEAR(cell.eval(1.0, 0.0, 0.0), 1.0, TOL);
    EXPECT_NEAR(cell.eval(1.0, 0.0, 1.0), 4.0, TOL);
    EXPECT_NEAR(cell.eval(1.0, 1.0, 0.0), 3.0, TOL);
    EXPECT_NEAR(cell.eval(1.0, 1.0, 1.0), 6.0, TOL);
}
