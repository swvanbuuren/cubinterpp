#include <gtest/gtest.h>
#include <cubic_spline.hpp>
#include "assertion_helpers.hpp"


using Vector = std::vector<double>;
using MonotonicSpline = cip::MonotonicSpline1D<double>;


TEST(TestCubicSpline1D, test_monotonic_spline_1d) {
    tcip::Vector x = { 1, 2, 3, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10 };
    tcip::Vector f = { 0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6 };
    tcip::Vector x_fine = { 1.0, 1.375, 1.75, 2.125, 2.5, 2.875, 3.25, 3.625, 4.0, 4.375, 4.75, 5.125, 5.5, 5.875, 6.25, 6.625, 7.0, 7.375, 7.75, 8.125, 8.5, 8.875, 9.25, 9.625, 10.0 };
    tcip::Vector f_fine = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.07450161897651952, 0.3304738093015729, 0.5, 0.4542055428769416, 0.37848534450908033, 0.5453815182574999, 1.2, 1.2, 1.2, 1.2, 1.2000000000003865, 0.8762920673080998, 0.31081730769267324, 0.06762319711538467, 0.0009615384615386802, -0.01053185096153797, 0.11971153846116067, 0.3749999999999991, 0.5999999999999988 };
    ASSERT_TRUE(tcip::Interp1DAssertions<MonotonicSpline>(x, f, x_fine, f_fine));
}
