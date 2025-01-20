#include <array>
#include <vector>
#include <utility>
#include <mdspan/mdspan.hpp>


namespace cip {


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
    using Mdspan = std::mdspan<T, std::dextents<std::size_t, N>, std::layout_stride>;
    using IndexArray = std::array<std::size_t, N>;
public:
    // Constructor from dimensions and initial value
    VectorN(const T& initial_value, const IndexArray& dimensions)
        : dimensions_(dimensions), data_(calculate_total_size(dimensions), initial_value),
          mdspan(std::mdspan(data_.data(), dimensions_))
    {
    }

    // Copy constructor
    VectorN(const VectorN &other)
    : data_(other.data_), 
      dimensions_(other.dimensions_), 
      mdspan(std::mdspan(data_.data(), dimensions_)),
      current_index(0)
    {
    }

    // Constructor from nested vectors
    template <typename NestedVector>
    VectorN(const NestedVector& nested)
        : dimensions_(determine_dimensions<NestedVector, N>(nested)) {
        data_.reserve(calculate_total_size(dimensions_));
        Flatten<NestedVector, T, N>::apply(nested, data_);
        mdspan = std::mdspan(data_.data(), dimensions_);
    }

    // Constructor from just dimensions
    VectorN(const IndexArray& dimensions)
        : dimensions_(dimensions)
    {
        data_.reserve(calculate_total_size(dimensions));
        mdspan = std::mdspan(data_.data(), dimensions_);
        current_index = 0;
    }


    // Access elements using variadic indices
    template <typename... Indices>
    T& operator()(Indices... indices) {
        static_assert(sizeof...(Indices) == N, "Incorrect number of indices");
        return mdspan(std::forward<Indices>(indices)...);
    }

    template <typename... Indices>
    const T& operator()(Indices... indices) const {
        static_assert(sizeof...(Indices) == N, "Incorrect number of indices");
        return mdspan(std::forward<Indices>(indices)...);
    }

    // access elements using std::array indices
    T& operator()(const IndexArray& indices) {
        return mdspan(std::forward<IndexArray>(indices));
    }

    template <typename... Indices>
    const T& operator()(IndexArray& indices) const {
        return mdspan(std::forward<IndexArray>(indices));
    }

    template <typename... Args>
    void emplace_back(Args... args) {
        if (current_index >= data_.capacity()) {
            throw std::out_of_range("Exceeded allocated size for VectorN");
        }
        data_.emplace_back(std::forward<Args>(args)...);
        current_index++;
    }

    void reset_fill() {
        current_index = 0;
    }

    void validate_fill_complete() const {
        if (current_index != data_.size()) {
            throw std::runtime_error("VectorN has not been completely filled");
        }
    }

    Mdspan get_mdspan() {
        return mdspan;
    }

    const Mdspan get_mdspan() const {
        return mdspan;
    }

    // Extended method to return an mdspan with offset
    template <typename... Pairs>
    Mdspan submdspan(Pairs... pairs) const {
        return std::submdspan(mdspan, std::forward<Pairs>(pairs)...);
    }

    template <typename... Pairs>
    Mdspan submdspan(Pairs... pairs) {
        return std::submdspan(mdspan, std::forward<Pairs>(pairs)...);
    }

    // Accessors
    const IndexArray& dimensions() const { return dimensions_; }
    const std::vector<T>& data() const { return data_; }

private:
    IndexArray dimensions_;
    std::vector<T> data_;
    Mdspan mdspan;
    std::size_t current_index = 0;

};


} // namespace cip
