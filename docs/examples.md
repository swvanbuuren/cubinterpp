# Examples

This page showcases cubinterpp interpolation examples in one and two dimensions, covering
different slope estimation methods and boundary conditions.

## 1 Dimension

The first example, available in [`main.py`](https://github.com/swvanbuuren/cubinterpp/tree/master/cubinterpp/main.py),
compares linear interpolation against several cubic spline variants side by side, illustrating
how the choice of slope estimation method affects the smoothness and accuracy of the result.

![Comparison of 1D interpolation types](images/comparison_1D.svg){ width="600" }
/// caption
Comparison of 1D interpolation types
///

The second example demonstrates periodic boundary conditions applied to a parametric curve.
The source code in [`periodic_spline.py`](https://github.com/swvanbuuren/cubinterpp/tree/master/cubinterpp/periodic_spline.py)
shows how to configure a spline that smoothly closes on itself, making it well suited for
cyclically repeating data.

![Parametric curve with periodic boundary conditions](images/periodic_spline.svg){ width="600" }
/// caption
Parametric curve with periodic boundary conditions
///

## 2 Dimensions

The following examples extend interpolation to two dimensions. Each plot shows the
interpolated surface together with the input data points (red dots), making it easy to see
how different methods handle the data. The full source is available in
[`main_2d.py`](https://github.com/swvanbuuren/cubinterpp/tree/master/cubinterpp/main_2d.py).

![Linear interpolation](images/linear_2D.png)
/// caption
Linear interpolation
///

![Monotonic spline interpolation](images/monotonic_2D.png)
/// caption
Monotonic spline interpolation
///

![2D Modified Akima spline interpolation](images/makima_2D.png)
/// caption
2D Modified Akima spline interpolation
///

![2D Natural spline interpolation](images/natural_spline_2D.png)
/// caption
2D Natural spline interpolation
///

The final example applies periodic boundary conditions to a 2D parametric surface, producing
a smooth, seamlessly tiling result. The corresponding source code can be found in
[`periodic_spline_2d.py`](https://github.com/swvanbuuren/cubinterpp/tree/master/cubinterpp/periodic_spline_2d.py).

![2D parametric periodic spline](images/periodic_spline_2D.png)
/// caption
2D parametric periodic spline
///