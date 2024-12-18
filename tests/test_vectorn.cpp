#include <vector>
#include <gtest/gtest.h>
#include <utility>
#include "vectorn.hpp"

TEST(TestVectorN, test_vectorn)
{
    using Vector = std::vector<double>;
    using Vector2 = std::vector<std::vector<double>>;
    using Vector3 = std::vector<std::vector<std::vector<double>>>;
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
    vec::VectorN<double, 3> vec1(nvec1);
    ASSERT_EQ(vec1(0, 0, 0), 1);
    ASSERT_EQ(vec1(1, 1, 2), 12);

    Vector2 nvec2 = {{1, 2, 3}, {4, 5, 6}};
    vec::VectorN<double, 2> vec2(nvec2);
    ASSERT_EQ(vec2(0, 1), 2);
    vec2(0, 1) = 10;
    ASSERT_EQ(vec2(0, 1), 10);
    
    Vector nvec3 = {1, 2, 3, 4, 5};
    vec::VectorN<double, 1> vec3(nvec3);
    ASSERT_EQ(vec3(0), 1);
    ASSERT_EQ(vec3(1), 2);
    ASSERT_EQ(vec3(2), 3);
    ASSERT_EQ(vec3(3), 4);
    ASSERT_EQ(vec3(4), 5);
}

TEST(TestVectorNMdspan, test_vectorn_mdspan)
{
    using Vector3 = std::vector<std::vector<std::vector<double>>>;
    using Pair = std::pair<std::size_t, std::size_t>;

    Vector3 vec1 = {
                    {
                        {1, 2, 3},
                        {4, 5, 6}
                        },
                    {
                        {7, 8, 9},
                        {10, 11, 12}
                        }
                    };
    vec::VectorN<double, 3> nvec1(vec1);

    auto view = nvec1.mdspan();
    ASSERT_EQ(view(0, 0, 0), 1);
    ASSERT_EQ(view(1, 1, 2), 12);

    auto view2 = nvec1.submdspan(Pair{1, 2}, Pair{0, 1}, Pair{1, 2});
    ASSERT_EQ(view2(0, 0, 0), 8);
    ASSERT_EQ(view2(0, 0, 1), 9);
    ASSERT_EQ(view2(0, 1, 0), 11);
    ASSERT_EQ(view2(0, 1, 1), 12);
}
