# Boundary conditions

Boundary conditions only are valid for cubic naturla splines.

## Natural

Natural boundary condition is described in
https://en.wikipedia.org/wiki/Spline_interpolation.

## Clamped

## Not-a-Knot

Not-a-Knot boundary condition is described in
https://en.wikipedia.org/wiki/Spline_interpolation.

## Periodic

$$
k_0 - k_{n-1} = 0
$$

and 

$$
\frac{k_{n-2}}{h_n}
* 2!\left(\frac{1}{h_n} + \frac{1}{h_1}\right) k_{n-1}
* \frac{k_1}{h_1}
  =
  3!\left(
  \frac{f_{n-1}-f_{n-2}}{h_n^2}
* \frac{f_1-f_0}{h_1^2}
\right).
$$