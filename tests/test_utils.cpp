#include <gtest/gtest.h>
#include <cubinterpp.hpp>


// ---------------------------------------------------------------------------
// Typed aliases
// ---------------------------------------------------------------------------
using BinarySearchIndexer = cip::Indexer<double, cip::IndexMethod::BinarySearch>;
using UniformIndexer      = cip::Indexer<double, cip::IndexMethod::Uniform>;


// ===========================================================================
// BinarySearch indexer — equidistant data {0, 1, 2, 3, 4}
//   index_front = 0, index_back = 3, x_delta = 1.0
// ===========================================================================
TEST(IndexerSortedEquidistant, AllCases)
{
    std::vector<double> x{0.0, 1.0, 2.0, 3.0, 4.0};
    BinarySearchIndexer idx(x);

    EXPECT_EQ(idx.index(-1.0), 0u);  // below front
    EXPECT_EQ(idx.index(5.0),  3u);  // above back
    EXPECT_EQ(idx.index(0.0),  0u);  // exact front
    EXPECT_EQ(idx.index(4.0),  3u);  // exact back — xi >= x_back clamps to index_back
    EXPECT_EQ(idx.index(0.5),  0u);  // first interval
    EXPECT_EQ(idx.index(2.5),  2u);  // middle interval
    EXPECT_EQ(idx.index(3.7),  3u);  // last interval
    EXPECT_EQ(idx.index(1.0),  1u);  // exact at x[1] → [x[1], x[2]) → index 1
    EXPECT_EQ(idx.index(2.0),  2u);  // exact at x[2] → [x[2], x[3]) → index 2
    EXPECT_EQ(idx.index(3.0),  3u);  // exact at x[3] → [x[3], x[4]) → index 3
}


// ===========================================================================
// BinarySearch indexer — non-uniform data {1.0, 2.5, 3.0, 5.0, 10.0}
//   Intervals: [1,2.5), [2.5,3), [3,5), [5,10]
//   index_back = 3
// ===========================================================================
TEST(IndexerSortedNonUniform, AllCases)
{
    std::vector<double> x{1.0, 2.5, 3.0, 5.0, 10.0};
    BinarySearchIndexer idx(x);

    EXPECT_EQ(idx.index(0.0),  0u);  // below front
    EXPECT_EQ(idx.index(11.0), 3u);  // above back
    EXPECT_EQ(idx.index(1.0),  0u);  // exact front
    EXPECT_EQ(idx.index(10.0), 3u);  // exact back
    EXPECT_EQ(idx.index(1.5),  0u);  // first interval:  [1.0, 2.5)
    EXPECT_EQ(idx.index(4.0),  2u);  // middle interval: [3.0, 5.0)
    EXPECT_EQ(idx.index(7.0),  3u);  // last interval:   [5.0, 10.0)
    EXPECT_EQ(idx.index(2.5),  1u);  // exact at x[1]=2.5  → [x[1], x[2]) → index 1
    EXPECT_EQ(idx.index(3.0),  2u);  // exact at x[2]=3.0  → [x[2], x[3]) → index 2
    EXPECT_EQ(idx.index(5.0),  3u);  // exact at x[3]=5.0  → [x[3], x[4]) → index 3
    EXPECT_EQ(idx.index(2.7),  1u);  // narrow interval [2.5, 3.0), point just inside
}


// ===========================================================================
// Edge case: minimal two-element array {0.0, 5.0}
//   Only one interval; index_back = 0.  Every query must return 0.
// ===========================================================================
TEST(IndexerSortedMinimalArray, AllCases)
{
    std::vector<double> x{0.0, 5.0};
    BinarySearchIndexer idx(x);
    EXPECT_EQ(idx.index(-1.0), 0u);  // below front
    EXPECT_EQ(idx.index(10.0), 0u);  // above back
    EXPECT_EQ(idx.index(2.5),  0u);  // inside interval
}

TEST(IndexerCellMinimalArray, AllCases)
{
    std::vector<double> x{0.0, 5.0};
    UniformIndexer idx(x);
    EXPECT_EQ(idx.index(-1.0), 0u);  // below front
    EXPECT_EQ(idx.index(10.0), 0u);  // above back
    EXPECT_EQ(idx.index(2.5),  0u);  // inside interval
}


// ===========================================================================
// Edge case: array spanning negative values {-4, -2, 0, 2, 4}
//   Spacing = 2.0, index_back = 3
//   Exercises uniform_index with a negative x_front in (xi - x_front) / x_delta.
// ===========================================================================
TEST(IndexerCellNegativeSpan, AllCases)
{
    std::vector<double> x{-4.0, -2.0, 0.0, 2.0, 4.0};
    UniformIndexer idx(x);
    EXPECT_EQ(idx.index(-5.0), 0u);  // below front
    EXPECT_EQ(idx.index(6.0),  3u);  // above back
    EXPECT_EQ(idx.index(-3.0), 0u);  // first interval:  [-4, -2)
    EXPECT_EQ(idx.index(0.5),  2u);  // middle interval: [0, 2)
    EXPECT_EQ(idx.index(3.0),  3u);  // last interval:   [2, 4)
}


// ===========================================================================
// Edge case: float (single-precision) type
//   Ensures the template instantiates correctly for float.
// ===========================================================================
TEST(IndexerSortedFloat, BasicCases)
{
    std::vector<float> x{0.f, 1.f, 2.f, 3.f};
    cip::Indexer<float, cip::IndexMethod::BinarySearch> idx(x);

    EXPECT_EQ(idx.index(-1.f), 0u);   // below
    EXPECT_EQ(idx.index(3.f),  2u);   // exact back
    EXPECT_EQ(idx.index(0.5f), 0u);   // first interval
    EXPECT_EQ(idx.index(1.5f), 1u);   // middle interval
    EXPECT_EQ(idx.index(2.5f), 2u);   // last interval
}

TEST(IndexerCellFloat, BasicCases)
{
    std::vector<float> x{0.f, 1.f, 2.f, 3.f};
    cip::Indexer<float, cip::IndexMethod::Uniform> idx(x);

    EXPECT_EQ(idx.index(-1.f), 0u);
    EXPECT_EQ(idx.index(3.f),  2u);
    EXPECT_EQ(idx.index(0.5f), 0u);
    EXPECT_EQ(idx.index(1.5f), 1u);
    EXPECT_EQ(idx.index(2.5f), 2u);
}


// ===========================================================================
// Consistency: BinarySearch and Uniform must agree on equidistant data
// ===========================================================================
TEST(IndexerConsistency, SortedAndCellAgreeSweep)
{
    std::vector<double> x{0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    BinarySearchIndexer binary_search_idx(x);
    UniformIndexer       uniform_idx(x);

    // Probe many points across the domain including boundaries
    for (double xi = -0.5; xi <= 5.5; xi += 0.1) {
        EXPECT_EQ(binary_search_idx.index(xi), uniform_idx.index(xi))
            << "Mismatch at xi = " << xi;
    }
}
