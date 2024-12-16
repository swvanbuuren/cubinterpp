#include <array>
#include <vector>
#include <mdspan/mdspan.hpp>


namespace vec {


// Utility to calculate total size of the N-dimensional array.
template <std::size_t N>
constexpr std::size_t calculate_total_size(const std::array<std::size_t, N>& dimensions) {
    std::size_t total_size = 1;
    for (std::size_t dim : dimensions) {
        total_size *= dim;
    }
    return total_size;
}


// Recursive template to handle nested std::vector flattening
template <typename NestedVector, typename T, std::size_t N>
struct Flatten {
    static void apply(const NestedVector& nested, std::vector<T>& flat) {
        for (const auto& inner : nested) {
            Flatten<typename NestedVector::value_type, T, N - 1>::apply(inner, flat);
        }
    }
};

template <typename NestedVector, typename T>
struct Flatten<NestedVector, T, 1> {
    static void apply(const NestedVector& nested, std::vector<T>& flat) {
        flat.insert(flat.end(), nested.begin(), nested.end());
    }
};

// General template to determine dimensions of a nested vector
template <typename NestedVector, std::size_t N>
std::array<std::size_t, N> determine_dimensions(const NestedVector& vec) {
    static_assert(N > 0, "N must be greater than 0");
    std::array<std::size_t, N> dimensions{};
    dimensions[0] = vec.size();
    if constexpr (N > 1) {
        if (!vec.empty()) {
            // Recursively call determine_dimensions on the first element of the vector
            auto sub_dimensions = determine_dimensions<typename NestedVector::value_type, N - 1>(vec[0]);
            std::copy(sub_dimensions.begin(), sub_dimensions.end(), dimensions.begin() + 1);
        }
    }
    return dimensions;
}

// VectorN definition template
template <typename T, std::size_t N>
class VectorN {
public:
    // Constructor from dimensions and initial value
    VectorN(const T& initial_value, const std::array<std::size_t, N>& dimensions)
        : dimensions_(dimensions), data_(calculate_total_size(dimensions), initial_value) {}

    // Copy constructor
    VectorN(const VectorN& other) = default;

    // Constructor from nested vectors
    template <typename NestedVector>
    VectorN(const NestedVector& nested)
        : dimensions_(determine_dimensions<NestedVector, N>(nested)) {
        data_.reserve(calculate_total_size(dimensions_));
        Flatten<NestedVector, T, N>::apply(nested, data_);
    }

    // Access elements using variadic indices
    template <typename... Indices>
    T& operator()(Indices... indices) {
        static_assert(sizeof...(Indices) == N, "Incorrect number of indices");
        return data_[calculate_index({static_cast<std::size_t>(indices)...})];
    }

    template <typename... Indices>
    const T& operator()(Indices... indices) const {
        static_assert(sizeof...(Indices) == N, "Incorrect number of indices");
        return data_[calculate_index({static_cast<std::size_t>(indices)...})];
    }

    auto mdspan() const {
        return std::mdspan<const T, std::dextents<size_t, N>>(data_.data(), dimensions_);
    }

        // Extended method to return an mdspan with offset
    template <typename... Pairs>
    auto submdspan(Pairs... pairs) const {
        auto full_mdspan = mdspan();
        return std::submdspan(full_mdspan, pairs...);
    }

    // Accessors
    const std::array<std::size_t, N>& dimensions() const { return dimensions_; }
    const std::vector<T>& data() const { return data_; }

private:
    std::array<std::size_t, N> dimensions_;
    std::vector<T> data_;

    std::size_t calculate_index(const std::array<std::size_t, N>& indices) const {
        std::size_t index = 0;
        std::size_t stride = 1;
        for (std::size_t i = N; i-- > 0;) {
            index += indices[i] * stride;
            stride *= dimensions_[i];
        }
        return index;
    }

};


} // namespace vec
