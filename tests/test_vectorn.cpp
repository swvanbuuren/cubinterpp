#include <vector>
#include <gtest/gtest.h>
#include <utility>
#include <cubinterpp.hpp>

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

TEST(TestVectorN, test_submdspan)
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


TEST(TestVectorN, test_vectorn_mdspan)
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


TEST(TestVectorN, test_vector_emplace_back)
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


// Test that move_into_submdspan correctly moves a 1D vector into a 1D subview.
// For a 2D VectorN (dimensions: 2 rows x 3 columns), we move new values into row 0.
TEST(TestVectorN, MoveIntoSubmdspanValid) {
    // Create a 2D VectorN with 2 rows and 3 columns, all elements initialized to 0.0.
    std::array<std::size_t, 2> dims = {2, 3};
    cip::VectorN<double, 2> vec(0.0, dims);

    // Prepare a 1D vector of three elements that we want to move into the first row.
    std::vector<double> rowData = {10.0, 20.0, 30.0};

    // Use move_into_submdspan to move the elements into row 0.
    // Here the slice specifiers are: fixed row index 0, and full_extent for the column dimension.
    EXPECT_NO_THROW({
        vec.move_into_submdspan(std::move(rowData), 0, std::full_extent);
    });

    // Verify that the first row now contains the new values.
    EXPECT_DOUBLE_EQ(vec(0, 0), 10.0);
    EXPECT_DOUBLE_EQ(vec(0, 1), 20.0);
    EXPECT_DOUBLE_EQ(vec(0, 2), 30.0);
}


// Test that move_into_submdspan throws an error when the size of the source vector
// does not match the extent of the subview.
TEST(TestVectorN, MoveIntoSubmdspanSizeMismatch) {
    // Create a 2D VectorN with 2 rows x 3 columns.
    std::array<std::size_t, 2> dims = {2, 3};
    cip::VectorN<double, 2> vec(0.0, dims);

    // Create a source vector with the wrong number of elements (only 2 instead of 3).
    std::vector<double> wrongRow = {1.0, 2.0};

    // Expect that calling move_into_submdspan throws a std::runtime_error.
    EXPECT_THROW({
        vec.move_into_submdspan(std::move(wrongRow), 1, std::full_extent);
    }, std::runtime_error);
}


TEST(TestVectorN, InsertRowIn3D_Dim1_Explicit) {
    std::array<std::size_t, 3> dims = {3, 4, 5};
    cip::VectorN<double, 3> vec(0.0, dims);

    std::vector<double> rowData = {1.1, 2.2, 3.3, 4.4};

    // Slice: fix dim0 = 1, leave dim1 dynamic, fix dim2 = 3.
    EXPECT_NO_THROW({
        vec.move_into_submdspan(std::move(rowData), 1, std::full_extent, 3);
    });

    // Check each element explicitly.
    EXPECT_DOUBLE_EQ(vec(1, 0, 3), 1.1);
    EXPECT_DOUBLE_EQ(vec(1, 1, 3), 2.2);
    EXPECT_DOUBLE_EQ(vec(1, 2, 3), 3.3);
    EXPECT_DOUBLE_EQ(vec(1, 3, 3), 4.4);
}


TEST(TestVectorN, InsertRowIn3D_Dim0_Explicit) {
    std::array<std::size_t, 3> dims = {3, 4, 5};
    cip::VectorN<double, 3> vec(0.0, dims);

    std::vector<double> rowData = {10.0, 20.0, 30.0};

    // Slice: fix dim1 = 2, fix dim2 = 4, leave dim0 dynamic.
    EXPECT_NO_THROW({
        vec.move_into_submdspan(std::move(rowData), std::full_extent, 2, 4);
    });

    // Explicit checks.
    EXPECT_DOUBLE_EQ(vec(0, 2, 4), 10.0);
    EXPECT_DOUBLE_EQ(vec(1, 2, 4), 20.0);
    EXPECT_DOUBLE_EQ(vec(2, 2, 4), 30.0);
}


