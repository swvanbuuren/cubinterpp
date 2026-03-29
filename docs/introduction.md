# Introduction

This C++ header library features tools for piecewise linear and cubic
interpolation.

For cubic piecewise interpolation, the library features three types:

- Monotone cubic interpolation
- Modified Akima spline interpolation 
- Natural cubic spline interpolation

Linear interpolation is supported for `N`-dimensional data. Cubic interpolation
supports `1`-, `2`- and `3`-dimensional data out of the box, and can be extended
to higher dimensions by inheriting from the `N`-dimensional base class.

All classes are templatized and support the STL's vector types.

The accompanying python script `main.py` in [cubinterpp
](https://github.com/swvanbuuren/cubinterpp/tree/master/cubinterpp) compares the
interpolation types.

The following figure features a comparison of 1D interpolation types that are supported by cubinterpp.

![Comparison of 1D interpolation types](images/comparison_1D.svg)
