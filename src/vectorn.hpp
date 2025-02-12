#pragma once

#include <array>
#include <vector>
#include <utility>
#include <mdspan/mdspan.hpp>


namespace cip {


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
      mdspan(std::mdspan(data_.data(), dimensions_))
    {
    }

    // Constructor from nested vectors
    template <typename NestedVector>
    VectorN(const NestedVector& nested)
        : dimensions_(determine_dimensions<NestedVector, N>(nested)) {
        data_.reserve(calculate_total_size(dimensions_));
        Flatten<NestedVector, N>::apply(nested, data_);
        mdspan = std::mdspan(data_.data(), dimensions_);
    }

    // Constructor from just dimensions
    VectorN(const IndexArray& dimensions)
        : dimensions_(dimensions)
    {
        data_.reserve(calculate_total_size(dimensions));
        mdspan = std::mdspan(data_.data(), dimensions_);
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
    T& operator()(IndexArray& indices) {
        return mdspan(std::forward<IndexArray>(indices));
    }

    template <typename... Indices>
    const T& operator()(IndexArray& indices) const {
        return mdspan(std::forward<IndexArray>(indices));
    }

    template <typename... Args>
    void emplace_back(Args... args) {
        data_.emplace_back(std::forward<Args>(args)...);
    }

    Mdspan get_mdspan() {
        return mdspan;
    }

    const Mdspan get_mdspan() const {
        return mdspan;
    }

    template <typename... Pairs>
    Mdspan submdspan(Pairs&&... pairs) const {
        return std::submdspan(mdspan, std::forward<Pairs>(pairs)...);
    }

    template <typename... Pairs>
    Mdspan submdspan(Pairs&&... pairs) {
        return std::submdspan(mdspan, std::forward<Pairs>(pairs)...);
    }

    // This method takes an rvalue 1D vector and moves its contents into a subview of the ND
    // vector. The additional slice specifiers must yield a 1D submdspan.
    template <typename... SliceSpecs>
    void move_into_submdspan(std::vector<T>&& source, SliceSpecs&&... specs) {
        // Obtain the subview using your existing submdspan function.
        auto subview = std::submdspan(mdspan, std::forward<SliceSpecs>(specs)...);
        static_assert(decltype(subview)::rank() == 1, "The submdspan must be 1-dimensional.");

        // Check at runtime that the source vector has the same number of elements
        // as the extent of the 1D subview.
        if (subview.extent(0) != source.size()) {
            throw std::runtime_error("Size mismatch: source vector size does not match subview extent");
        }
        // Use std::move to transfer the elements.
        // Due to strided layout, the elements are not contiguous in memory.
        for (std::size_t i = 0; i < subview.extent(0); ++i) {
            subview(i) = std::move(source[i]);
        }
    }

    // Move a lower-dimensional VectorN into a submdspan of this VectorN, using the source's
    // operator()(const IndexArray&) to access its elements.
    template <std::size_t M, typename... SliceSpecs>
    void move_into_submdspan(VectorN<T, M>&& source, SliceSpecs&&... specs) {
        // Obtain the target subview.
        auto subview = std::submdspan(mdspan, std::forward<SliceSpecs>(specs)...);
        static_assert(decltype(subview)::rank() == M,
                    "The submdspan must have rank equal to the source VectorN.");

        constexpr std::size_t rank = decltype(subview)::rank();

        // Get extents from the target subview.
        std::array<std::size_t, rank> targetExtents;
        std::size_t targetTotal = 1;
        for (std::size_t d = 0; d < rank; ++d) {
            targetExtents[d] = subview.extent(d);
            targetTotal *= targetExtents[d];
        }

        // Get extents from the source VectorN.
        const auto& srcDims = source.dimensions();
        std::array<std::size_t, M> srcExtents = srcDims; // source.dimensions() returns an std::array<std::size_t, M>
        std::size_t srcTotal = 1;
        for (std::size_t d = 0; d < M; ++d) {
            srcTotal *= srcExtents[d];
        }

        if (srcTotal != targetTotal) {
            throw std::runtime_error("Size mismatch: source VectorN size does not match target subview total size");
        }

        // Use a recursive lambda to iterate over all multi-index combinations.
        std::array<std::size_t, rank> indexArray{};
        auto recursiveFill = [&](auto& self, std::size_t dim) -> void {
            if (dim == rank) {
                // For each multi-index, use source.operator()(indexArray) to access the element.
                subview(indexArray) = std::move(source(indexArray));
            } else {
                for (std::size_t i = 0; i < targetExtents[dim]; ++i) {
                    indexArray[dim] = i;
                    self(self, dim + 1);
                }
            }
        };
        recursiveFill(recursiveFill, 0);
    }


    const IndexArray& dimensions() const { return dimensions_; }
    const std::vector<T>& data() const { return data_; }

private:
    IndexArray dimensions_;
    std::vector<T> data_;
    Mdspan mdspan;

    // Utility to calculate total size of the N-dimensional array.
    constexpr std::size_t calculate_total_size(const std::array<std::size_t, N>& dimensions) {
        std::size_t total_size = 1;
        for (std::size_t dim : dimensions) {
            total_size *= dim;
        }
        return total_size;
    }

    // Recursive template to handle nested std::vector flattening
    template <typename NestedVector, std::size_t Rank>
    struct Flatten {
        static void apply(const NestedVector& nested, std::vector<T>& flat) {
            for (const auto& inner : nested) {
                Flatten<typename NestedVector::value_type, Rank - 1>::apply(inner, flat);
            }
        }
    };

    template <typename NestedVector>
    struct Flatten<NestedVector, 1> {
        static void apply(const NestedVector& nested, std::vector<T>& flat) {
            flat.insert(flat.end(), nested.begin(), nested.end());
        }
    };

    // General template to determine dimensions of a nested vector
    template <typename NestedVector, std::size_t Rank>
    std::array<std::size_t, Rank> determine_dimensions(const NestedVector& vec) {
        static_assert(Rank > 0, "Rank must be greater than 0");
        std::array<std::size_t, Rank> dimensions{};
        dimensions[0] = vec.size();
        if constexpr (Rank > 1) {
            if (!vec.empty()) {
                // Recursively call determine_dimensions on the first element of the vector
                auto sub_dimensions = determine_dimensions<typename NestedVector::value_type, Rank - 1>(vec[0]);
                std::copy(sub_dimensions.begin(), sub_dimensions.end(), dimensions.begin() + 1);
            }
        }
        return dimensions;
    }

};


} // namespace cip
