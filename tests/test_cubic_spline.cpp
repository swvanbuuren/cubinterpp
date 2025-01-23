#include <vector>
#include <gtest/gtest.h>
#include <cubic_spline.hpp>

using Vector = std::vector<double>;


testing::AssertionResult MonotonicSpline1DAssertions(Vector x, Vector f, Vector x_fine, Vector f_fine) {
    cip::MonotonicSpline1D<double> interp(x, f);
    for ( auto i = 0; i < x_fine.size(); i++ ) {
        auto val = interp.eval(x_fine[i]);
        if (!testing::internal::CmpHelperFloatingPointEQ<double>("expected", "actual", val, f_fine[i])  ) {
            return testing::AssertionFailure()
                << "for x = " << x_fine[i] << "expected " << f_fine[i] << " but got " << val;
        }
    }
    return testing::AssertionSuccess();
}


TEST(TestCubicSpline1D, test_monotonic_spline_1d) {
    Vector x = { 1, 2, 3, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10 };
    Vector f = { 0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6 };
    Vector x_fine = { 1.0, 1.375, 1.75, 2.125, 2.5, 2.875, 3.25, 3.625, 4.0, 4.375, 4.75, 5.125, 5.5, 5.875, 6.25, 6.625, 7.0, 7.375, 7.75, 8.125, 8.5, 8.875, 9.25, 9.625, 10.0 };
    Vector f_fine = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.07450161897651952, 0.3304738093015729, 0.5, 0.4542055428769416, 0.37848534450908033, 0.5453815182574999, 1.2, 1.2, 1.2, 1.2, 1.2000000000003865, 0.8762920673080998, 0.31081730769267324, 0.06762319711538467, 0.0009615384615386802, -0.01053185096153797, 0.11971153846116067, 0.3749999999999991, 0.5999999999999988 };
    ASSERT_TRUE(MonotonicSpline1DAssertions(x, f, x_fine, f_fine));
}
