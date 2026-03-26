# cubinterpp - ND Cubic interpolation in C++

`N`-dimensional cubic and linear interpolation in modern C++ with Python
support.

<p align="center">
  <img src="docs/images/animated_periodic_spline_2D.gif" alt="2D parametric periodic spline"/>
  <br/>
  <em>2D parametric periodic cubic spline interpolation of a distorted torus</em>
</p>

## Key features

This C++ header library features tools for piecewise linear and cubic
interpolation in `N` dimensions.

For cubic piecewise interpolation, the library features three types:

- Monotone cubic interpolation
- Modified Akima spline interpolation 
- Natural cubic spline interpolation, with the following boundary conditions:
  - Not-A-Knot (first and second cell at ends share same polynomial)
  - Natural (vanishing second derivative at ends)
  - Clamped (vanishing first derivative at ends)
  - Periodic (value, first and second derivative match for ends)

All classes are templatized and support the STL's vector types.

The accompanying python scripts in [cubinterpp ](cubinterpp) and [example
page](https://swvanbuuren.github.io/cubinterpp/examples) compare the interpolation types.

Refer to the [documentation](https://swvanbuuren.github.io/cubinterpp/) for:

- [Requirements](https://swvanbuuren.github.io/cubinterpp/requirements/)
- [Usage instructions](https://swvanbuuren.github.io/cubinterpp/usage/)
  - [Single header file](https://swvanbuuren.github.io/cubinterpp/usage#single-header-library)
  - [Build from source](https://swvanbuuren.github.io/cubinterpp/usage#build-from-source)
  - [Examples](https://swvanbuuren.github.io/cubinterpp/examples)
- [Mathematical background](https://swvanbuuren.github.io/cubinterpp/theory/)
- [Code reference](https://swvanbuuren.github.io/cubinterpp/reference/)

## Pre-commit hooks

This repository comes with pre-commit hooks. To enable the hooks issue:

```bash
uv run pre-commit install --install-hooks
```

## License

An MIT style license applies for cubinterpp, see the [LICENSE](LICENSE) file for
more details.
