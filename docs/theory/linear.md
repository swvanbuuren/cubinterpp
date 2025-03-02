# Linear interpolation

For linear interpolation, the order of the polynomial $n = 1$. Thus, for a given
interval $\bar x \in [0, 1]^N$ now we're looking for the values
$a_{i_1,\dots,i_N}$ in the following function

$$
s(\bar x_1,\dots,\bar x_N) = \sum_{i_1,\dots,i_N=0}^1 \bar a_{i_1,\dots,i_N}\prod_{k=0}^N\bar x_{k}^{i_k}
$$

As stated [here](index.md), $s(\bar x_1,\dots,\bar x_N)$ can also be expressed in terms of $f(\bar x_1, \dots, \bar x_n)$. For linear interpolation ($n=1$), this boils down to:

$$
\begin{equation}
s^{(n)}(\bar x_1,\dots,\bar x_N) = \sum_{i_1,\dots,i_N=0,1} f(i_1,\dots,i_N) \prod_{k=1}^N \alpha^{(1,0)}_{i_k} (\bar x_k), \label{eq:s_normalized}
\end{equation}
$$

where

$$
\begin{align}
\alpha_0^{(1,0)}(\bar x_k) &= 1 - \bar x_k \label{eq:alpha0_normalized} \\
\alpha_1^{(1,0)}(\bar x_k) &= \bar x_k \label{eq:alpha1_normalized}
\end{align}
$$

To avoid unnecessary computations during evaluation, equation
$\eqref{eq:s_normalized}$ as well as $\eqref{eq:alpha0_normalized}$ and
$\eqref{eq:alpha1_normalized}$ have to be rewritten to its non-normalized
version. The interval is now defined by $x \in [{}^0x, {}^1x]^N$.  
We're now looking for $s(x_1,\dots,x_N)$:

$$
s(x_1,\dots,x_N) = \sum_{i_1,\dots,i_N=0}^1 a_{i_1,\dots,i_N}\prod_{k=0}^N
x_{k}^{i_k},
$$

in which the coefficients $a_{i_1,\dots,i_N}$ need to be determined.  
The transformation is achieved by substituting $\bar x = (x - {}^0x)/h$ (where
$h={}^1x-{}^0x$) into $\eqref{eq:s_normalized}$. This leads to

$$
\begin{equation}
s^{(n)}(x_1,\dots,x_N) = \sum_{i_1,\dots,i_N=^0x,^1x} f(i_1,\dots,i_N) \prod_{k=1}^N \alpha^{(1,0)}_{i_k} (x_k), \label{eq:s_non_normalized}
\end{equation}
$$

where

$$
\begin{align}
\alpha_0^{(1,0)}(x_k) &= \sum^1_{i=0} \gamma_{(0,i)} x_k^i,~~~\textrm{where}~~~ \gamma_{(0,0)} = +\tfrac{1}{h_k}{}^{1}x_k~~~\textrm{and}~~~\gamma_{(0,1)} = - \tfrac{1}{h_k}
\label{eq:alpha0_non_normalized}\\
\alpha_1^{(1,0)}(x_k) &= \sum^1_{i=0} \gamma_{(1,i)} x_k^i,~~~\textrm{where}~~~
\gamma_{(1,0)} = -\tfrac{1}{h_k}{}^{0}x_k~~~\textrm{and}~~~\gamma_{(1,1)} = +
\tfrac{1}{h_k} \label{eq:alpha1_non_normalized} \end{align}
$$

The following sections show how to employ $\eqref{eq:s_non_normalized}$ to
obtain expressions for the coefficients $a_{i_1,\dots,i_N}$ for `1`, `2` and `N`
dimensions.

## `1` dimension

For `1` dimension the interval definition simplifies to $x \in [x_0, x_1]$. With
this:

$$
\begin{equation}
s(x) = \sum_{k=0}^1 a_k x^k =  a_0 + a_1 x
\end{equation}
$$

and the coefficients can be rewritten to

$$
\begin{align}
a_0 &= (x_1 f(x_0) - x_0 f(x_1))/h, \\
a_1 &= (f(x_1) - f(x_0))/h,
\end{align}
$$

where $h = x_1 - x_0$.

## `2` dimensions

For `2` dimensions the interval definition now reads $x \in [{}^0x, {}^1x]^2$.
With this:

$$
\begin{equation}
s(x_1, x_2) = \sum_{k,l=0}^1 a_{kl} x_1^k x_2^l = a_{00} + a_{10}x_1 + a_{01}x_2 + a_{11}x_1x_2
\end{equation}
$$

## `N` dimensions

!!! info

    Still under construction! Sorry!!
