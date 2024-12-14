#pragma once

#include <vector>
#include <cstddef>
#include <stdexcept>
#include <initializer_list>


namespace vec {


// Forward declaration of VectorN
template <typename T, std::size_t N>
class VectorN;

// Specialization for 0 dimensions (scalar value)
template <typename T>
class VectorN<T, 0> {
private:
    T value; // Holds the scalar value

public:
    // Constructor to initialize scalar value
    explicit VectorN(const T& val) : value(val) {}

    // Accessor for the scalar value
    T& get() { return value; }
    const T& get() const { return value; }

    // Allow direct access and modification
    operator T&() { return value; }
    
    operator const T&() const { return value; }
};

// General case for N-dimensional vector
template <typename T, std::size_t N>
class VectorN {
private:
    std::vector<VectorN<T, N - 1>> data; // Contains (N-1)-dimensional vectors

public:
    // Constructor to initialize the vector with a given size and value
    explicit VectorN(std::size_t size, const T& value)
        : data(size, VectorN<T, N - 1>(size, value)) {}

    // Constructor to initialize from an existing std::vector
    explicit VectorN(const std::vector<VectorN<T, N - 1>>& vec) : data(vec) {}

    // Constructor to initialize from a nested initializer list
    explicit VectorN(const std::initializer_list<typename VectorN<T, N - 1>::data_type>& init_list) {
        for (const auto& element : init_list) {
            data.emplace_back(element);
        }
    }

    // Templated constructor to allow initialization from nested std::vector structures
    template <typename NestedVector,
              typename = std::enable_if_t<std::is_same_v<NestedVector, std::vector<typename VectorN<T, N - 1>::data_type>>>>
    explicit VectorN(const NestedVector& vec) {
        for (const auto& element : vec) {
            data.emplace_back(element);
        }
    }

    // Accessor to retrieve elements by index
    VectorN<T, N - 1>& operator[](std::size_t index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const VectorN<T, N - 1>& operator[](std::size_t index) const {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Getter for the size of the current dimension
    std::size_t size() const { return data.size(); }

    // Getter for underlying std::vector
    const std::vector<VectorN<T, N - 1>>& getData() const { return data; }

    using data_type = std::vector<VectorN<T, N - 1>>;
};


} // namespace vec
