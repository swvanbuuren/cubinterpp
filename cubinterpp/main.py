""" Simple test for Akima Spline Interpolation """

import numpy as np
from scipy.interpolate import RegularGridInterpolator
import mlpyqtgraph as mpg
import cubinterpp_py as cubinterpp  # cubinterpp_py is a pybind11 module


def get_test_data(case='akima', start=1.0, end=5.0, size=8):
    """ Generates test input data for Akima Spline tests """
    if case == 'akima':
        return np.array([1, 2, 3, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10]), \
               np.array([0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6])

    if case == 'random':
        return np.linspace(start, end, num=size), \
               np.round(10.0 * np.random.rand(size))

    return (start, end), (0, 1)


def get_test_data_2d(case='standard'):
    """ Generate 2D test data """
    match case:
        case 'normalized':
            x = np.array([0.0, 1.0, 2.0])
            y = np.array([0.0, 1.0, 2.0])
            f = np.array([[1.0, 2.0, 2.0],
                        [2.0, 3.0, 3.0],
                        [3.0, 3.0, 4.0]])
        case 'standard':
            x = np.array([0.0, 1.5, 3.0])
            y = np.array([0.0, 2.0, 4.0])
            f = np.array([[1.0, 2.0, 2.0],
                        [2.0, 3.0, 3.0],
                        [3.0, 3.0, 4.0]])
        case 'non-monotonic':
            x = np.array([0.0, 1, 1.5])
            y = np.array([0.0, 0.5, 3.0])
            f = np.array([[1.0, 2.0, 2.0],
                        [2.0, 3.0, 3.0],
                        [3.0, 3.0, 4.0]])
    return x, y, f


def refine_grid(x_coord, size_fine=1000, extension=0):
    """ Refines x grid and provide placeholder data for y """
    return np.linspace(x_coord[0] - extension, x_coord[-1] + extension,
                       num=size_fine)


def scipy_linear_interp(x, y, f, x_fine, y_fine):
    interp2 = RegularGridInterpolator((x, y), f)
    x_grid, y_grid = np.meshgrid(x_fine, y_fine, indexing='ij')
    return interp2((x_grid, y_grid))


@mpg.plotter(projection='orthographic')
def main():
    """ Tets Cubic spline interpolation """

    x, y = get_test_data(case='akima')
    x_fine = refine_grid(x)

    spline = cubinterpp.LinearInterp1D(x, y)
    y_fine_linear = spline.evaln(x_fine)

    spline = cubinterpp.NaturalSpline1D(x, y)
    y_fine_natural = spline.evaln(x_fine)

    spline = cubinterpp.AkimaSpline1D(x, y)
    y_fine_akima = spline.evaln(x_fine)

    spline = cubinterpp.MonotonicSpline1D(x, y)
    y_fine_monotonic = spline.evaln(x_fine, )

    mpg.figure(title='Test figure')
    mpg.plot(x_fine, y_fine_linear)
    mpg.plot(x_fine, y_fine_monotonic)
    mpg.plot(x_fine, y_fine_akima)
    mpg.plot(x_fine, y_fine_natural)
    mpg.plot(x, y, width=0, symbol='o', symbol_color='r', symbol_size=6)
    mpg.gca().grid = True
    mpg.legend(
        'Linear interpolation',
        'Monotonic cubic interpolation',
        'Akima spline',
        'Natural cubic spline',
        'data points'
    )

    x, y, f = get_test_data_2d(case='non-monotonic')
    interp2 = cubinterpp.LinearInterp2D(x, y, f)
    x_fine, y_fine = refine_grid(x, 20), refine_grid(y, 20)
    z_fine = np.zeros((len(x_fine), len(y_fine)))
    for xi, xf in enumerate(x_fine):
        for yi, yf in enumerate(y_fine):
            z_fine[xi, yi] = interp2.eval(xf, yf)

    mpg.figure(title='Test figure', layout_type='Qt')
    mpg.surf(x_fine, y_fine, z_fine)
    ax = mpg.gca()
    ax.azimuth = 225

    z_fine_scipy = scipy_linear_interp(x, y, f, x_fine, y_fine)
    mpg.figure(title='Test figure compare', layout_type='Qt')
    mpg.surf(x_fine, y_fine, z_fine_scipy)
    ax = mpg.gca()
    ax.azimuth = 225


if __name__ == '__main__':
    main()
