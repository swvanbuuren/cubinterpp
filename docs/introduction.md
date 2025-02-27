# Introduction

This C++ header library features tools for piecewise linear and cubic
interpolation.

For cubic piecewise interpolation, the library features three types:

- Monotone cubic interpolation
- Akima spline interpolation 
- Natural cubic spline interpolation

Linear interpolation is supported for `N`-dimensional data, whereas cubic
interpolation currently only supports `1`- and `2`-dimensional data. Cubic
piecewise interpolation for `N`-dimensional data is planned.

All classes are templatized and support the STL's vector types.

The accompanying python script in [cubinterpp ](cubinterpp ) compares the
interpolation types.

## Data requirements

`cubinterpp` is designed to only handle rectangular grid data with strictly
ascending coordinates. To this end, coordinate data is only required in one
dimensional form for each coordinate direction. Only the actual data itself
needs to be supplied in the actual dimensions. The user is responsible to assure
that the supplied data size of each dimension fits to the coordinate data.
