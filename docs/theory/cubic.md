# Cubic interpolation

For cubic interpolation, the polynomial order is raised to $n=3$. Now, for a
given interal $\bar x \in [0, 1]^N$ we're looking for the values
$a_{i_1,\dots,i_N}$ in the following function

$$
s(\bar x_1,\dots,\bar x_N) = \sum_{i_1,\dots,i_N=0}^3 \bar a_{i_1,\dots,i_N}\prod_{k=0}^N\bar x_{k}^{i_k}
$$

Expressing $s(\bar x_1,\dots,\bar x_N)$ in terms of $f(\bar x_1,\dots,\bar x_N)$
leads to:

$$
\begin{equation}
s(\bar x_1,\dots,\bar x_N) = \sum_{l_1,\dots,l_N=0}^1 \sum_{i_1,\dots,i_N=0,1} f^{(l_1,\dots,l_N)}(i_1,\dots,i_N) \prod_{k=1}^N \alpha^{(3,l_k)}_{i_k} (\bar x_k), \label{eq:s_normalized}
\end{equation}
$$

where

$$
\begin{align*}
\alpha_0^{3,0}(\bar x_k) &= 1 - 3\bar x_k^2 + 2 \bar x_k ^3\\
\alpha_0^{3,1}(\bar x_k) &= 3\bar x_k^2 - 2\bar x_k^3\\
\alpha_1^{3,0}(\bar x_k) &= \bar x_k - 2 \bar x_k^2 + \bar x_k^3\\
\alpha_1^{3,1}(\bar x_k) &=-\bar x_k^2 + \bar x_k^3
\end{align*}
$$

The reader is reminded that, as stated [here](index.md), $f^{(l)}(\bar x) \equiv
\frac{\textrm{d}^lf(\bar x)}{\textrm{d}\bar x^l}$. For cubic interpolation only
zeroth and first order differentation occurs, which occasionaly might be denoted
using $f(\bar x)$ and $f'(\bar x)$ respectively.  
Analogue to linear interpolation, $\eqref{eq:s_normalized}$ is also rewritten to its non-normalized version:

$$
s(x_1,\dots,x_N) = \sum_{i_1,\dots,i_N=0}^3 a_{i_1,\dots,i_N}\prod_{k=0}^N x_{k}^{i_k}
$$

in which we're looking for expressions for the coefficients $a_{i_1,\dots,i_N}$.
By substututing $\bar x_k = (x_k - {}^0x_k)/h_k$ (where $h_k={}^1x_k-{}^0x_k$)
into $\eqref{eq:s_normalized}$ we obtain

$$
\begin{equation}
\begin{split}
s(x_1,\dots,x_N) = \sum_{l_1,\dots,l_N=0}^1 \sum_{i_1,\dots,i_N=0}^1 &
\left( \prod_{k=1}^N h_k^{l_k} \right) f^{(l_1,\dots,l_N)}({}^{i_1}x_1,\dots,{}^{i_N}x_N) \dots \\
& \dots \prod_{k=1}^N \alpha^{(3,l_k)}_{i_k} (x_k)
\end{split} \label{eq:s_non_normalized}
\end{equation}
$$

Note the additional term $\prod_{k=1}^N h_k^{l_k}$, which arises due to
substition of $\bar x_k = (x_k - {}^0x_k)/h_k$ into $f^{(l)}(\bar x_k)$.  
In $\eqref{eq:s_non_normalized}$ $\alpha_{i_k}^{(3,l_k)}$ can be expressed as:

$$
\begin{align*}
\alpha_0^{3,0}(x_k) &= \frac{1}{h_k^3} \sum^3_{i=0} \delta^{(0,i)}_k x_k^i\\
\alpha_0^{3,1}(x_k) &= \frac{1}{h_k^3} \sum^3_{i=0} \delta^{(1,i)}_k x_k^i\\
\alpha_1^{3,0}(x_k) &= \frac{1}{h_k^3} \sum^3_{i=0} \delta^{(2,i)}_k x_k^i\\
\alpha_1^{3,1}(x_k) &= \frac{1}{h_k^3} \sum^3_{i=0} \delta^{(3,i)}_k x_k^i
\end{align*}
$$

