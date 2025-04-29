#include <vector>
#include <gtest/gtest.h>
#include <array>
#include <tuple>
#include <type_traits>
#include <sstream>
#include <functional>
#include <utility>

namespace cip {

using Vector = std::vector<double>;
using Vector2 = std::vector<Vector>;
using Vector3 = std::vector<Vector2>;
using Vector4 = std::vector<Vector3>;

constexpr double TOLERANCE_DEFAULT = 5.0e-12;

// Helper class for N-dimensional interpolation assertions
template <typename T, size_t N, typename FuncValT>
class InterpNDAssertions {
private:
    // Helper to access nested vector value at specific indices
    template <typename VecT>
    static double get_value(const VecT& val) {
        return val;
    }
    
    template <typename VecT, typename... Indices>
    static double get_value(const VecT& vec, size_t idx, Indices... rest) {
        return get_value(vec[idx], rest...);
    }
    
    // Helper for dimension name (x, y, z, w, etc.)
    static constexpr char dim_name(size_t i) {
        return i < 3 ? 'x' + i : 'w' + (i - 3);
    }
    
    // Recursive template to build nested loops over N dimensions
    template <size_t Dim>
    static testing::AssertionResult check_points(
            const T& interp,
            const std::array<Vector, N>& fine_grid_vectors,
            const FuncValT& f_fine,
            std::array<size_t, N>& indices,
            std::array<double, N>& coords,
            double tol) {
        
        // Base case: we've set up all N dimension indices
        if constexpr (Dim == N) {
            // Call eval with the coordinates using apply
            double val = std::apply([&interp](auto... args) { 
                return interp.eval(args...); 
            }, coords);
            
            // Get expected value from nested vector using indices
            double expected = std::apply([&f_fine](auto... args) { 
                return get_value(f_fine, args...); 
            }, indices);
            
            // Check if the values match within tolerance
            if (!testing::internal::DoubleNearPredFormat("expected", "actual", "tolerance", val, expected, tol)) {
                testing::AssertionResult failure = testing::AssertionFailure();
                
                // Format dimensions for error message (x=1.23, y=4.56, etc.)
                for (size_t i = 0; i < N; ++i) {
                    failure << (i == 0 ? "for " : ", ") << dim_name(i) << " = " << coords[i];
                }
                
                failure << " expected " << expected << " but got " << val;
                return failure;
            }
            
            return testing::AssertionSuccess();
        }
        // Recursive case: loop over the current dimension
        else {
            for (size_t i = 0; i < fine_grid_vectors[Dim].size(); ++i) {
                indices[Dim] = i;
                coords[Dim] = fine_grid_vectors[Dim][i];
                
                // Recursively process the next dimension
                auto result = check_points<Dim + 1>(interp, fine_grid_vectors, f_fine, indices, coords, tol);
                if (!result) {
                    return result;
                }
            }
            return testing::AssertionSuccess();
        }
    }

public:
    // Main assertion method
    static testing::AssertionResult test(
            const std::array<Vector, N>& grid_vectors,
            const FuncValT& f,
            const std::array<Vector, N>& fine_grid_vectors,
            const FuncValT& f_fine,
            double tol = TOLERANCE_DEFAULT) {
        
        // Create the interpolator with a simple lambda + std::apply
        T interp = std::apply([&f](const auto&... grid_args) {
            return T(grid_args..., f);
        }, grid_vectors);
        
        // Set up indices and coordinates arrays
        std::array<size_t, N> indices{};
        std::array<double, N> coords{};
        
        // Start the recursive dimension traversal
        return check_points<0>(interp, fine_grid_vectors, f_fine, indices, coords, tol);
    }
};

// Convenience wrapper functions to maintain backward compatibility
template <typename T>
testing::AssertionResult Interp1DAssertions(Vector x, Vector f, Vector x_fine, Vector f_fine, double tol=TOLERANCE_DEFAULT) {
    std::array<Vector, 1> grid_vectors = {x};
    std::array<Vector, 1> fine_grid_vectors = {x_fine};
    return InterpNDAssertions<T, 1, Vector>::test(grid_vectors, f, fine_grid_vectors, f_fine, tol);
}

template <typename T>
testing::AssertionResult Interp2DAssertions(const Vector &x, const Vector &y, const Vector2 &f, const Vector &x_fine, const Vector &y_fine, const Vector2 &f_fine, double tol=TOLERANCE_DEFAULT) {
    std::array<Vector, 2> grid_vectors = {x, y};
    std::array<Vector, 2> fine_grid_vectors = {x_fine, y_fine};
    return InterpNDAssertions<T, 2, Vector2>::test(grid_vectors, f, fine_grid_vectors, f_fine, tol);
}

template <typename T>
testing::AssertionResult Interp3DAssertions(const Vector &x, const Vector &y, const Vector &z, const Vector3 &f, const Vector &x_fine, const Vector &y_fine, const Vector &z_fine, const Vector3 &f_fine, double tol=TOLERANCE_DEFAULT) {
    std::array<Vector, 3> grid_vectors = {x, y, z};
    std::array<Vector, 3> fine_grid_vectors = {x_fine, y_fine, z_fine};
    return InterpNDAssertions<T, 3, Vector3>::test(grid_vectors, f, fine_grid_vectors, f_fine, tol);
}

template <typename T>
testing::AssertionResult Interp4DAssertions(const Vector &x, const Vector &y, const Vector &z, const Vector &w, 
                                           const Vector4 &f, const Vector &x_fine, const Vector &y_fine, 
                                           const Vector &z_fine, const Vector &w_fine, const Vector4 &f_fine, 
                                           double tol=TOLERANCE_DEFAULT) {
    std::array<Vector, 4> grid_vectors = {x, y, z, w};
    std::array<Vector, 4> fine_grid_vectors = {x_fine, y_fine, z_fine, w_fine};
    return InterpNDAssertions<T, 4, Vector4>::test(grid_vectors, f, fine_grid_vectors, f_fine, tol);
}

} // namespace cip
