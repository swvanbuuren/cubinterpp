#include <vector>
#include <algorithm>


namespace utils {


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
    const double x_front;
    const double x_back;
    const double x_delta;
};


} // namespace utils