in which the expressions $\delta^{(j,i)}_k$ read

|   | $i=0$ | $i=1$ | $i=2$ | $i=3$ |
|--:|:-----:|:-----:|:-----:|:-----:|
| $j=0$ | ${}^1x_k^2({}^1x_k-3{}^0x_k)$ | $+6{}^0x_k{}^1x_k$ | $-3({}^0x_k+{}^1x_k)$ | $+2$ |
| $j=1$ | ${}^0x_k^2(3{}^1x_k-{}^0x_k)$ | $-6{}^0x_k{}^1x_k$ | $+3({}^0x_k+{}^1x_k)$ | $-2$ |
| $j=2$ | $-{}^0x_k{}^1x_k^2$ | ${}^1x_k(2{}^0x_k+{}^1x_k)$ | $-({}^0x_k+2{}^1x_k)$ | $+1$ |
| $j=3$ | $-{}^1x_k{}^0x_k^2$ | ${}^0x_k({}^0x_k+2{}^1x_k)$ | $-(2{}^0x_k+{}^1x_k)$ | $+1$ |

The following sections show how to employ $\eqref{eq:s_non_normalized}$ to
obtain expressions for the coefficients $a_{i_1,\dots,i_N}$ for `1`, `2` and `N`
dimensions.

## `1` dimension

For `1` dimension the interval definition simplifies to $x \in [x_0, x_1]$. With
this:

$$
\begin{equation}
s(x) = \sum_{k=0}^3 a_k x^k =  a_0 + a_1 x + a_2 x^2 + a_3 x^3
\end{equation}
$$

in which the coefficients can be written as

$$
\begin{align*}
a_0 &= (f(x_ 0)x_1^2(x_1 - 3 x_0) + f(x_1)x_0^2(3x_1 - x_0) - h x_0 x_1(x_1f'(x_0) +  x_0f'(x_1)))/h^3\\
a_1 &= (+6 x_0 x_1 (f_0-f_1) + h ( x_1 (2 x_0 + x_1)f'(x_0) + x_0 (x_0 + 2 x_1)f'(x_1)))/h^3\\
a_2 &= (-3 (x_0 + x_1)(f_0-f_1) - h( (x_0 + 2 x_1)f'(x_0) + (2 x_0 + x_1)f'(x_1)))/h^3\\
a_3 &= (+2 (f_0-f_1) + h (f'(x_0) + f'(x_1)))/h^3
\end{align*}
$$

where $h = x_1 - x_0$.

## `2` dimensions

For `2` dimensions the interval definition now reads $x \in [{}^0x, {}^1x]^2$.
With this:

$$
\begin{align*}
s(x_1,x_2) = & \sum_{k,l=0}^3 a_{kl} x_1^k x_2^l
\end{align*}
$$

Now, we're looking for the coefficients $a_{kl}$ for $k,l=0,1,2,3$.  
Analogue to linear interpolationt this is accomplished by rewriting
$\eqref{eq:s_non_normalized}$ for $N=2$. To simplify the expressions, we shall
rewrite $f_{ij}^{(k,l)} = f^{(k,l)}({}^{i}x_1{}^{j}x_2)$. This leads
to

$$
\begin{align*}
s(x_1,x_2) = & \sum_{l_1,l_2=0}^1 \sum_{i_1,i_2=0}^1 
 h_1^{l_1}h_2^{l_2} f_{i_1i_2}^{(l_1,l_2)} \alpha^{(3,l_1)}_{i_1} (x_1) \alpha^{(3,l_2)}_{i_2} (x_2)\\
