#pragma once

#include <cmath>
#include <vector>
#include <algorithm>


namespace cip {


template <typename T>
class Indexer {
    using Vector = std::vector<T>;
public:
    Indexer(const Vector &_x)
    : x(_x),
      index_back(x.size()-2),
      x_front(x[index_front]),
      x_back(x[x.size()-1]),
      x_delta((x_back-x_front)/(x.size()-1))
    {
    }
    ~Indexer() { }

    const std::size_t cell_index(const T xi) const
    {
        return
        (xi < x_back) ?
            ((xi < x_front) ?
                index_front :
                (std::size_t)((xi-x_front)/x_delta)) :
            index_back;
    }

    const std::size_t sort_index(const T xi) const
    {
        if (xi < x_front)
        {
            return index_front;
        }
        if (xi >= x_back)
        {
            return index_back;
        }
        return std::distance(x.begin(), std::upper_bound(x.begin(), x.end(), xi)) - 1;
    }

private:
    const Vector x;
    const size_t index_front = 0;
    const size_t index_back;
    const T x_front;
    const T x_back;
    const T x_delta;
};



constexpr inline int factorial(int n)
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}


constexpr inline std::size_t binomial(std::size_t n, std::size_t k) noexcept
{
    return
      (        k> n  )? 0 :          // out of range
      (k==0 || k==n  )? 1 :          // edge
      (k==1 || k==n-1)? n :          // first
      (     k+k < n  )?              // recursive:
      (binomial(n-1,k-1) * n)/k :    //  path to k=1   is faster
      (binomial(n-1,k) * n)/(n-k);   //  path to k=n-1 is faster
}


template <typename T>
T binomial_power_coefficient(const T y, const int n, const int k)
{
    return binomial(n, k)*std::pow(y, n-k);
}


template <std::size_t Base, std::size_t Exp>
constexpr std::size_t power()
{
    if constexpr (Exp <= 0)
    {
        return 1;
    } else {
        return Base*power<Base, Exp-1>();
    }
}


} // namespace cip
