# Derivatives

The [cubic interpolation](cubic.md) method requires first derivatives $f'(x_i)$ at each grid point $x_i$ to compute the polynomial coefficients, but these derivatives are typically unknown. This section describes three methods to estimate them using slope values $m_i \approx f'(x_i)$ computed from the data. Throughout, we use the notation: grid points $\{x_i\}$ and function values $\{f_i = f(x_i)\}$ for $i = 0, 1, \ldots, n-1$, with grid spacing $h_i = x_i - x_{i-1}$.

## Monotone Slope Method

The monotone slope method, based on monotone cubic interpolation[^3], computes derivatives at grid points to ensure that the resulting interpolant is monotonic wherever the data is monotonic.

The method computes divided differences:

$$
\delta_k = \frac{f_{k+1} - f_k}{h_{k+1}}, \quad k = 0, 1, \ldots, n-2
$$

Initial slope estimates are obtained by averaging adjacent divided differences:

$$
\begin{align}
m_0 &= \delta_0 \\
m_k &= \frac{1}{2}(\delta_{k-1} + \delta_k), \quad k = 1, 2, \ldots, n-2 \\
m_{n-1} &= \delta_{n-2}
\end{align}
$$

Monotonicity is enforced through constraints:

- If $\delta_k = 0$, set $m_k = m_{k+1} = 0$  
- Otherwise, compute $\alpha = m_k/\delta_k$ and $\beta = m_{k+1}/\delta_k$  
- If $h = \sqrt{\alpha^2 + \beta^2} > 3$, rescale: $m_k \leftarrow 3\alpha\delta_k/h$ and $m_{k+1} \leftarrow 3\beta\delta_k/h$

## Modified Akima Slope Method

The Modified Akima method estimates slopes as weighted averages of divided differences, providing robustness for non-monotonic data.

Slopes are computed as:

$$
m_i = \frac{w_{i+1}\delta_{i-1} + w_i\delta_i}{w_{i+1} + w_i}
$$

where divided differences are:

$$
\delta_k = \frac{f_{k+1} - f_k}{h_{k+1}}
$$

At boundaries, virtual divided differences extend the data via quadratic extrapolation:

$$
\delta_{-1} = 2\delta_0 - \delta_1, \quad \delta_{-2} = 2\delta_{-1} - \delta_0, \quad \delta_n = 2\delta_{n-2} - \delta_{n-3}, \quad \delta_{n+1} = 2\delta_n - \delta_{n-2}
$$

The key refinement from Akima's 1970 formula[^1] is the weight definition:

$$
\begin{align}
w_i &= |\delta_i - \delta_{i-1}| + \frac{|\delta_i + \delta_{i-1}|}{2} \\
w_{i+1} &= |\delta_{i+1} - \delta_i| + \frac{|\delta_{i+1} + \delta_i|}{2}
\end{align}
$$

This modification ensures numerical stability (guarantees $m_i = 0$ for constant data) and eliminates spurious oscillations while maintaining smoothness.

## Natural Spline Method

The natural spline method enforces $C^2$ continuity by requiring continuous second derivatives at interior points.

For each interior point $x_i$ (where $i = 1, 2, \ldots, n-2$), continuity of the second derivative requires:

$$
\frac{m_i - m_{i-1}}{x_i - x_{i-1}} = \frac{m_{i+1} - m_i}{x_{i+1} - x_i}
$$

This yields the tridiagonal system: $a_i m_{i-1} + b_i m_i + c_i m_{i+1} = d_i$ with:

$$
h_i m_{i-1} + 2(h_i + h_{i+1})m_i + h_{i+1}m_{i+1} = 3\left(\frac{(f_i - f_{i-1})}{h_i^2} + \frac{(f_{i+1} - f_i)}{h_{i+1}^2}\right) h_i h_{i+1}
$$

