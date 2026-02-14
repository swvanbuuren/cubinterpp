

# File derivatives.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**derivatives.hpp**](derivatives_8hpp.md)

[Go to the documentation of this file](derivatives_8hpp.md)


```C++
#pragma once

#include <cstddef>
#include <tuple>
#include <utility>
#include <mdspan/mdspan.hpp>


namespace cip {


template <std::size_t N, std::size_t D, std::size_t current = 0, typename Func, typename XiArray, typename... Indices>
void iterate_over_indices(Func&& func, XiArray const& xi, Indices... indices) {
    if constexpr (current == N) {
        func(indices...);
    } else {
        if constexpr (current == D) {
            iterate_over_indices<N, D, current + 1>(std::forward<Func>(func), xi, indices...);
        } else {
            for (std::size_t i = 0; i < xi[current].size(); ++i)
            {
                iterate_over_indices<N, D, current + 1>(std::forward<Func>(func), xi, indices..., i);
            }
        }
    }
}

template <std::size_t I, std::size_t N, std::size_t D, typename Tuple, typename Accum>
constexpr auto build_coordinate_indices_impl(const Tuple& t, Accum acc) {
    if constexpr (I == N) {
        return acc;
    } else {
        if constexpr (I == D) {
            return build_coordinate_indices_impl<I + 1, N, D>(
                t, std::tuple_cat(acc, std::make_tuple(std::full_extent))
            );
        } else if constexpr (I < D) {
            return build_coordinate_indices_impl<I + 1, N, D>(
                t, std::tuple_cat(acc, std::make_tuple(std::get<I>(t)))
            );
        } else {
            return build_coordinate_indices_impl<I + 1, N, D>(
                t, std::tuple_cat(acc, std::make_tuple(std::get<I - 1>(t)))
            );
        }
    }
}

template <std::size_t N, std::size_t D, typename Tuple>
constexpr auto build_coordinate_indices(const Tuple& t) {
    return build_coordinate_indices_impl<0, N, D>(t, std::tuple<>());
}

template <typename FType, typename CoordTuple, typename DerivTuple>
auto call_submdspan_1d(FType& F, const CoordTuple& coord, const DerivTuple& deriv) {
    return std::apply([&F](const auto&... args) {
        return F.submdspan_1d(args...);
    }, std::tuple_cat(coord, deriv));
}

template <typename FType, typename SlopeType, typename CoordTuple, typename DerivTuple>
void call_move_into_submdspan(FType& F, SlopeType&& slopes, const CoordTuple& coord, const DerivTuple& deriv) {
    std::apply([&F, &slopes](const auto&... args) {
        F.move_into_submdspan(std::forward<SlopeType>(slopes),args...);
    }, std::tuple_cat(coord, deriv));
}

constexpr std::size_t SIZE_T_ZERO = 0;

template <std::size_t N>
constexpr auto make_zero_tuple() {
    return []<std::size_t... I>(std::index_sequence<I...>) {
        return std::make_tuple(((void)I, SIZE_T_ZERO)...);
    }(std::make_index_sequence<N>{});
}

template <std::size_t D, typename Tuple, typename T>
constexpr auto update_tuple_element(const Tuple& tup, T new_value) {
    constexpr std::size_t N = std::tuple_size_v<Tuple>;
    return []<std::size_t... Is>(const Tuple& tup, T new_value, std::index_sequence<Is...>) {
        return std::make_tuple((Is == D ? new_value : std::get<Is>(tup))...);
    }(tup, new_value, std::make_index_sequence<N>{});
}

template <std::size_t N, typename Tuple, typename Func, std::size_t... Is>
constexpr void for_each_dimension_impl(const Tuple& tup, Func&& func, std::index_sequence<Is...>) {
    ((std::get<Is>(tup) == 0 ? (func(std::integral_constant<std::size_t, Is>{}), 0) : 0), ...);
}

template <std::size_t N, typename Tuple, typename Func>
constexpr void for_each_dimension(const Tuple& tup, Func&& func) {
    for_each_dimension_impl<N>(tup, std::forward<Func>(func), std::make_index_sequence<N>{});
}

template <std::size_t N, typename FType, typename XiArray, typename DerivTuple, typename CalcSlopesFunctor>
void compute_mixed_derivatives_impl(FType& F, XiArray const& xi, const DerivTuple& currentDeriv, CalcSlopesFunctor calcSlopes, std::size_t start = 0) {
    for_each_dimension<N>(currentDeriv, [&](const auto d_const) {
        constexpr std::size_t D = d_const.value;
        if (D < start) {
            return;
        }
        iterate_over_indices<N, D>([&, d_const](auto... loopIndices) {
            constexpr std::size_t D = d_const.value;
            auto indicesTuple = std::make_tuple(loopIndices...);
            auto coord = build_coordinate_indices<N, D>(indicesTuple);
            auto extractionPattern = update_tuple_element<D>(currentDeriv, 0);
            auto f_slice = call_submdspan_1d(F, coord, extractionPattern);
            auto newDeriv = update_tuple_element<D>(currentDeriv, 1);
            call_move_into_submdspan(F, calcSlopes(xi[D], f_slice), coord, newDeriv);
        }, xi);
        auto newDeriv = update_tuple_element<D>(currentDeriv, 1);
        compute_mixed_derivatives_impl<N>(F, xi, newDeriv, calcSlopes, D+1);
    });
}

template <std::size_t N, typename FType, typename XiArray, typename CalcSlopesFunctor>
void compute_mixed_derivatives(FType& F, XiArray const& xi, CalcSlopesFunctor calcSlopes) {
    auto basePattern = make_zero_tuple<N>();
    compute_mixed_derivatives_impl<N>(F, xi, basePattern, calcSlopes, 0);
}


}  // namespace cip
```


