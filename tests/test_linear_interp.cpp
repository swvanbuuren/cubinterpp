#include <gtest/gtest.h>
#include <utility>
#include <cubinterpp.hpp>
#include "assertion_helpers.hpp"


using VectorN1 = cip::VectorN<double, 1>;
using VectorN2 = cip::VectorN<double, 2>;
using VectorN3 = cip::VectorN<double, 3>;
using VectorN4 = cip::VectorN<double, 4>;
using Span = std::span<const double>;
using Pr = std::pair<size_t, size_t>;


TEST(TestLinearCell1D, test_linear_cell_1d) {
    cip::Vector x = {0, 1, 2};
    cip::Vector f = {3, 3, 4};

    VectorN1 fvec(f);
    size_t i = 1;
    cip::LinearCellND<double, 1> cell(
        Span(&x[i], 2),
        Span(&f[i], 2)
    );
    ASSERT_EQ(cell.eval(1), 3.0);
    ASSERT_EQ(cell.eval(1), 3.0);
    ASSERT_EQ(cell.eval(2), 4.0);
    ASSERT_EQ(cell.eval(1.5), 3.5);
}


TEST(TestInterp1D, test_linear_interp_1d_akima) {
    cip::Vector x = { 1.0, 2.0, 3.0, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10.0 };
    cip::Vector f = { 0.0, 0.0, 0.0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6 };
    cip::Vector x_fine = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
    cip::Vector f_fine = { 0.0, 0.0, 0.0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.6 };
    ASSERT_TRUE(cip::Interp1DAssertions<cip::LinearInterp1D<double>>(x, f, x_fine, f_fine));
}


TEST(TestInterp1D, test_linear_interp_1d_random) {
    cip::Vector x = { 1.0, 1.5714285714285714, 2.142857142857143, 2.7142857142857144, 3.2857142857142856, 3.8571428571428568, 4.428571428571429, 5.0 };
    cip::Vector f = { 4.0, 0.0, 6.0, 2.0, 3.0, 8.0, 4.0, 9.0 };
    cip::Vector x_fine = { 1.0, 1.4444444444444444, 1.8888888888888888, 2.333333333333333, 2.7777777777777777, 3.2222222222222223, 3.6666666666666665, 4.111111111111111, 4.555555555555555, 5.0 };
    cip::Vector f_fine = { 4.0, 0.8888888888888888, 3.3333333333333335, 4.66666666666667, 2.1111111111111107, 2.8888888888888893, 6.333333333333335, 6.222222222222224, 5.111111111111107, 9.0 };
    ASSERT_TRUE(cip::Interp1DAssertions<cip::LinearInterp1D<double>>(x, f, x_fine, f_fine));
}


TEST(TestLinearCell2D, test_linear_cell_2d) {
    cip::Vector x = {0, 1, 2};
    cip::Vector y = {0, 1, 2};
    cip::Vector2 f = {{1, 2, 2},
                 {2, 3, 3},
                 {3, 3, 4}};

    VectorN2 fvec(f);
    size_t i = 1;
    size_t j = 1;
    cip::LinearCellND<double, 2> cell(
        fvec.submdspan(Pr{i, i+1}, Pr{j, j+1}),
        {Span(&x[i], 2),
        Span(&y[j], 2)}
    );
    ASSERT_EQ(cell.eval(1.0, 1.0), 3.0);
    ASSERT_EQ(cell.eval(1.0, 2.0), 3.0);
    ASSERT_EQ(cell.eval(2.0, 1.0), 3.0);
    ASSERT_EQ(cell.eval(2.0, 2.0), 4.0);
    ASSERT_EQ(cell.eval(1.5, 1.5), 3.25);
}


TEST(TestInterp2D, test_linear_interp_2d_normalized) {
    cip::Vector x = { 0.0, 1.0, 2.0 };
    cip::Vector y = { 0.0, 1.0, 2.0 };
    cip::Vector2 f = { 
                { 1.0, 2.0, 2.0 }, 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 3.0, 4.0 }
                };
    cip::Vector x_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector y_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector2 f_fine = { 
                    { 1.0, 1.5, 2.0, 2.0, 2.0 }, 
                    { 1.5, 2.0, 2.5, 2.5, 2.5 }, 
                    { 2.0, 2.5, 3.0, 3.0, 3.0 }, 
                    { 2.5, 2.75, 3.0, 3.25, 3.5 }, 
                    { 3.0, 3.0, 3.0, 3.5, 4.0 }
                    };
    ASSERT_TRUE(cip::Interp2DAssertions<cip::LinearInterp2D<double>>(x, y, f, x_fine, y_fine, f_fine));    
}


