# cubinterpp

Cubic interpolation with C++

## Introduction

This C++ header library features tools for piecewise cubic interpolation.

Currently, ony 1D interpolation is supported, however, future released are planned to extend the library to higher dimensions.

The library features three kinds of different interpolation types

- Monotone cubic interpolation
- Akima spline interpolation 
- Natural cubic spline interpolation

All classes are templatized and support the STL's vector as well as Eigen
vector types.

The following [example](#example) compares the three interpolation types.

![image](https://user-images.githubusercontent.com/5203275/211158056-08ae2f59-2a00-4e01-b336-0dd763da936b.png)

## Prerequisites

- C++ compiler
- [Eigen](https://eigen.tuxfamily.org): to be able to use Eigen vector
- [cmake](https://cmake.org/): to use the provided cmake configuration
- [pybind11](https://github.com/pybind/pybind11): to compile the example's python module
- [mlpyqtgraph](https://github.com/swvanbuuren/mlpyqtgraph): to plot the example's results

## Example

An [example](examples/main.py) is provided to compare the three interpolation types. Data
preparation and visualization is done in python with
[mlpyqtgraph](https://github.com/swvanbuuren/mlpyqtgraph).

The easiest way to test out `cubinterpp`, is to use cmake to compile and test the example.
An appropriate cmake configuration is provided. Prior to compilation, the required external
libraries need to placed into the [extern](extern/) directory:
- Eigen: the directory and its contents [Eigen](https://gitlab.com/libeigen/eigen/-/tree/master/Eigen)
- pybind11: the contents of the [complete repository](https://github.com/pybind/pybind11)
