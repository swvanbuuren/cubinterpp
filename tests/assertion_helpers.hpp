#include <vector>
#include <gtest/gtest.h>


namespace tcip {


using Vector = std::vector<double>;
using Vector2 = std::vector<Vector>;
using Vector3 = std::vector<Vector2>;

template <typename T>
testing::AssertionResult Interp1DAssertions(Vector x, Vector f, Vector x_fine, Vector f_fine) {
    T interp(x, f);
    for ( auto i = 0; i < x_fine.size(); i++ ) {
        auto val = interp.eval(x_fine[i]);
        if (!testing::internal::CmpHelperFloatingPointEQ<double>("expected", "actual", val, f_fine[i])  ) {
            return testing::AssertionFailure()
                << "for x = " << x_fine[i] << "expected " << f_fine[i] << " but got " << val;
        }
    }
    return testing::AssertionSuccess();
}



template <typename T>
testing::AssertionResult Interp2DAssertions(const tcip::Vector &x, const tcip::Vector &y, const Vector2 &f, const tcip::Vector &x_fine, const tcip::Vector &y_fine, const Vector2 &f_fine) {
    T interp2(x, y, f);
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


template <typename T>
testing::AssertionResult Interp3DAssertions(const tcip::Vector &x, const tcip::Vector &y, const tcip::Vector &z, const Vector3 &f, const tcip::Vector &x_fine, const tcip::Vector &y_fine, const tcip::Vector &z_fine, const Vector3 &f_fine) {
    T interp3(x, y, z, f);
    for ( auto i = 0; i < x_fine.size(); ++i ) {
        for ( auto j = 0; j < y_fine.size(); ++j ) {
            for ( auto k = 0; k < y_fine.size(); ++k ) {
                auto val = interp3.eval(x_fine[i], y_fine[j], z_fine[k]);
                if (!testing::internal::CmpHelperFloatingPointEQ<double>("expected", "actual", val, f_fine[i][j][k])  ) {
                    return testing::AssertionFailure()
                        << "for x = " << x_fine[i] << ", y = " << y_fine[j] << ", z = " << z_fine[j] << " expected " << f_fine[i][j][k] << " but got " << val;
                }
            }
        }
    }
    return testing::AssertionSuccess();
}


} // namespace tcip
