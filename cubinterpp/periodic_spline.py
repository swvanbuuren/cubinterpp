""" Simple test for Periodic Spline Interpolation """

import numpy as np
import mlpyqtgraph as mpg
import cubinterpp.cubinterpp_py as cubinterpp  # cubinterpp_py is a pybind11 module
from scipy.interpolate import CubicSpline


def test_data(case='circle'):
    """ Generate test data for periodic spline interpolation """
    match case:
        case 'circle':
            nx = 5
            x = np.array([1.0, 0.0, -1.0, 0.0, 1.0])
            y = np.array([0.0, 1.0, 0.0, -1.0, 0.0])
        case _:
            nx = 10
            x = np.array([2.0, 5.0, 8.0, 9.0, 7.0, 6.0, 8.0, 10.0, 7.0, 2.0])
            y = np.array([4.0, 0.5, 3.0, 6.0, 7.0, 5.0, 4.0,  6.0, 8.0, 4.0])

    t = np.linspace(0.0, 1.0, nx)

    return t, x, y


@mpg.plotter
def main():

    t, x, y = test_data('arbitrary')

    spline_x = cubinterpp.NaturalPeriodicSpline1D(t, x)
    spline_y = cubinterpp.NaturalPeriodicSpline1D(t, y)
    t_interp = np.linspace(0.0, 1.0, 500)
    x_interp = spline_x.evaln(t_interp)
    y_interp = spline_y.evaln(t_interp)

    x_interp_scipy = CubicSpline(t, x, bc_type='periodic')(t_interp)
    y_interp_scipy = CubicSpline(t, y, bc_type='periodic')(t_interp)

    mpg.figure(title='Periodic Spline Interpolation')
    mpg.plot(x_interp, y_interp)
    mpg.plot(x_interp_scipy, y_interp_scipy, style='--')
    mpg.plot(x, y, width=0, symbol='o', symbol_color='r', symbol_size=6)
    mpg.legend('cubinterpp', 'scipy', 'data points')
    mpg.gca().grid = True


if __name__ == '__main__':
    main()