#pragma once

#include <cmath>
#include <vector>
#include <algorithm>


namespace cip {


enum class IndexMethod { BinarySearch, Uniform };


template <typename T, IndexMethod Method = IndexMethod::BinarySearch>
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

    [[nodiscard]] std::size_t index(const T xi) const noexcept
    {
        if constexpr (Method == IndexMethod::BinarySearch) {
            return binary_search_index(xi);
        } else if constexpr (Method == IndexMethod::Uniform) {
            return uniform_index(xi);
        } else {
            static_assert(sizeof(T) == 0,
                "Unhandled IndexMethod enumerator in Indexer::index — "
                "add a corresponding branch.");
        }
    }

private:
    bool clamp_index(const T xi, std::size_t &idx) const noexcept
    {
        if (xi < x_front) { idx = index_front; return true; }
        if (xi >= x_back) { idx = index_back;  return true; }
        return false;
    }

    std::size_t uniform_index(const T xi) const noexcept
    {
        std::size_t idx;
        if (clamp_index(xi, idx)) return idx;
        return static_cast<std::size_t>((xi - x_front) / x_delta);
    }

    std::size_t binary_search_index(const T xi) const noexcept
    {
        std::size_t idx;
        if (clamp_index(xi, idx)) return idx;
        return std::upper_bound(x.begin(), x.end(), xi) - x.begin() - 1;
    }

    const Vector x;
    static constexpr std::size_t index_front = 0;
    const std::size_t index_back;
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