TEST(TestInterp2D, test_linear_interp_2d_standard) {
    cip::Vector x = { 0.0, 1.5, 3.0 };
    cip::Vector y = { 0.0, 2.0, 4.0 };
    cip::Vector2 f = { 
                { 1.0, 2.0, 2.0 }, 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 3.0, 4.0 }
                };
    cip::Vector x_fine = { 0.0, 0.75, 1.5, 2.25, 3.0 };
    cip::Vector y_fine = { 0.0, 1.0, 2.0, 3.0, 4.0 };
    cip::Vector2 f_fine = { 
                    { 1.0, 1.5, 2.0, 2.0, 2.0 }, 
                    { 1.5, 2.0, 2.5, 2.5, 2.5 }, 
                    { 2.0, 2.5, 3.0, 3.0, 3.0 }, 
                    { 2.5, 2.75, 3.0, 3.25, 3.5 }, 
                    { 3.0, 3.0, 3.0, 3.5, 4.0 }
                    };
    ASSERT_TRUE(cip::Interp2DAssertions<cip::LinearInterp2D<double>>(x, y, f, x_fine, y_fine, f_fine));    
}


TEST(TestInterp2D, test_linear_interp_2d_non_monotonic) {
    cip::Vector x = { 0.0, 1.0, 1.5 };
    cip::Vector y = { 0.0, 0.5, 3.0 };
    cip::Vector2 f = { 
                { 1.0, 2.0, 2.0 }, 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 3.0, 4.0 }
                };
    cip::Vector x_fine = { 0.0, 0.375, 0.75, 1.125, 1.5 };
    cip::Vector y_fine = { 0.0, 0.75, 1.5, 2.25, 3.0 };
    cip::Vector2 f_fine = { 
                    { 1.0, 2.0, 2.0, 2.0, 2.0 }, 
                    { 1.375, 2.375, 2.375, 2.375, 2.375 }, 
                    { 1.75, 2.75, 2.75, 2.75, 2.75 }, 
                    { 2.25, 3.025, 3.1, 3.175, 3.25 }, 
                    { 3.0, 3.1, 3.4, 3.7, 4.0 }
                    };
    ASSERT_TRUE(cip::Interp2DAssertions<cip::LinearInterp2D<double>>(x, y, f, x_fine, y_fine, f_fine));    
}


TEST(TestInterp3D, test_linear_interp_3d) {
    cip::Vector x = { 0.0, 1.0, 2.0 };
    cip::Vector y = { 0.0, 1.0, 2.0 };
    cip::Vector z = { 0.0, 1.0, 2.0 };
    cip::Vector3 f = { 
              { 
                { 1.0, 2.0, 2.0 }, 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 3.0, 4.0 }
            }, 
              { 
                { 3.0, 4.0, 4.0 }, 
                { 4.0, 5.0, 5.0 }, 
                { 5.0, 5.0, 6.0 }
            }, 
              { 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 4.0, 4.0 }, 
                { 4.0, 4.0, 5.0 }
            }
            };
    cip::Vector x_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector y_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector z_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector3 f_fine = { 
                   { 
                     { 1.0, 1.5, 2.0, 2.0, 2.0 }, 
                     { 1.5, 2.0, 2.5, 2.5, 2.5 }, 
                     { 2.0, 2.5, 3.0, 3.0, 3.0 }, 
                     { 2.5, 2.75, 3.0, 3.25, 3.5 }, 
                     { 3.0, 3.0, 3.0, 3.5, 4.0 }
                    }, 
                    { 
                     { 2.0, 2.5, 3.0, 3.0, 3.0 }, 
                     { 2.5, 3.0, 3.5, 3.5, 3.5 }, 
                     { 3.0, 3.5, 4.0, 4.0, 4.0 }, 
                     { 3.5, 3.75, 4.0, 4.25, 4.5 }, 
                     { 4.0, 4.0, 4.0, 4.5, 5.0 }
                 }, 
                   { 
                     { 3.0, 3.5, 4.0, 4.0, 4.0 }, 
                     { 3.5, 4.0, 4.5, 4.5, 4.5 }, 
                     { 4.0, 4.5, 5.0, 5.0, 5.0 }, 
                     { 4.5, 4.75, 5.0, 5.25, 5.5 }, 
                     { 5.0, 5.0, 5.0, 5.5, 6.0 }
                 }, 
                   { 
                     { 2.5, 3.0, 3.5, 3.5, 3.5 }, 
                     { 3.0, 3.5, 4.0, 4.0, 4.0 }, 
                     { 3.5, 4.0, 4.5, 4.5, 4.5 }, 
                     { 4.0, 4.25, 4.5, 4.75, 5.0 }, 
                     { 4.5, 4.5, 4.5, 5.0, 5.5 }
                 }, 
                   { 
                     { 2.0, 2.5, 3.0, 3.0, 3.0 }, 
                     { 2.5, 3.0, 3.5, 3.5, 3.5 }, 
                     { 3.0, 3.5, 4.0, 4.0, 4.0 }, 
                     { 3.5, 3.75, 4.0, 4.25, 4.5 }, 
                     { 4.0, 4.0, 4.0, 4.5, 5.0 }
                 }
                };