which is solved using the [Thomas algorithm](#thomas-algorithm).

### Boundary Conditions

Four boundary condition types complete the system:

#### Natural Boundary Condition

The second derivative is zero at both boundaries: $f''(x_0) = 0$ and $f''(x_{n-1}) = 0$, giving boundary equations:

$$
\begin{align}
b_0 m_0 + c_0 m_1 &= d_0 \\
a_{n-1} m_{n-2} + b_{n-1} m_{n-1} &= d_{n-1}
\end{align}
$$

where:

$$
\begin{align}
b_0 &= \frac{2}{h_1}, \quad c_0 = \frac{1}{h_1}, \quad d_0 = \frac{3(f_1 - f_0)}{h_1^2} \\
a_{n-1} &= \frac{1}{h_{n-1}}, \quad b_{n-1} = \frac{2}{h_{n-1}}, \quad d_{n-1} = \frac{3(f_{n-1} - f_{n-2})}{h_{n-1}^2}
\end{align}
$$

#### Clamped Boundary Condition

The first derivative is zero at both boundaries: $m_0 = m_{n-1} = 0$, implemented as $b_0 = b_{n-1} = 1$ in the tridiagonal system.

#### Not-a-Knot Boundary Condition

The third derivative is continuous at the first and last interior knots: $f'''(x_1^-) = f'''(x_1^+)$ and $f'''(x_{n-2}^-) = f'''(x_{n-2}^+)$. This condition eliminates boundary kinks by extending the polynomial smoothness.

#### Periodic Boundary Condition

For periodic data where $f_0 = f_{n-1}$, the slope is also periodic:

$$
m_0 = m_{n-1}
$$

The system becomes cyclic, with interior equations:

$$
\frac{m_{n-2}}{h_{n-1}} + 2\left(\frac{1}{h_{n-1}} + \frac{1}{h_1}\right)m_{n-1} + \frac{m_1}{h_1} = 3\left(\frac{f_{n-1} - f_{n-2}}{h_{n-1}^2} + \frac{f_1 - f_0}{h_1^2}\right)h_{n-1}h_1
$$

A [cyclic variant](#cyclic-thomas-algorithm) of the Thomas algorithm is employed to solve this system.

### Thomas Algorithm

The tridiagonal matrix algorithm[^4] solves systems of the form:

$$
\begin{bmatrix}
b_0 & c_0 & 0 & \cdots & 0 \\
a_1 & b_1 & c_1 & \cdots & 0 \\
0 & a_2 & b_2 & \cdots & 0 \\
\vdots & \vdots & \vdots & \ddots & \vdots \\
0 & 0 & 0 & \cdots & b_{n-1}
\end{bmatrix}
\begin{bmatrix}
m_0 \\ m_1 \\ m_2 \\ \vdots \\ m_{n-1}
\end{bmatrix}
=
\begin{bmatrix}
d_0 \\ d_1 \\ d_2 \\ \vdots \\ d_{n-1}
\end{bmatrix}
$$

where coefficients $a_i, b_i, c_i, d_i$ are set by the boundary conditions and interior equations above.

The algorithm proceeds in two passes:

**Forward elimination:** Normalize rows to create an upper triangular system:

$$
\begin{align}
c_0' &= \frac{c_0}{b_0}, \quad d_0' = \frac{d_0}{b_0}
\end{align}
$$

For $i = 1, 2, \ldots, n-1$:

$$
\begin{align}
c_i' &= \frac{c_i}{b_i - a_i c_{i-1}'} \\
d_i' &= \frac{d_i - a_i d_{i-1}'}{b_i - a_i c_{i-1}'}
\end{align}
$$

**Back substitution:** Solve the triangular system from the last row backward:

$$
m_{n-1} = d_{n-1}'
$$

For $i = n-2, n-3, \ldots, 0$:

$$
m_i = d_i' - c_i' m_{i+1}
$$

This $O(n)$ algorithm yields the slopes $m_i = f'(x_i)$ at each grid point.

### Cyclic Thomas Algorithm

For periodic boundary conditions ($m_0 = m_{n-1}$), a cyclic variant handles the coupling. The method eliminates the cyclic structure by using the boundary rows to reduce the system size.

**Cyclic reduction:** Extract scaling factors and modify interior rows:

$$
\alpha = \frac{c_0}{b_0}, \quad \gamma = \frac{a_{n-1}}{b_{n-1}}
$$

$$
b_1' = b_1 - a_1 \alpha, \quad d_1' = d_1 - a_1 \frac{d_0}{b_0}
$$

$$
b_{n-2}' = b_{n-2} - c_{n-2} \gamma, \quad d_{n-2}' = d_{n-2} - c_{n-2} \frac{d_{n-1}}{b_{n-1}}
$$

**Solution:** Apply the standard Thomas algorithm to the reduced system of size $n-2$, then recover boundary values:

$$
m_0 = \frac{d_0 - c_0 m_1}{b_0}, \quad m_{n-1} = \frac{d_{n-1} - a_{n-1} m_{n-2}}{b_{n-1}}
$$

[^1]: Akima, H., **1970**, "A new method of interpolation and smooth curve fitting based on local procedures", [*J. ACM*](https://dx.doi.org/10.1145/321607.321609), 17(4):589-602.

[^2]: Modified Akima implementation details based on Cleve Moler's analysis of [makima piecewise cubic interpolation](https://blogs.mathworks.com/cleve/2019/04/29/makima-piecewise-cubic-interpolation/).

[^3]: Monotone cubic interpolation method: [Wikipedia - Monotone cubic interpolation](https://en.wikipedia.org/wiki/Monotone_cubic_interpolation)

[^4]: Thomas algorithm overview: [Wikipedia - Tridiagonal matrix algorithm](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm)