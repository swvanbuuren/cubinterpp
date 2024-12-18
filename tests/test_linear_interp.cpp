#include <vector>
#include <gtest/gtest.h>
#include <utility>
#include "linear_interp.hpp"

using Vector = std::vector<double>;
using Vector2 = std::vector<std::vector<double>>;
using VectorN2 = vec::VectorN<double, 2>;
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
    size_t i = 2;
    size_t j = 2;
    lns::LinearCell2D<double> cell(
        Span(&x[i], 2), 
        Span(&y[j], 2), 
        fvec.submdspan(Pr{i, i+1}, Pr{j, j+1})
    );
    std::cout << "cell(0.5, 0.5) = " << cell.eval(0.5, 0.5) << std::endl;
}


TEST(TestInterp2D, test_linear_interp_2d) {
    Vector x = {0, 1, 2};
    Vector y = {0, 1, 2};
    Vector2 f = {{1, 2, 2},
                 {2, 3, 3},
                 {3, 3, 4}};

    VectorN2 fvec(f);
    lns::LinearInterp2D<double> interp(x, y, f);    
}