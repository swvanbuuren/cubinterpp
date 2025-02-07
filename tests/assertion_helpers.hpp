#include <vector>
#include <gtest/gtest.h>


namespace cip {


using Vector = std::vector<double>;
using Vector2 = std::vector<Vector>;
using Vector3 = std::vector<Vector2>;

const double TOLERANCE = 5.0e-12;

template <typename T>
testing::AssertionResult Interp1DAssertions(Vector x, Vector f, Vector x_fine, Vector f_fine) {
    T interp(x, f);
    for ( auto i = 0; i < x_fine.size(); i++ ) {
        auto val = interp.eval(x_fine[i]);
        if (!testing::internal::DoubleNearPredFormat("expected", "actual", "tolerance", val, f_fine[i], TOLERANCE)  ) {
            return testing::AssertionFailure()
                << "for x = " << x_fine[i] << " expected " << f_fine[i] << " but got " << val;
        }
    }
    return testing::AssertionSuccess();
}



template <typename T>
testing::AssertionResult Interp2DAssertions(const Vector &x, const Vector &y, const Vector2 &f, const Vector &x_fine, const Vector &y_fine, const Vector2 &f_fine) {
    T interp2(x, y, f);
    for ( auto i = 0; i < x_fine.size(); ++i ) {
        for ( auto j = 0; j < y_fine.size(); ++j ) {
            auto val = interp2.eval(x_fine[i], y_fine[j]);
            if (!testing::internal::DoubleNearPredFormat("expected", "actual", "tolerance", val, f_fine[i][j], TOLERANCE)  ) {
                return testing::AssertionFailure()
                    << "for x = " << x_fine[i] << ", y = " << y_fine[j] << " expected " << f_fine[i][j] << " but got " << val;
            }
        }
    }
    return testing::AssertionSuccess();
}


template <typename T>
testing::AssertionResult Interp3DAssertions(const Vector &x, const Vector &y, const Vector &z, const Vector3 &f, const Vector &x_fine, const Vector &y_fine, const Vector &z_fine, const Vector3 &f_fine) {
    T interp3(x, y, z, f);
    for ( auto i = 0; i < x_fine.size(); ++i ) {
        for ( auto j = 0; j < y_fine.size(); ++j ) {
            for ( auto k = 0; k < y_fine.size(); ++k ) {
                auto val = interp3.eval(x_fine[i], y_fine[j], z_fine[k]);
                if (!testing::internal::DoubleNearPredFormat("expected", "actual", "tolerance", val, f_fine[i][j][k], TOLERANCE)  ) {
                    return testing::AssertionFailure()
                        << "for x = " << x_fine[i] << ", y = " << y_fine[j] << ", z = " << z_fine[j] << " expected " << f_fine[i][j][k] << " but got " << val;
                }
            }
        }
    }
    return testing::AssertionSuccess();
}


} // namespace cip
