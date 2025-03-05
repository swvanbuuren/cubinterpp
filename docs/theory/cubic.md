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
s^{(n)}(\bar x_1,\dots,\bar x_N) = \sum_{l_1,\dots,l_N=0}^1 \sum_{i_1,\dots,i_N=0,1} f^{(l_1,\dots,l_N)}(i_1,\dots,i_N) \prod_{k=1}^N \alpha^{(3,l_k)}_{i_k} (\bar x_k),
\end{equation}
$$

where

$$
\begin{align*}
\alpha_0^{3,0}(\bar x) &= 1 - 3\bar x^2 + 2 \bar x ^3\\
\alpha_0^{3,1}(\bar x) &= 3\bar x^2 - 2\bar x^3\\
\alpha_1^{3,0}(\bar x) &= \bar x - 2 \bar x^2 + \bar x^3\\
\alpha_1^{3,1}(\bar x) &=-\bar x^2 + \bar x^3
\end{align*}
$$
