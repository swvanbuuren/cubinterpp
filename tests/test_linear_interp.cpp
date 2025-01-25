#include <gtest/gtest.h>
#include <utility>
#include "linear_interp.hpp"
#include "assertion_helpers.hpp"


using VectorN1 = cip::VectorN<double, 1>;
using VectorN2 = cip::VectorN<double, 2>;
using VectorN3 = cip::VectorN<double, 3>;
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