TEST(TestVectorN, InsertRowIn4D_Dim2_Explicit) {
    std::array<std::size_t, 4> dims = {2, 3, 4, 5};
    cip::VectorN<double, 4> vec(0.0, dims);

    std::vector<double> rowData = {100.0, 200.0, 300.0, 400.0};

    // Slice: fix dim0 = 1, fix dim1 = 2, leave dim2 dynamic, fix dim3 = 4.
    EXPECT_NO_THROW({
        vec.move_into_submdspan(std::move(rowData), 1, 2, std::full_extent, 4);
    });

    // Explicit checks for dimension 2.
    EXPECT_DOUBLE_EQ(vec(1, 2, 0, 4), 100.0);
    EXPECT_DOUBLE_EQ(vec(1, 2, 1, 4), 200.0);
    EXPECT_DOUBLE_EQ(vec(1, 2, 2, 4), 300.0);
    EXPECT_DOUBLE_EQ(vec(1, 2, 3, 4), 400.0);
}


TEST(TestVectorN, InsertRowIn4D_Dim0_Explicit) {
    std::array<std::size_t, 4> dims = {2, 3, 4, 5};
    cip::VectorN<double, 4> vec(0.0, dims);

    std::vector<double> rowData = {55.5, 66.6};

    // Slice: fix dim1 = 1, fix dim2 = 2, fix dim3 = 3, leave dim0 dynamic.
    EXPECT_NO_THROW({
        vec.move_into_submdspan(std::move(rowData), std::full_extent, 1, 2, 3);
    });

    // Explicitly check both entries along dimension 0.
    EXPECT_DOUBLE_EQ(vec(0, 1, 2, 3), 55.5);
    EXPECT_DOUBLE_EQ(vec(1, 1, 2, 3), 66.6);
}