=& \frac{1}{h_1^3 h_2^3} \sum^3_{k,l=0} \Bigl( \\ 
 & f_{00}^{(0,0)} \delta^{(0,k)}_1 \delta^{(0,l)}_2  + 
   f_{01}^{(0,0)} \delta^{(0,k)}_1 \delta^{(1,l)}_2  + 
   f_{10}^{(0,0)} \delta^{(1,k)}_1 \delta^{(0,l)}_2  + 
   f_{11}^{(0,0)} \delta^{(1,k)}_1 \delta^{(1,l)}_2 ~ + \\
 & h_2\left(f_{00}^{(0,1)} \delta^{(0,k)}_1 \delta^{(2,l)}_2 + 
            f_{01}^{(0,1)} \delta^{(0,k)}_1 \delta^{(3,l)}_2 +
            f_{10}^{(0,1)} \delta^{(1,k)}_1 \delta^{(2,l)}_2 +
            f_{11}^{(0,1)} \delta^{(1,k)}_1 \delta^{(3,l)}_2\right) ~ + \\
 & h_1\left(f_{00}^{(1,0)} \delta^{(2,k)}_1 \delta^{(0,l)}_2 + 
            f_{01}^{(1,0)} \delta^{(2,k)}_1 \delta^{(1,l)}_2 + 
            f_{10}^{(1,0)} \delta^{(3,k)}_1 \delta^{(0,l)}_2 + 
            f_{11}^{(1,0)} \delta^{(3,k)}_1 \delta^{(1,l)}_2 \right)~ + \\
 & h_1h_2\left(f_{00}^{(1,1)} \delta^{(2,k)}_1 \delta^{(2,l)}_2 + 
               f_{01}^{(1,1)} \delta^{(2,k)}_1 \delta^{(3,l)}_2 +
               f_{10}^{(1,1)} \delta^{(3,k)}_1 \delta^{(2,l)}_2 +
               f_{11}^{(1,1)} \delta^{(3,k)}_1 \delta^{(3,l)}_2 \right)\\
& \Bigr)x_1^k x_2^l
\end{align*}
$$

From this it becomes clear that

$$
\begin{align*}
a_{kl} = &
 \Biggl( f_{00}^{(0,0)} \delta^{(0,k)}_1 \delta^{(0,l)}_2  + 
   f_{01}^{(0,0)} \delta^{(0,k)}_1 \delta^{(1,l)}_2  + 
   f_{10}^{(0,0)} \delta^{(1,k)}_1 \delta^{(0,l)}_2  + 
   f_{11}^{(0,0)} \delta^{(1,k)}_1 \delta^{(1,l)}_2 ~ + \\
 & h_2\left(f_{00}^{(0,1)} \delta^{(0,k)}_1 \delta^{(2,l)}_2 + 
            f_{01}^{(0,1)} \delta^{(0,k)}_1 \delta^{(3,l)}_2 +
            f_{10}^{(0,1)} \delta^{(1,k)}_1 \delta^{(2,l)}_2 +
            f_{11}^{(0,1)} \delta^{(1,k)}_1 \delta^{(3,l)}_2\right) ~ + \\
 & h_1\left(f_{00}^{(1,0)} \delta^{(2,k)}_1 \delta^{(0,l)}_2 + 
            f_{01}^{(1,0)} \delta^{(2,k)}_1 \delta^{(1,l)}_2 + 
            f_{10}^{(1,0)} \delta^{(3,k)}_1 \delta^{(0,l)}_2 + 
            f_{11}^{(1,0)} \delta^{(3,k)}_1 \delta^{(1,l)}_2 \right)~ + \\
 & h_1h_2\left(f_{00}^{(1,1)} \delta^{(2,k)}_1 \delta^{(2,l)}_2 + 
               f_{01}^{(1,1)} \delta^{(2,k)}_1 \delta^{(3,l)}_2 +
               f_{10}^{(1,1)} \delta^{(3,k)}_1 \delta^{(2,l)}_2 +
               f_{11}^{(1,1)} \delta^{(3,k)}_1 \delta^{(3,l)}_2 \right)\Biggr)\\
\end{align*}
$$
