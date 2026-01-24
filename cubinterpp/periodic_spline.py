""" Simple test for Periodic Spline Interpolation """

import numpy as np
import mlpyqtgraph as mpg
import cubinterpp.cubinterpp_py as cubinterpp  # cubinterpp_py is a pybind11 module


@mpg.plotter
def main():
    x = np.array([2.0, 5.0, 8.0, 9.0, 7.0, 6.0, 8.0, 10.0, 7.0, 2.0])
    y = np.array([4.0, 0.5, 3.0, 6.0, 7.0, 5.0, 4.0,  6.0, 8.0, 4.0])
    t = np.linspace(0.0, 1.0, 10)

    spline_x = cubinterpp.NaturalPeriodicSpline1D(t, x)
    spline_y = cubinterpp.NaturalPeriodicSpline1D(t, y)
    t_interp = np.linspace(0.0, 1.0, 500)
    x_interp = spline_x.evaln(t_interp)
    y_interp = spline_y.evaln(t_interp)

    mpg.figure(title='Periodic Spline Interpolation')
    mpg.plot(x_interp, y_interp)
    mpg.plot(x, y, width=0, symbol='o', symbol_color='r', symbol_size=6)
    mpg.gca().grid = True


if __name__ == '__main__':
    main()