#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <vector>


namespace lns {


template <typename T>
class LinearCell1D {
public:
    explicit LinearCell1D(T _x0, T _x1, T _f0, T _f1)
     : x0(_x0), b0(_f0), a0((_f1-_f0)/(_x1-_x0))
    {
    }

    T eval(const T &xi) const
    {
        return b0 + a0 * (xi - x0);
    }

private:
    const T x0;
    const T a0;
    const T b0;
};


template <typename T>
class LinearInterp1D {
    using Cell = LinearCell1D<T>;
    using Vector = std::vector<T>;
public:
    LinearInterp1D(const Vector &_x, const Vector &_y)
      : x(_x),
        index_back(x.size()-2),
        x_front(x[index_front]),
        x_back(x[x.size()-1]),
        x_delta((x_back-x_front)/(x.size()-1))
    {
        assert(_x.size() == _y.size());
        build(_x, _y);
    }
    ~LinearInterp1D() { }

    void build(const Vector &x, const Vector &y)
    {
        cells.reserve(x.size()-1);
        for (int i = 0; i < x.size()-1; ++i)
        {
            cells.push_back(Cell(x[i], x[i+1], y[i], y[i+1]));
        }
    }
    T eval(const T xi) const
    {
        return cells[get_index(xi)].eval(xi);
    }


    Vector evaln(const Vector &xi) const
    {
        auto xi_iter = xi.begin();
        Vector yi(xi.size());
        for (auto &yi_i : yi)
        {
            yi_i = eval(*xi_iter++);
        }
        return yi;
    }

private:
    const Vector x;
    const size_t index_front = 0;
    const size_t index_back;
    const double x_front;
    const double x_back;
    const double x_delta;
    std::vector<Cell> cells;

    const size_t cell_index(const double xi) const
    {
        return
        (xi < x_back) ?
            ((xi < x_front) ?
                index_front :
                (size_t)((xi-x_front)/x_delta)) :
            index_back;
    }

    const size_t sort_index(const double xi) const
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

    const size_t get_index(const double xi) const
    {
        return sort_index(xi);
    }

}; // class LinearInterp1D


} // namespace lns
