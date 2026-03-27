""" Compare 1D Spline Interpolation types """

import numpy as np
import mlpyqtgraph as mpg
import cubinterpp.cubinterpp_py as cubinterpp  # cubinterpp_py is a pybind11 module


def get_test_data(case='makima', start=1.0, end=5.0, size=8):
    """ Generates test input data for Modified Akima Spline tests """
    if case == 'makima':
        return np.array([1, 2, 3, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10]), \
               np.array([0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6])

    if case == 'random':
        return np.linspace(start, end, num=size), \
               np.round(10.0 * np.random.rand(size))

    return (start, end), (0, 1)


def refine_grid(x_coord, size_fine=1000, extension=0):
    """ Refines x grid and provide placeholder data for y """
    return np.linspace(x_coord[0] - extension, x_coord[-1] + extension,
                       num=size_fine)


@mpg.plotter(projection='orthographic')
def main():
    """ Tets Cubic spline interpolation """

    x, y = get_test_data(case='makima')
    x_fine = refine_grid(x)

    spline = cubinterpp.LinearInterp1D(x, y)
    y_fine_linear = spline.evaln(x_fine)

    spline = cubinterpp.NaturalCubicInterp1D(x, y)
    y_fine_natural = spline.evaln(x_fine)

    spline = cubinterpp.MakimaCubicInterp1D(x, y)
    y_fine_makima = spline.evaln(x_fine)

    spline = cubinterpp.MonotonicCubicInterp1D(x, y)
    y_fine_monotonic = spline.evaln(x_fine)

    mpg.figure(title='Test figure')
    mpg.plot(x_fine, y_fine_linear)
    mpg.plot(x_fine, y_fine_monotonic)
    mpg.plot(x_fine, y_fine_makima)
    mpg.plot(x_fine, y_fine_natural)
    mpg.plot(x, y, width=0, symbol='o', symbol_color='r', symbol_size=6)
    mpg.gca().grid = True
    mpg.legend(
        'Linear interpolation',
        'Monotonic cubic interpolation',
        'Modified Akima spline',
        'Natural cubic spline',
        'data points'
    )


if __name__ == '__main__':
    main()
