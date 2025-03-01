# Linear interpolation

For linear interpolation, the order of the polynomial $n = 1$. Thus, for a given
interval $\bar x \in [0, 1]^N$ now we're looking for the values
$a_{i_1,\dots,i_N}$ in the following function

$$
s(\bar x_1,\dots,\bar x_N) = \sum_{i_1,\dots,i_N=0}^1 a_{i_1,\dots,i_N}\prod_{k=0}^N\bar x_{k}^{i_k}
$$

## `1` dimension

For `1D` interpolation, this leads to the following interpolation function for given interval $[0, 1]$

$$
s(\bar x) = \sum_{k=0}^1 a_k \bar x^k =  a_0 + a_1 \bar x
$$

Here, we're looking to express $a_0$ and $a_1$ in terms of the input data
$f(x)$.  
The interpolation function can also be expressed in terms of $f(\bar x)$ for values of $\bar x$ on the interval boundaries $[0, 1]$:

$$
s(\bar x) = \sum_{i=0,1} f(i) \alpha_i^{(1,0)}(\bar x),
$$

where

$$
\begin{align}
\alpha_0^{(1,0)}(\bar x) &= 1 - \bar x \\
\alpha_1^{(1,0)}(\bar x) &= \bar x
\end{align}
$$

Given this, $a_{0,1}$ become

$$
\begin{align}
a_0 &= f(0) \\
a_1 &= f(1) - f(0)
\end{align}
$$

### Non-normalized

The non-normalized version can be attained by substituing $\bar x = (x -
x_0)/h$, where $h = x_1 - x_0$. In doing so, the interpolation function now
reads

$$
s(x) = \sum_{k=0}^1 a_k x^k =  a_0 + a_1 x
$$

and the coefficients can be rewritten to

$$
\begin{align}
a_0 &= (x_1 f(x_0) - x_0 f(x_1))/h \\
a_1 &= (f(x_1) - f(x_0))/h
\end{align}
$$

## `2` dimensions

!!! info

    Still under construction! Sorry!!

## `N` dimensions

!!! info

    Still under construction! Sorry!!
