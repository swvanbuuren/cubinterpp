#include <vector>
#include <gtest/gtest.h>
#include <utility>
#include "vectorn.hpp"

using Vector = std::vector<double>;
using Vector2 = std::vector<std::vector<double>>;
using Vector3 = std::vector<std::vector<std::vector<double>>>;

template<class T>unsigned char identify(T&& v) {return v;}


TEST(TestVectorN, test_vectorn)
{
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
    cip::VectorN<double, 3> vec1(nvec1);
    ASSERT_EQ(vec1(0, 0, 0), 1);
    ASSERT_EQ(vec1(1, 1, 2), 12);

    Vector2 nvec2 = {{1, 2, 3}, {4, 5, 6}};
    cip::VectorN<double, 2> vec2(nvec2);
    ASSERT_EQ(vec2(0, 1), 2);
    vec2(0, 1) = 10;
    ASSERT_EQ(vec2(0, 1), 10);
    
    Vector nvec3 = {1, 2, 3, 4, 5};
    cip::VectorN<double, 1> vec3(nvec3);
    ASSERT_EQ(vec3(0), 1);
    ASSERT_EQ(vec3(1), 2);
    ASSERT_EQ(vec3(2), 3);
    ASSERT_EQ(vec3(3), 4);
    ASSERT_EQ(vec3(4), 5);

    cip::VectorN<double, 1> vec4(vec3);
    ASSERT_EQ(vec3(0), 1);
    ASSERT_EQ(vec3(1), 2);
    ASSERT_EQ(vec3(2), 3);
    ASSERT_EQ(vec3(3), 4);
    ASSERT_EQ(vec3(4), 5);

}

using Pair = std::pair<std::size_t, std::size_t>;
using Mdspan = std::mdspan<const double, std::dextents<std::size_t, 2>, std::layout_stride>;

TEST(TestSubmdspan, test_submdspan)
{
    Vector vec = {1, 2, 2, 2, 3, 3, 3, 3, 4};

    //Mdspan33 view = std::mdspan<const double, std::extents<std::size_t, 3, 3>>(vec.data(), 3, 3);
    Mdspan view = std::mdspan(vec.data(), 3, 3);
    ASSERT_EQ(view(0, 0), 1);
    ASSERT_EQ(view(1, 1), 3);

    Mdspan subview = std::submdspan(view, Pair{0, 2}, Pair{0, 2});
    ASSERT_EQ(subview(0, 0), 1);
    ASSERT_EQ(subview(0, 1), 2);
    ASSERT_EQ(subview(0, 2), 2);
    ASSERT_EQ(subview(1, 0), 2);
    ASSERT_EQ(subview(1, 1), 3);
    ASSERT_EQ(subview(1, 2), 3);
    ASSERT_EQ(subview(2, 0), 3);
    ASSERT_EQ(subview(2, 1), 3);
    ASSERT_EQ(subview(2, 2), 4);
   
}


