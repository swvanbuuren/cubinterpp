# Mathematical background

The mathematical foundation of the interpolation algorithms used in Cubinterpp
is based on the mathematical formulations outlined by Lalescu, 2009[^1]. Lalescu
treats Hermite splines as well as grid splines. Cubinterpp only uses the former
type of splines.

[^1]: Lalescu, C.C., **2009**,  Two hierarchies of spline interpolations. Practical algorithms for multivariate higher order splines. [*arXiv:0905.3564*](https://arxiv.org/abs/0905.3564)

First, the general definition of `1`-Dimensional (`1D`) splines and the
generalization to `N`-Dimensional (`ND`) Hermite splines for $n^\textrm{th}$
order splines shall be introduced here. Spline definitions are only given for a
specific interval (a.k.a. cell): e.g. $x \in [x_0, x_0 + h]$ for the
`1`-Dimensional case. The extension to piecewise interpolation with a series
neighboring cells is not treated here.

## `1` dimension

The main goal is to find an approximating interpolation of $f(x)$ on an
interval $x \in [x_0, x_0 + h]$, given values and derivatives of $f(x)$ on the boundaries of that interval.  
In the following the interval is normalized from $x \in [x_0, x_0 + h]$ to $\bar
x \in [0, 1]$. 

!!! tip

    The sections for the [linear](linear.md) and [cubic interpolation](cubic.md) also give unnormalized derivations.

For the `1`-Dimensional case, the goal is to obtain a polynomial 
$$
s^{(n)}(\bar x) = \sum_{k=0}^n a^{(n)}_k \bar x^k,
$$
that approximates $f(x)$ in the interpolation interval. Here, $n$ is the order
of the spline, $\bar x$ the variable coordinate and $a_k$ are the
to-be-determined coefficients.

The (derivate) values $f^{l}(\bar x)$, where $f^{l}(\bar x) \equiv
\frac{\textrm{d}^lf(\bar x)}{\textrm{d}\bar x^l}$, are assumed to coincide with those of
$s^{(n)}(\bar x)$ on the boundaries of the interval.

Given this, according to Lalescu, 2009[^1], $s^{(n)}(\bar x)$ can be written as follows:
$$
s^{(n)}(\bar x) = \sum_{l=0}^{} \sum_{i=0,1} f^{(l)}(i) \alpha^{(n,l)}_i (\bar
x),
$$
where $m \equiv (n-1)/2$ and $\alpha^{(n,l)}_i$ becomes

$$
\begin{align}
\alpha^{(n,l)}_0 (x) &= \frac{x^l}{l!}(1-x)^{m+1} \sum_{k=0}^{m-l} \frac{(m+k)!}{m! k!}x^k \\
\alpha^{(n,l)}_1 (x) &= \frac{(x-1)^l}{l!}x^{m+1} \sum_{k=0}^{m-l} \frac{(m+k)!}{m! k!}(1-x)^k
\end{align}
$$

## `N` dimensions

The `1D` spline definition can be directly be generalized to a scalar function $N$ variables $\bar x_i$ for $i=1,\dots,N$ defined for the interval $[0, 1]^N$:
$$
s^{(n)}(\bar x_1,\bar x_2,\dots,\bar x_N) = \sum_{l_1,\dots,l_N=0}^m \sum_{i_1,\dots,i_N=0,1} f^{(l_1,\dots,l_N)}(i_1,\dots,i_N) \prod_{k=1}^N \alpha^{(n,l_k)}_{i_k} (\bar x_k)
$$

For multiple dimensions, we're now also dealing with (combined) derivatives in multiple directions. That is, $f^{(l_1,\dots,l_N)}$ is now defined as:

$$
f^{(l_1,\dots,l_N)}(\bar x_1,\dots,\bar x_N) = \left( \prod_{j=1}^{N} \left(\frac{\partial}{\partial \bar x_j}\right)^{l_j} \right)f(\bar x_1,\dots,\bar x_N)
$$

## Specific implementations

Specific derivations for linear ($n=1$) and cubic splines ($n=3$) can be found
in the sections [linear interpolation](linear.md) and [cubic
interpolation](cubic.md).
