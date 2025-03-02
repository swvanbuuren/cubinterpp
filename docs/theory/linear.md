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
s(\bar x_1,\dots,\bar x_N) = \sum_{i_1,\dots,i_N=0,1} f(i_1,\dots,i_N) \prod_{k=1}^N \alpha^{(1,0)}_{i_k} (\bar x_k), \label{eq:s_normalized}
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
s^{(n)}(x_1,\dots,x_N) = \sum_{i_1,\dots,i_N=0,1} f({}^{i_1}x_1,\dots,{}^{i_N}x_N) \prod_{k=1}^N \alpha^{(1,0)}_{i_k} (x_k), \label{eq:s_non_normalized}
\end{equation}
$$

where

$$
\begin{align}
\alpha_0^{(1,0)}(x_k) &= \sum^1_{i=0} \gamma^{(0,i)}_k x_k^i,~~~\textrm{where}~~~ \gamma^{(0,0)}_k = +\tfrac{1}{h_k}{}^{1}x_k,~~~\gamma^{(0,1)}_k = - \tfrac{1}{h_k}
\label{eq:alpha0_non_normalized}\\
\alpha_1^{(1,0)}(x_k) &= \sum^1_{i=0} \gamma^{(1,i)}_k x_k^i,~~~\textrm{where}~~~
\gamma^{(1,0)}_k = -\tfrac{1}{h_k}{}^{0}x_k,~~~\gamma^{(1,1)}_k = +
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
s(x_1, x_2) = \sum_{k,l=0}^1 a_{kl} x_1^k x_2^l = a_{00} + a_{10}x_1 + a_{01}x_2 + a_{11}x_1x_2,
\end{equation}
$$

in which we're looking the coefficients $a_{ij}$ for $i,j=0,1$.  
This can be accomplished by rewriting $\eqref{eq:s_non_normalized}$ for $N=2$
using $\eqref{eq:alpha0_non_normalized}$ and $\eqref{eq:alpha1_non_normalized}$.
To simplify the expressions we shall abbreviate $f_{ij} \equiv f({}^ix_1,{}^jx_2)$. With this $s(x_1,x_2)$ reads

$$
\begin{align*}
s(x_1,x_2) =& \sum_{i_1,i_2=0,1} f_{i_1i_2} \alpha^{(1,0)}_{i_1} (x_1)
\alpha^{(1,0)}_{i_2} (x_2)\\
 = &~ \underbrace{f_{00}\gamma^{(0,0)}_1 \gamma^{(0,0)}_2 + 
      f_{01}\gamma^{(0,0)}_1 \gamma^{(1,0)}_2 + 
      f_{10}\gamma^{(1,0)}_1 \gamma^{(0,0)}_2 + 
      f_{11}\gamma^{(1,0)}_1 \gamma^{(1,0)}_2}_{a_{00}} + \\
   &~ \underbrace{\left( f_{00}\gamma^{(0,1)}_1 \gamma^{(0,0)}_2 + 
             f_{01}\gamma^{(0,1)}_1 \gamma^{(1,0)}_2 + 
             f_{10}\gamma^{(1,1)}_1 \gamma^{(0,0)}_2 + 
             f_{11}\gamma^{(1,1)}_1 \gamma^{(1,0)}_2 \right)}_{a_{10}} x_1 +  \\
   &~ \underbrace{\left( f_{00}\gamma^{(0,0)}_1 \gamma^{(0,1)}_2 + 
             f_{01}\gamma^{(0,0)}_1 \gamma^{(1,1)}_2 + 
             f_{10}\gamma^{(1,0)}_1 \gamma^{(0,1)}_2 + 
             f_{11}\gamma^{(1,0)}_1 \gamma^{(1,1)}_2 \right)}_{a_{01}} x_2 +  \\
   &~ \underbrace{\left( f_{00}\gamma^{(0,1)}_1 \gamma^{(0,1)}_2 + 
             f_{01}\gamma^{(0,1)}_1 \gamma^{(1,1)}_2 + 
             f_{10}\gamma^{(1,1)}_1 \gamma^{(0,1)}_2 + 
             f_{11}\gamma^{(1,1)}_1 \gamma^{(1,1)}_2 \right)}_{a_{11}} x_1x_2\\
\end{align*}
$$

This eventually leads to the following coefficients:

$$
\begin{align*}
a_{00} &= \left(f_{00}{}^1x_1 {}^1x_2 - 
                f_{01}{}^1x_1 {}^0x_2 - 
                f_{10}{}^0x_1 {}^1x_2 + 
                f_{11}{}^0x_1 {}^0x_2 \right)/h_1h_2\\
a_{10} &= \left(-f_{00}{}^1x_2 + 
                 f_{01}{}^0x_2 + 
                 f_{10}{}^1x_2 -
                 f_{11}{}^0x_2 \right)/h_1h_2\\
a_{01} &= \left(-f_{00}{}^1x_1 + 
                 f_{01}{}^1x_1 + 
                 f_{10}{}^0x_1 -
                 f_{11}{}^0x_1 \right)/h_1h_2\\
a_{11} &= \left( f_{00} - 
                 f_{01} - 
                 f_{10} +
                 f_{11} \right)/h_1h_2
\end{align*}
$$

## `N` dimensions

For `N` dimensions, again the $\alpha$-terms in equation
$\eqref{eq:s_non_normalized}$ are now replaced with the expressions from
$\eqref{eq:alpha0_non_normalized}$ and $\eqref{eq:alpha1_non_normalized}$:

$$
s^{(n)}(x_1,\dots,x_N) = \sum_{i_1,\dots,i_N=0,1} f({}^{i_1}x_1,\dots,{}^{i_N}x_N) \prod_{k=1}^N \left( \gamma^{(i_k,0)}_k + \gamma^{(i_k,1)}_k x_k  \right)
$$

Analogue to `2` dimensions, this expression needs to be reordered to terms with
common combinations of $x_k$​. One term for $x_1$​, one for $x_2$​, and so on,
also terms combining multiple $x_k$'s. Then, each term will correspond to one
the coefficients in $a_{i_1,\dots,i_N}$.  
Doing so leads to

$$
s^{(n)}(x_1,\dots,x_N) = \sum_{J\subseteq \{1,\dots,N\}} c_J \prod_{k\in J} x_k,
$$

where

$$
c_J = \sum_{i_1,\dots,i_N=0,1} f\Bigl({}^{i_1}x_1,\dots,{}^{i_N}x_N\Bigr)
\prod_{k\notin J} \gamma^{(i_k,0)}_k \prod_{k\in J} \gamma^{(i_k,1)}_k.
$$
