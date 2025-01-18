#include <vector>
#include <gtest/gtest.h>
#include <utility>
#include "linear_interp.hpp"

using Vector = std::vector<double>;
using Vector2 = std::vector<std::vector<double>>;
using VectorN1 = cip::VectorN<double, 1>;
using VectorN2 = cip::VectorN<double, 2>;
using Span = std::span<const double>;
using Pr = std::pair<size_t, size_t>;


testing::AssertionResult Interp1DAssertions(Vector x, Vector f, Vector x_fine, Vector f_fine) {
    cip::LinearInterp1D<double> interp(x, f);
    for ( auto i = 0; i < x_fine.size(); i++ ) {
        auto val = interp.eval(x_fine[i]);
        if (!testing::internal::CmpHelperFloatingPointEQ<double>("expected", "actual", val, f_fine[i])  ) {
            return testing::AssertionFailure()
                << "for x = " << x_fine[i] << "expected " << f_fine[i] << " but got " << val;
        }
    }
    return testing::AssertionSuccess();
}



TEST(TestLinearCell1D, test_linear_cell_1d) {
    Vector x = {0, 1, 2};
    Vector f = {3, 3, 4};

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
    Vector x = { 1.0, 2.0, 3.0, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10.0 };
    Vector f = { 0.0, 0.0, 0.0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6 };
    Vector x_fine = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
    Vector f_fine = { 0.0, 0.0, 0.0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.6 };
    ASSERT_TRUE(Interp1DAssertions(x, f, x_fine, f_fine));
}


TEST(TestInterp1D, test_linear_interp_1d_random) {
    Vector x = { 1.0, 1.5714285714285714, 2.142857142857143, 2.7142857142857144, 3.2857142857142856, 3.8571428571428568, 4.428571428571429, 5.0 };
    Vector f = { 4.0, 0.0, 6.0, 2.0, 3.0, 8.0, 4.0, 9.0 };
    Vector x_fine = { 1.0, 1.4444444444444444, 1.8888888888888888, 2.333333333333333, 2.7777777777777777, 3.2222222222222223, 3.6666666666666665, 4.111111111111111, 4.555555555555555, 5.0 };
    Vector f_fine = { 4.0, 0.8888888888888888, 3.3333333333333335, 4.66666666666667, 2.1111111111111107, 2.8888888888888893, 6.333333333333335, 6.222222222222224, 5.111111111111107, 9.0 };
    ASSERT_TRUE(Interp1DAssertions(x, f, x_fine, f_fine));
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


testing::AssertionResult Interp2DEvalAssertions(const Vector &x, const Vector &y, const Vector2 &f, const Vector &x_fine, const Vector &y_fine, const Vector2 &f_fine) {
    cip::LinearInterp2D<double> interp2(x, y, f);
    for ( auto i = 0; i < x_fine.size(); ++i ) {
        for ( auto j = 0; j < y_fine.size(); ++j ) {
            auto val = interp2.eval(x_fine[i], y_fine[j]);
            if (!testing::internal::CmpHelperFloatingPointEQ<double>("expected", "actual", val, f_fine[i][j])  ) {
                return testing::AssertionFailure()
                    << "for x = " << x_fine[i] << ", y = " << y_fine[j] << " expected " << f_fine[i][j] << " but got " << val;
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
    ASSERT_TRUE(Interp2DEvalAssertions(x, y, f, x_fine, y_fine, f_fine));    
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
    ASSERT_TRUE(Interp2DEvalAssertions(x, y, f, x_fine, y_fine, f_fine));    
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
    ASSERT_TRUE(Interp2DEvalAssertions(x, y, f, x_fine, y_fine, f_fine));    
}