ASSERT_TRUE(cip::Interp3DAssertions<cip::LinearInterp3D<double>>(x, y, z, f, x_fine, y_fine, z_fine, f_fine));    

}

/**
 * Test for 4D linear interpolation
 * 
 * This test demonstrates using the InterpNDAssertions class with N=4 dimensions.
 * We use a simple 4D linear function f(x,y,z,w) = a*x + b*y + c*z + d*w + e
 * to generate the test data, which should be perfectly interpolated by a linear interpolator.
 */
TEST(TestInterp4D, test_linear_interp_4d) {
    // Create grid vectors for each dimension
    cip::Vector x = { 0.0, 1.0, 2.0 };
    cip::Vector y = { 0.0, 1.0, 2.0 };
    cip::Vector z = { 0.0, 1.0, 2.0 };
    cip::Vector w = { 0.0, 1.0, 2.0 };
    
    // Define a linear function coefficients: f(x,y,z,w) = 2*x + 3*y - z + 0.5*w + 1
    const double a = 2.0;
    const double b = 3.0;
    const double c = -1.0;
    const double d = 0.5;
    const double e = 1.0;
    
    // Create 4D function values array
    cip::Vector4 f;
    f.resize(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        f[i].resize(y.size());
        for (size_t j = 0; j < y.size(); ++j) {
            f[i][j].resize(z.size());
            for (size_t k = 0; k < z.size(); ++k) {
                f[i][j][k].resize(w.size());
                for (size_t l = 0; l < w.size(); ++l) {
                    // Function: f(x,y,z,w) = 2*x + 3*y - z + 0.5*w + 1
                    f[i][j][k][l] = a*x[i] + b*y[j] + c*z[k] + d*w[l] + e;
                }
            }
        }
    }
    
    // Fine grid for testing interpolation
    cip::Vector x_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector y_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector z_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector w_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    
    // Expected values at fine grid points
    cip::Vector4 f_fine;
    f_fine.resize(x_fine.size());
    for (size_t i = 0; i < x_fine.size(); ++i) {
        f_fine[i].resize(y_fine.size());
        for (size_t j = 0; j < y_fine.size(); ++j) {
            f_fine[i][j].resize(z_fine.size());
            for (size_t k = 0; k < z_fine.size(); ++k) {
                f_fine[i][j][k].resize(w_fine.size());
                for (size_t l = 0; l < w_fine.size(); ++l) {
                    // Function: f(x,y,z,w) = 2*x + 3*y - z + 0.5*w + 1
                    f_fine[i][j][k][l] = a*x_fine[i] + b*y_fine[j] + c*z_fine[k] + d*w_fine[l] + e;
                }
            }
        }
    }
    
    // Test the 4D interpolation using our InterpNDAssertions class
    ASSERT_TRUE(cip::Interp4DAssertions<cip::LinearInterp4D<double>>(
        x, y, z, w, f, x_fine, y_fine, z_fine, w_fine, f_fine));
}

