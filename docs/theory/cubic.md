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
\frac{\textrm{d}^lf(\bar x)}{\textrm{d}\bar x^l}$.

Analogue to linear interpolation, $\eqref{eq:s_normalized}$ is also rewritten to its non-normalized version:

$$
s(x_1,\dots,x_N) = \sum_{i_1,\dots,i_N=0}^3 a_{i_1,\dots,i_N}\prod_{k=0}^N x_{k}^{i_k}
$$

in which we're looking for expressions for the coefficients $a_{i_1,\dots,i_N}$.
By substututing $\bar x = (x - {}^0x)/h$ (where $h={}^1x-{}^0x$) into
$\eqref{eq:s_normalized}$ we obtain

$$
\begin{equation}
s(x_1,\dots,x_N) = \sum_{l_1,\dots,l_N=0}^1 \sum_{i_1,\dots,i_N=0}^1 f^{(l_1,\dots,l_N)}({}^{i_1}x_1,\dots,{}^{i_N}x_N) \prod_{k=1}^N \alpha^{(3,l_k)}_{i_k} (x_k), \label{eq:s_non_normalized}
\end{equation}
$$

where

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