TEST(TestVectorNMdspan, test_vectorn_mdspan)
{
    Vector2 vec2 = {
                   {1, 2, 2},
                   {2, 3, 3},
                   {3, 3, 4}
                   };

    cip::VectorN<double, 2> nvec2(vec2);

    auto view2 = nvec2.get_mdspan();
    ASSERT_EQ(view2(0, 0), 1);
    ASSERT_EQ(view2(1, 1), 3);

    auto subview2 = nvec2.submdspan(Pair{0, 1}, Pair{0, 1});
    ASSERT_EQ(subview2(0, 0), 1);
    ASSERT_EQ(subview2(0, 1), 2);
    ASSERT_EQ(subview2(1, 0), 2);
    ASSERT_EQ(subview2(1, 1), 3);

    cip::VectorN<double, 2> nvec2a(nvec2);

    auto view2a = nvec2a.get_mdspan();
    ASSERT_EQ(view2a(0, 0), 1);
    ASSERT_EQ(view2a(1, 1), 3);

    auto subview2a = nvec2a.submdspan(Pair{0, 1}, Pair{0, 1});
    ASSERT_EQ(subview2a(0, 0), 1);
    ASSERT_EQ(subview2a(0, 1), 2);
    ASSERT_EQ(subview2a(1, 0), 2);
    ASSERT_EQ(subview2a(1, 1), 3);

    Vector3 vec3 = {
                    {
                        {1, 2, 3},
                        {4, 5, 6}
                        },
                    {
                        {7, 8, 9},
                        {10, 11, 12}
                        }
                    };
    cip::VectorN<double, 3> nvec3(vec3);

    auto view3 = nvec3.get_mdspan();
    ASSERT_EQ(view3(0, 0, 0), 1);
    ASSERT_EQ(view3(1, 1, 2), 12);

    auto subview3 = nvec3.submdspan(Pair{1, 2}, Pair{0, 1}, Pair{1, 2});
    ASSERT_EQ(subview3(0, 0, 0), 8);
    ASSERT_EQ(subview3(0, 0, 1), 9);
    ASSERT_EQ(subview3(0, 1, 0), 11);
    ASSERT_EQ(subview3(0, 1, 1), 12);

    cip::VectorN<double, 3> nvec3a(nvec3);

    auto view3a = nvec3a.get_mdspan();
    ASSERT_EQ(view3a(0, 0, 0), 1);
    ASSERT_EQ(view3a(1, 1, 2), 12);

    auto subview3a = nvec3a.submdspan(Pair{1, 2}, Pair{0, 1}, Pair{1, 2});
    ASSERT_EQ(subview3a(0, 0, 0), 8);
    ASSERT_EQ(subview3a(0, 0, 1), 9);
    ASSERT_EQ(subview3a(0, 1, 0), 11);
    ASSERT_EQ(subview3a(0, 1, 1), 12);
}


TEST(TestVectorPushBack, test_vector_push_back)
{
    cip::VectorN<int, 3> vec({2, 3, 4}); // A 3D array with dimensions 2x3x4
    int counter = 0;

    // Fill the vector
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            for (size_t k = 0; k < 4; ++k) {
                vec.push_back(counter++);
            }
        }
    }
    ASSERT_EQ(vec(1, 2, 3), 23);

    // Create a submdspan
    auto sub = vec.submdspan(std::pair{0, 1}, std::pair{1, 3}, std::pair{0, 2});
    ASSERT_EQ(sub(0, 0, 0), 4);
}

TEST(TestVectorEmplaceBack, test_vector_emplace_back)
{
    
    Vector2 vec2 = {
                   {1, 2, 2},
                   {2, 3, 3},
                   {3, 3, 4}
                   };
    
    cip::VectorN<double, 2> vecn2({3, 3}); // A 3D array with dimensions 2x3x4
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            vecn2.emplace_back(vec2[i][j]);
        }
    }
    ASSERT_EQ(vecn2(0, 0), 1);
    ASSERT_EQ(vecn2(0, 1), 2);
    ASSERT_EQ(vecn2(0, 2), 2);
    ASSERT_EQ(vecn2(1, 0), 2);
    ASSERT_EQ(vecn2(1, 1), 3);
    ASSERT_EQ(vecn2(1, 2), 3);
    ASSERT_EQ(vecn2(2, 0), 3);
    ASSERT_EQ(vecn2(2, 1), 3);
    ASSERT_EQ(vecn2(2, 2), 4);

    cip::VectorN<int, 3> vec({2, 3, 4}); // A 3D array with dimensions 2x3x4
    int counter = 0;

    // Fill the vector
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            for (size_t k = 0; k < 4; ++k) {
                vec.emplace_back(counter++);
            }
        }
    }
    ASSERT_EQ(vec(1, 2, 3), 23);

    // Create a submdspan
    auto sub = vec.submdspan(std::pair{0, 1}, std::pair{1, 3}, std::pair{0, 2});
    ASSERT_EQ(sub(0, 0, 0), 4);
}