// Direct test using the InterpNDAssertions template with N=4
TEST(TestInterp4D, test_direct_nd_assertions) {
    // Create grid vectors for each dimension
    cip::Vector x = { 0.0, 1.0, 2.0 };
    cip::Vector y = { 0.0, 1.0, 2.0 };
    cip::Vector z = { 0.0, 1.0, 2.0 };
    cip::Vector w = { 0.0, 1.0, 2.0 };
    
    // Create 4D function values array
    // Function is f(x,y,z,w) = x + y + z + w
    cip::Vector4 f;
    f.resize(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        f[i].resize(y.size());
        for (size_t j = 0; j < y.size(); ++j) {
            f[i][j].resize(z.size());
            for (size_t k = 0; k < z.size(); ++k) {
                f[i][j][k].resize(w.size());
                for (size_t l = 0; l < w.size(); ++l) {
                    // Function: f(x,y,z,w) = x + y + z + w
                    f[i][j][k][l] = x[i] + y[j] + z[k] + w[l];
                }
            }
        }
    }
    
    // Fine grid for testing interpolation
    cip::Vector x_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector y_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector z_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    cip::Vector w_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    
    // Expected values at fine grid points
    cip::Vector4 f_fine;
    f_fine.resize(x_fine.size());
    for (size_t i = 0; i < x_fine.size(); ++i) {
        f_fine[i].resize(y_fine.size());
        for (size_t j = 0; j < y_fine.size(); ++j) {
            f_fine[i][j].resize(z_fine.size());
            for (size_t k = 0; k < z_fine.size(); ++k) {
                f_fine[i][j][k].resize(w_fine.size());
                for (size_t l = 0; l < w_fine.size(); ++l) {
                    // Function: f(x,y,z,w) = x + y + z + w
                    f_fine[i][j][k][l] = x_fine[i] + y_fine[j] + z_fine[k] + w_fine[l];
                }
            }
        }
    }
    
    // Pack grid vectors and fine grid vectors into arrays
    std::array<cip::Vector, 4> grid_vectors = {x, y, z, w};
    std::array<cip::Vector, 4> fine_grid_vectors = {x_fine, y_fine, z_fine, w_fine};
    
    // Test the 4D interpolation directly using the InterpNDAssertions class
    auto result = cip::InterpNDAssertions<cip::LinearInterp4D<double>, 4, cip::Vector4>::test(
        grid_vectors, f, fine_grid_vectors, f_fine);
    ASSERT_TRUE(result);
}

// Test a non-linear function with 4D interpolation
TEST(TestInterp4D, test_nonlinear_4d) {
    // Create grid vectors for each dimension
    cip::Vector x = { 0.0, 1.0, 2.0 };
    cip::Vector y = { 0.0, 1.0, 2.0 };
    cip::Vector z = { 0.0, 1.0, 2.0 };
    cip::Vector w = { 0.0, 1.0, 2.0 };
    
    // Create 4D function values array for a quadratic function
    // Function is f(x,y,z,w) = x^2 + y^2 + z^2 + w^2
    cip::Vector4 f;
    f.resize(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        f[i].resize(y.size());
        for (size_t j = 0; j < y.size(); ++j) {
            f[i][j].resize(z.size());
            for (size_t k = 0; k < z.size(); ++k) {
                f[i][j][k].resize(w.size());
                for (size_t l = 0; l < w.size(); ++l) {
                    // Quadratic function
                    f[i][j][k][l] = x[i]*x[i] + y[j]*y[j] + z[k]*z[k] + w[l]*w[l];
                }
            }
        }
    }
    
    // Create interpolator
    cip::LinearInterp4D<double> interp(x, y, z, w, f);
    
    // Test a few specific points without using fine grid
    // For interpolation at grid points, result should be exact
    EXPECT_DOUBLE_EQ(interp.eval(0.0, 0.0, 0.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(interp.eval(1.0, 1.0, 1.0, 1.0), 4.0);
    EXPECT_DOUBLE_EQ(interp.eval(2.0, 2.0, 2.0, 2.0), 16.0);
}
