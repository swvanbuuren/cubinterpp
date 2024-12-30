#include <vector>
#include <gtest/gtest.h>
#include <utility>
#include "linear_interp.hpp"

using Vector = std::vector<double>;
using Vector2 = std::vector<std::vector<double>>;
using VectorN2 = cip::VectorN<double, 2>;
using Span = std::span<const double>;
using Pr = std::pair<size_t, size_t>;



TEST(TestLinearCell1D, test_linear_cell_1d) {
    
}


TEST(TestInterp1D, test_linear_interp_1d) {
    
}


TEST(TestLinearCell2D, test_linear_cell_2d) {
    Vector x = {0, 1, 2};
    Vector y = {0, 1, 2};
    Vector2 f = {{1, 2, 2},
                 {2, 3, 3},
                 {3, 3, 4}};

    VectorN2 fvec(f);
    size_t i = 1;
    size_t j = 1;
    cip::LinearCell2D<double> cell(
        fvec.submdspan(Pr{i, i+1}, Pr{j, j+1}),
        Span(&x[i], 2),
        Span(&y[j], 2)
    );
    ASSERT_EQ(cell.eval(1, 1), 3.0);
    ASSERT_EQ(cell.eval(1, 2), 3.0);
    ASSERT_EQ(cell.eval(2, 1), 3.0);
    ASSERT_EQ(cell.eval(2, 2), 4.0);
    ASSERT_EQ(cell.eval(1.5, 1.5), 3.25);
}


testing::AssertionResult Interp2DAssertions(Vector x, Vector y, Vector2 f, Vector x_fine, Vector y_fine, Vector2 f_fine) {
    cip::LinearInterp2D<double> interp2(x, y, f);
    for ( auto i = 0; i < x_fine.size(); i++ ) {
        for ( auto j = 0; j < y_fine.size(); j++ ) {
            auto val = interp2.eval(x_fine[i], y_fine[j]);
            if (!testing::internal::CmpHelperFloatingPointEQ<double>("expected", "actual", val, f_fine[i][j])  ) {
                return testing::AssertionFailure()
                    << "for x = " << x_fine[i] << ", y = " << y_fine[j] << "expected " << f_fine[i][j] << " but got " << val;
            }
        }
    }
    return testing::AssertionSuccess();
}


TEST(TestInterp2D, test_linear_interp_2d_normalized) {
    Vector x = { 0.0, 1.0, 2.0 };
    Vector y = { 0.0, 1.0, 2.0 };
    Vector2 f = { 
                { 1.0, 2.0, 2.0 }, 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 3.0, 4.0 }
                };
    Vector x_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    Vector y_fine = { 0.0, 0.5, 1.0, 1.5, 2.0 };
    Vector2 f_fine = { 
                    { 1.0, 1.5, 2.0, 2.0, 2.0 }, 
                    { 1.5, 2.0, 2.5, 2.5, 2.5 }, 
                    { 2.0, 2.5, 3.0, 3.0, 3.0 }, 
                    { 2.5, 2.75, 3.0, 3.25, 3.5 }, 
                    { 3.0, 3.0, 3.0, 3.5, 4.0 }
                    };
    EXPECT_TRUE(Interp2DAssertions(x, y, f, x_fine, y_fine, f_fine));    
}


TEST(TestInterp2D, test_linear_interp_2d_standard) {
    Vector x = { 0.0, 1.5, 3.0 };
    Vector y = { 0.0, 2.0, 4.0 };
    Vector2 f = { 
                { 1.0, 2.0, 2.0 }, 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 3.0, 4.0 }
                };
    Vector x_fine = { 0.0, 0.75, 1.5, 2.25, 3.0 };
    Vector y_fine = { 0.0, 1.0, 2.0, 3.0, 4.0 };
    Vector2 f_fine = { 
                    { 1.0, 1.5, 2.0, 2.0, 2.0 }, 
                    { 1.5, 2.0, 2.5, 2.5, 2.5 }, 
                    { 2.0, 2.5, 3.0, 3.0, 3.0 }, 
                    { 2.5, 2.75, 3.0, 3.25, 3.5 }, 
                    { 3.0, 3.0, 3.0, 3.5, 4.0 }
                    };
    EXPECT_TRUE(Interp2DAssertions(x, y, f, x_fine, y_fine, f_fine));    
}


TEST(TestInterp2D, test_linear_interp_2d_non_monotonic) {
    Vector x = { 0.0, 1.0, 1.5 };
    Vector y = { 0.0, 0.5, 3.0 };
    Vector2 f = { 
                { 1.0, 2.0, 2.0 }, 
                { 2.0, 3.0, 3.0 }, 
                { 3.0, 3.0, 4.0 }
                };
    Vector x_fine = { 0.0, 0.375, 0.75, 1.125, 1.5 };
    Vector y_fine = { 0.0, 0.75, 1.5, 2.25, 3.0 };
    Vector2 f_fine = { 
                    { 1.0, 2.0, 2.0, 2.0, 2.0 }, 
                    { 1.375, 2.375, 2.375, 2.375, 2.375 }, 
                    { 1.75, 2.75, 2.75, 2.75, 2.75 }, 
                    { 2.25, 3.025, 3.1, 3.175, 3.25 }, 
                    { 3.0, 3.1, 3.4, 3.7, 4.0 }
                    };
    EXPECT_TRUE(Interp2DAssertions(x, y, f, x_fine, y_fine, f_fine));    
}
