#include <vector>
#include "vectorn.hpp"
#include <iostream>
#include <utility>

int main() {
    using Vector = std::vector<double>;
    using Vector2 = std::vector<std::vector<double>>;
    using Vector3 = std::vector<std::vector<std::vector<double>>>;
    // Test 1: Create a 3D VectorN and access elements
    Vector3 nvec1 = {
                    {
                        {1, 2, 3},
                        {4, 5, 6}
                        },
                    {
                        {7, 8, 9},
                        {10, 11, 12}
                        }
                    };
    VectorN<double, 3> vec1(nvec1); //, {2, 2, 3}
    std::cout << "vec1(0, 0, 0) = " << vec1(0, 0, 0) << std::endl;
    std::cout << "vec1(1, 1, 2) = " << vec1(1, 1, 2) << std::endl;

    // Test 2: Create a 2D VectorN and modify elements
    Vector2 nvec2 = {{1, 2, 3}, {4, 5, 6}};
    VectorN<double, 2> vec2(nvec2);
    vec2(0, 1) = 10;
    std::cout << "vec2(0, 1) = " << vec2(0, 1) << std::endl;

    // Test 3: Create a 1D VectorN and iterate over elements
    Vector nvec3 = {1, 2, 3, 4, 5};
    VectorN<double, 1> vec3(nvec3);
    for (int i = 0; i < 5; ++i) {
        std::cout << "vec3(" << i << ") = " << vec3(i) << std::endl;
    }

    // Test 5: Create an mdspan view of a subset of a 3D VectorN
    Vector3 nvec4 = {{{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}};
    VectorN<double, 3> vec4(nvec4);
    auto view = vec4.mdspan();
    std::cout << "view(0, 0, 0) = " << view(0, 0, 0) << std::endl;
    std::cout << "view(0, 0, 1) = " << view(0, 0, 1) << std::endl;
    std::cout << "view(1, 1, 2) = " << view(1, 1, 2) << std::endl;

    using pr = std::pair<std::size_t, std::size_t>;
    auto view2 = vec4.submdspan(pr{1, 2}, pr{0, 1}, pr{1, 2});

    std::cout << "view2(0, 0, 0) = " << view2(0, 0, 0) << std::endl;
    std::cout << "view2(0, 0, 1) = " << view2(0, 0, 1) << std::endl;
    std::cout << "view2(0, 1, 0) = " << view2(0, 1, 0) << std::endl;
    std::cout << "view2(0, 1, 1) = " << view2(0, 1, 1) << std::endl;

    return 0;
}
