#include <gtest/gtest.h>
#include <cubinterpp.hpp>


// Test a 1-dimensional NDHyperArray.
TEST(NDHyperArrayTest, OneDArray) {
    cip::NDHyperArray<int, 1, 4> arr;
    arr(0) = 10;
    arr(1) = 20;
    arr(2) = 30;
    arr(3) = 40;
    EXPECT_EQ(arr(0), 10);
    EXPECT_EQ(arr(1), 20);
    EXPECT_EQ(arr(2), 30);
    EXPECT_EQ(arr(3), 40);
}


// Test a 2-dimensional NDHyperArray.
TEST(NDHyperArrayTest, TwoDArray) {
    cip::NDHyperArray<int, 2, 4> arr;
    arr(0, 0) = 1;
    arr(0, 1) = 2;
    arr(1, 0) = 3;
    arr(3, 3) = 4;
    EXPECT_EQ(arr(0, 0), 1);
    EXPECT_EQ(arr(0, 1), 2);
    EXPECT_EQ(arr(1, 0), 3);
    EXPECT_EQ(arr(3, 3), 4);
}


// Test a 3-dimensional NDHyperArray.
TEST(NDHyperArrayTest, ThreeDArray) {
    cip::NDHyperArray<double, 3, 4> arr;
    arr(1, 2, 3) = 3.14159;
    arr(0, 0, 0) = 2.71828;
    EXPECT_NEAR(arr(1, 2, 3), 3.14159, 1e-6);
    EXPECT_NEAR(arr(0, 0, 0), 2.71828, 1e-6);
}


// Test const access to ensure the const overload of operator() works.
TEST(NDHyperArrayTest, ConstAccess) {
    cip::NDHyperArray<int, 2, 4> arr;
    arr(2, 2) = 99;
    const cip::NDHyperArray<int, 2, 4>& constArr = arr;
    EXPECT_EQ(constArr(2, 2), 99);
}
