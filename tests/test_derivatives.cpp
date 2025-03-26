#include <gtest/gtest.h>
#include <cubinterpp.hpp>


template <typename Tuple>
void print_tuple(const Tuple& tup) {
    bool first = true;
    std::apply([&first](const auto&... elems) {
        ((std::cout << (first ? "" : ", ")
                     << (elems == cip::FULL_EXTENT_SENTINEL
                             ? "std::full_extent"
                             : std::to_string(elems)),
          first = false), ...);
    }, tup);
}

template <std::size_t N, typename FType, typename XiArray, typename DerivTuple>
void print_mixed_derivatives_impl(FType& F, XiArray const& xi, const DerivTuple& currentDeriv, std::size_t start = 0) {
    cip::for_each_dimension<N>(currentDeriv, [&](auto d_const) {
        constexpr std::size_t D = d_const.value;
        if (D < start) {
            return;
        }
        cip::iterate_over_indices<N, D>([&](auto... loopIndices) {
            auto indicesTuple = std::make_tuple(loopIndices...);
            auto coord = cip::build_coordinate_indices<N, D>(indicesTuple);
            auto extractionPattern = cip::update_tuple_element<D>(currentDeriv, 0);
            
            //auto f_slice = call_submdspan_1d(F, coord, extractionPattern);
            std::cout << "auto f_slice = F.submdspan(";
            print_tuple(coord);
            std::cout << ", ";
            print_tuple(extractionPattern);
            std::cout << ");" << std::endl;

            //auto slopes = calc_slopes(xi[D], f_slice);
            std::cout << "auto slope = calc_slopes(xi[" << D << "], f_slice);" << std::endl;

            auto newDeriv = cip::update_tuple_element<D>(currentDeriv, 1);
            
            //call_move_into_submdspan(F, slopes, coord, newDeriv);
            std::cout << "F.move_into_submdspan(slopes, ";
            print_tuple(coord);
            std::cout << ", ";
            print_tuple(newDeriv);
            std::cout << ");" << std::endl << std::endl;
        }, xi);
        auto newDeriv = cip::update_tuple_element<D>(currentDeriv, 1);
        print_mixed_derivatives_impl<N>(F, xi, newDeriv, D+1);
    });
}

template <std::size_t N, typename FType, typename XiArray>
void print_mixed_derivatives(FType& F, XiArray const& xi) {
    auto basePattern = cip::make_zero_tuple<N>();
    print_mixed_derivatives_impl<N>(F, xi, basePattern, 0);
}


TEST(TestDerivatives, test_derivatives_1d) {
    using Type = double;
    constexpr std::size_t N = 1;
    using FType =  cip::VectorN<Type, 2*N>;
    using XiArray = std::array<std::vector<Type>, N>;

    XiArray xi = {{{1.0, 2.0, 3.0}}};
    FType F(Type{}, {3, 2});

    std::cout << std::endl << "- Mixed derivatives -" << std::endl;
    print_mixed_derivatives<N>(F, xi);
}


TEST(TestDerivatives, test_derivatives_2d) {
    using Type = double;
    constexpr std::size_t N = 2;
    using FType =  cip::VectorN<Type, 2*N>;
    using XiArray = std::array<std::vector<Type>, N>;

    XiArray xi = {{{1.0, 2.0, 3.0},
                   {4.0, 5.0, 6.0}}};
    FType F(Type{}, {3,3, 2,2});


    std::cout << std::endl << "- Mixed derivatives -" << std::endl;
    print_mixed_derivatives<N>(F, xi);
}


TEST(TestDerivatives, test_derivatives_3d) {
    using Type = double;
    constexpr std::size_t N = 3;
    using FType =  cip::VectorN<Type, 2*N>;
    using XiArray = std::array<std::vector<Type>, N>;

    XiArray xi = {{{1.0, 2.0, 3.0},
                   {4.0, 5.0, 6.0},
                   {7.0, 8.0, 9.0}}};
    FType F(Type{}, {3,3,3, 2,2, 2});

    std::cout << std::endl << "- Mixed derivatives -" << std::endl;
    print_mixed_derivatives<N>(F, xi);
}
