""" Compare 2D Spline Interpolation types """

from pathlib import Path
import numpy as np
import mlpyqtgraph as mpg
import cubinterpp.cubinterpp_py as cubinterpp  # cubinterpp_py is a pybind11 module


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
        case 'makima':
            x = np.array([1, 2, 3, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10])
            y = np.array([1, 2, 3, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10])
            f = np.array([[0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                          [0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                          [0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                          [0, 0, 0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5],
                          [0, 0, 0, 0.5, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.1, 0.1, 0.1],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.0, 0.0],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.3],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6]])
            f *= 5 + 1
        case 'three_bumps':
            x = np.linspace(1, 10, 10)
            y = np.linspace(1, 10, 10)
            f = np.zeros((10, 10))
            f[1:5, 1:5] = 0.45
            f[5:7, 5:7] = np.array([[0.25, 0.2], [0.2, 0.25]])
            f[7:9, 7:9] = 0.5
            f = 10 * np.flip(f, axis=0) + 1
    return np.array(x), np.array(y), np.array(f)


def refine_grid(x_coord, size_fine=1000, extension=0):
    """ Refines x grid and provide placeholder data for y """
    return np.linspace(x_coord[0] - extension, x_coord[-1] + extension,
                       num=size_fine)


def cubinterpp_interp2(interp_type='linear', data_case='makima', refinement=50):
    """ Short hand for 2D interpolation with cubinterpp """
    x, y, f = get_test_data_2d(case=data_case)
    match interp_type:
        case 'linear':
            interp2 = cubinterpp.LinearInterp2D(x, y, f)
        case 'monotonic':
            interp2 = cubinterpp.MonotonicSpline2D(x, y, f)
        case 'makima':
            interp2 = cubinterpp.MakimaSpline2D(x, y, f)
        case 'natural_spline':
            interp2 = cubinterpp.NaturalSpline2D(x, y, f)
    x_fine, y_fine = refine_grid(x, refinement), refine_grid(y, refinement)
    z_fine = np.zeros((len(x_fine), len(y_fine)))
    for xi, xf in enumerate(x_fine):
        for yi, yf in enumerate(y_fine):
            z_fine[xi, yi] = interp2.eval(xf, yf)
    return x_fine, y_fine, z_fine


@mpg.plotter(projection='orthographic')
def main():
    """ Compare 2D cubic spline interpolation methods """

    x, y, f = get_test_data_2d(case='three_bumps')
    xp = np.repeat(x, y.size)
    yp = np.tile(y, x.size)
    zp = f.flatten()

    for interp_type in ('linear', 'monotonic', 'makima', 'natural_spline'):
        x_fine, y_fine, z_fine = cubinterpp_interp2(
            interp_type=interp_type,
            data_case='three_bumps',
            refinement=46
        )
        title = interp_type.capitalize().replace('_', ' ')
        mpg.figure(title=f'{title} interpolation')
        mpg.surf(x_fine, y_fine, z_fine)
        ax = mpg.gca()
        ax.azimuth = 225
        ax.xticks = 8
        ax.yticks = 8
        ax.zlim = [0, 8]
        mpg.points3(xp, yp, zp, color=(0.8, 0.1, 0.1, 1), size=3)
        ax.distance = 115
        filepath = Path(__file__).parents[1] / 'docs' / 'images' / f'{interp_type}_2D.png'
        ax.export(str(filepath))


if __name__ == '__main__':
    main()
