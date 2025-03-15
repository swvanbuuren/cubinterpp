#pragma once

#include <mdspan/mdspan.hpp>
#include <array>
#include <cstddef>


namespace cip {


constexpr std::size_t power(std::size_t base, std::size_t exp) {
    return exp == 0 ? 1 : base * power(base, exp - 1);
}


template <std::size_t Value, std::size_t... I>
constexpr std::extents<std::size_t, ((void)I, Value)...>
make_extents(std::index_sequence<I...>) {
    return {};
}


template<typename T, std::size_t N, std::size_t Size>
class NDHyperArray {
    static constexpr std::size_t total_size = power(Size, N);
    using Array = std::array<T, total_size>;
    using ExtentsType = decltype(make_extents<Size>(std::make_index_sequence<N>{}));
    using Mdspan  = std::mdspan<T, ExtentsType>;
public:
    NDHyperArray()
      : data{}, mdspan(data.data())
    {}

    ~NDHyperArray() = default;

    template<typename... Index>
    T& operator()(Index... idx) {
        return mdspan(idx...);
    }

    template<typename... Index>
    const T& operator()(Index... idx) const {
        return mdspan(idx...);
    }

private:
    Array data;
    Mdspan mdspan;

};


} // namespace cip