// ----------------------------------------------------------------
// Test 1: Move a 1D VectorN into a 2D VectorN.
// Target: 2D with dimensions {5, 3}.
// Source: 1D with dimensions {5}.
// We use slice specifiers (full_extent, 0) to select column 0.
// After the move, target(i, 0) should equal the source element at i.
// ----------------------------------------------------------------
TEST(MoveIntoSubmdspanOverloadTest, Move1Dinto2D) {
    // Create target 2D VectorN with dimensions {5, 3} initialized to 0.
    cip::VectorN<double, 2> target(0.0, std::array<std::size_t, 2>{5, 3});
    
    // Create source 1D VectorN with dimension {5} and fill with 1,2,3,4,5.
    cip::VectorN<double, 1> source(0.0, std::array<std::size_t, 1>{5});
    for (std::size_t i = 0; i < 5; ++i) {
        source(i) = static_cast<double>(i + 1);
    }
    
    // Move source into target column 0: slice specifiers (full_extent, 0).
    EXPECT_NO_THROW({
        target.move_into_submdspan(std::move(source), std::full_extent, 0);
    });
    
    // Check each element in column 0.
    EXPECT_DOUBLE_EQ(target(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(target(1, 0), 2.0);
    EXPECT_DOUBLE_EQ(target(2, 0), 3.0);
    EXPECT_DOUBLE_EQ(target(3, 0), 4.0);
    EXPECT_DOUBLE_EQ(target(4, 0), 5.0);
}

// ----------------------------------------------------------------
// Test 2: Move a 2D VectorN into a 3D VectorN.
// Target: 3D with dimensions {2, 3, 4}.
// Source: 2D with dimensions {2, 3}.
// We choose the subview by fixing the third dimension to 2:
// slice specifiers: (full_extent, full_extent, 2)
// Then target(i,j,2) should equal source(i,j).
// ----------------------------------------------------------------
TEST(MoveIntoSubmdspanOverloadTest, Move2Dinto3D) {
    // Create target 3D VectorN with dimensions {2, 3, 4}.
    cip::VectorN<double, 3> target(0.0, std::array<std::size_t, 3>{2, 3, 4});
    
    // Create source 2D VectorN with dimensions {2, 3}.
    cip::VectorN<double, 2> source(0.0, std::array<std::size_t, 2>{2, 3});
    // Fill source with distinct values.
    source(0, 0) = 0.0;
    source(0, 1) = 1.0;
    source(0, 2) = 2.0;
    source(1, 0) = 10.0;
    source(1, 1) = 11.0;
    source(1, 2) = 12.0;
    
    // Move source into target subview at fixed third index = 2.
    EXPECT_NO_THROW({
        target.move_into_submdspan(std::move(source), std::full_extent, std::full_extent, 2);
    });
    
    // Check explicit values.
    EXPECT_DOUBLE_EQ(target(0, 0, 2), 0.0);
    EXPECT_DOUBLE_EQ(target(0, 1, 2), 1.0);
    EXPECT_DOUBLE_EQ(target(0, 2, 2), 2.0);
    EXPECT_DOUBLE_EQ(target(1, 0, 2), 10.0);
    EXPECT_DOUBLE_EQ(target(1, 1, 2), 11.0);
    EXPECT_DOUBLE_EQ(target(1, 2, 2), 12.0);
}

// ----------------------------------------------------------------
// Test 3: Move a 2D VectorN into a 4D VectorN.
// Target: 4D with dimensions {3, 4, 2, 5}.
// Source: 2D with dimensions {3, 4}.
// We select a subview by fixing dimensions 3 and 4 to 0 and 2 respectively,
// i.e. slice specifiers: (full_extent, full_extent, 0, 2).
// After the move, target(i,j,0,2) should equal source(i,j).
// ----------------------------------------------------------------
TEST(MoveIntoSubmdspanOverloadTest, Move2Dinto4D) {
    // Create target 4D VectorN with dimensions {3, 4, 2, 5}.
    cip::VectorN<double, 4> target(0.0, std::array<std::size_t, 4>{3, 4, 2, 5});
    
    // Create source 2D VectorN with dimensions {3, 4}.
    cip::VectorN<double, 2> source(0.0, std::array<std::size_t, 2>{3, 4});
    // Fill source: value = i*100 + j.
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            source(i, j) = static_cast<double>(i * 100 + j);
        }
    }
    
    // Move source into target subview: fix dimensions 3 = 0 and 4 = 2.
    // Slice specifiers: (full_extent, full_extent, 0, 2).
    EXPECT_NO_THROW({
        target.move_into_submdspan(std::move(source), std::full_extent, std::full_extent, 0, 2);
    });
    
    // Check explicit values.
    EXPECT_DOUBLE_EQ(target(0, 0, 0, 2), 0.0);
    EXPECT_DOUBLE_EQ(target(0, 1, 0, 2), 1.0);
    EXPECT_DOUBLE_EQ(target(0, 2, 0, 2), 2.0);
    EXPECT_DOUBLE_EQ(target(0, 3, 0, 2), 3.0);
    
    EXPECT_DOUBLE_EQ(target(1, 0, 0, 2), 100.0);
    EXPECT_DOUBLE_EQ(target(1, 1, 0, 2), 101.0);
    EXPECT_DOUBLE_EQ(target(1, 2, 0, 2), 102.0);
    EXPECT_DOUBLE_EQ(target(1, 3, 0, 2), 103.0);
    
    EXPECT_DOUBLE_EQ(target(2, 0, 0, 2), 200.0);
    EXPECT_DOUBLE_EQ(target(2, 1, 0, 2), 201.0);
    EXPECT_DOUBLE_EQ(target(2, 2, 0, 2), 202.0);
    EXPECT_DOUBLE_EQ(target(2, 3, 0, 2), 203.0);
}

// ----------------------------------------------------------------
// Test 4: Check that a size mismatch throws an exception.
// For example, if the source 2D VectorN has fewer elements than the target subview,
// the move_into_submdspan should throw.
// ----------------------------------------------------------------
TEST(MoveIntoSubmdspanOverloadTest, SizeMismatchThrows) {
    // Create a target 3D VectorN with dimensions {2, 3, 4} (total elements for subview = 2*3 = 6).
    cip::VectorN<double, 3> target(0.0, std::array<std::size_t, 3>{2, 3, 4});
    
    // Create a source 2D VectorN with dimensions {2, 2} (total elements = 4).
    cip::VectorN<double, 2> source(0.0, std::array<std::size_t, 2>{2, 2});
    
    // Attempt to move source into a subview of target that expects 6 elements.
    EXPECT_THROW({
        target.move_into_submdspan(std::move(source), std::full_extent, std::full_extent, 1);
    }, std::runtime_error);
}
