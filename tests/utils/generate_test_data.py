""" Generates test data for cubinterpp tests """

import numpy as np
from scipy.interpolate import RegularGridInterpolator, CubicSpline


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
        case 'akima':
            x = np.array([1, 2, 3, 4.0, 5.0, 5.5, 7.0, 8.0, 9.0, 9.5, 10])
            y = x
            f = np.array([[0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.6],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.3, 0.3],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.0, 0.0, 0.0],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 0.1, 0.1, 0.1, 0.1],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2],
                          [0, 0, 0, 0.5, 0.4, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2],
                          [0, 0, 0, 0.5, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4],
                          [0, 0, 0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5],
                          [0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                          [0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                          [0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]])
            f *= 5 + 1
    return x, y, f


def get_test_data_3d(case='standard'):
    """ Generate 2D test data """
    match case:
        case 'normalized':
            x = np.array([0.0, 1.0, 2.0])
            y = np.array([0.0, 1.0, 2.0])
            z = np.array([0.0, 1.0, 2.0])
            # generate some similar 3d data for f as for 2d data
            f = np.array([
                          [
                           [1.0, 2.0, 2.0],
                           [2.0, 3.0, 3.0],
                           [3.0, 3.0, 4.0]
                          ],
                          [
                           [3.0, 4.0, 4.0],
                           [4.0, 5.0, 5.0],
                           [5.0, 5.0, 6.0]
                          ],
                          [
                           [2.0, 3.0, 3.0],
                           [3.0, 4.0, 4.0],
                           [4.0, 4.0, 5.0]
                          ]
                         ])
    return x, y, z, f


def refine_grid(x_coord, size_fine=1000, extension=0):
    """ Refines x grid and provide placeholder data for y """
    return np.linspace(x_coord[0] - extension, x_coord[-1] + extension,
                       num=size_fine)


def scipy_interp_1d(x, f, x_fine, method='linear'):
    interp2 = RegularGridInterpolator((x, ), f, method=method)
    return interp2((x_fine, ))


def scipy_interp_2d(x, y, f, x_fine, y_fine, method='linear'):  # noqa: PLR0913, PLR0917
    interp2 = RegularGridInterpolator((x, y), f, method=method)
    x_grid, y_grid = np.meshgrid(x_fine, y_fine, indexing='ij')
    return interp2((x_grid, y_grid))


def scipy_interp_3d(x, y, z, f, x_fine, y_fine, z_fine, method='linear'):  # noqa: PLR0913, PLR0917
    interp2 = RegularGridInterpolator((x, y, z), f, method=method)
    x_grid, y_grid, z_grid = np.meshgrid(x_fine, y_fine, z_fine, indexing='ij')
    return interp2((x_grid, y_grid, z_grid))


def cpp_array(array, indent=0):
    """ Prints a nested array in C++ syntax """
    yield (" " * indent) + "{ "
    first = True
    for value in array:
        if isinstance(value, list) or isinstance(value, np.ndarray):
            if not first:
                yield ", "
            yield "\n"
            yield from cpp_array(value, indent=indent + 2)
            printed_list = True
        else:
            if not first:
                yield ", "
            yield str(value)
            printed_list = False
        if first:
            first = False
    if printed_list:
        yield "\n}"
    else:
        yield " }"


def print_cpp_vector(vector_type, name, array):
    """ Prints a nested array in C++ syntax """
    array_str = "".join(cpp_array(array))
    lhs = f'{vector_type} {name}'
    indentation = len(lhs) + 3
    base_str = '\n' + (' ' * indentation)
    array_str = base_str.join(array_str.split('\n'))
    rhs = f'{array_str};'
    print(f'{lhs} = {rhs}')


def generate_1d_example(
        case='akima',
        size_fine=15,
        method='linear',
        bc_type='natural',
    ):
    x, f = get_test_data(case=case)
    x_fine = refine_grid(x, size_fine=size_fine)
    if method == 'cubic_spline':
        print('Using CubicSpline')
        spline = CubicSpline(x, f, bc_type=bc_type)
        f_fine = spline(x_fine)
    else:
        f_fine = scipy_interp_1d(x, f, x_fine, method=method)

    print_cpp_vector('cip::Vector', 'x', x)
    print_cpp_vector('cip::Vector', 'f', f)
    print_cpp_vector('cip::Vector', 'x_fine', x_fine)
    print_cpp_vector('cip::Vector', 'f_fine', f_fine)


def generate_2d_example(case='normalized', size_fine=5, method='linear'):
    x, y, f = get_test_data_2d(case=case)
    x_fine = refine_grid(x, size_fine=size_fine)
    y_fine = refine_grid(y, size_fine=size_fine)
    f_fine = scipy_interp_2d(x, y, f, x_fine, y_fine, method=method)

    print_cpp_vector('cip::Vector', 'x', x)
    print_cpp_vector('cip::Vector', 'y', y)
    print_cpp_vector('cip::Vector2', 'f', f)
    print_cpp_vector('cip::Vector', 'x_fine', x_fine)
    print_cpp_vector('cip::Vector', 'y_fine', y_fine)
    print_cpp_vector('cip::Vector2', 'f_fine', f_fine)


def generate_3d_example(case='normalized', size_fine=5, method='linear'):
    x, y, z, f = get_test_data_3d(case=case)
    x_fine = refine_grid(x, size_fine=size_fine)
    y_fine = refine_grid(y, size_fine=size_fine)
    z_fine = refine_grid(z, size_fine=size_fine)
    f_fine = scipy_interp_3d(x, y, z, f, x_fine, y_fine, z_fine, method=method)

    print_cpp_vector('cip::Vector', 'x', x)
    print_cpp_vector('cip::Vector', 'y', y)
    print_cpp_vector('cip::Vector', 'z', z)
    print_cpp_vector('cip::Vector3', 'f', f)
    print_cpp_vector('cip::Vector', 'x_fine', x_fine)
    print_cpp_vector('cip::Vector', 'y_fine', y_fine)
    print_cpp_vector('cip::Vector', 'z_fine', z_fine)
    print_cpp_vector('cip::Vector3', 'f_fine', f_fine)


def main():
    method = 'cubic_spline'
    data_case = 'akima'
    # data_case = 'normalized'
    generate_1d_example(case=data_case, size_fine=20, method=method,
                        bc_type='not-a-knot')
    # generate_2d_example(case=data_case, size_fine=5, method=method)
    # generate_3d_example(case=data_case, size_fine=5, method=method)


if __name__ == '__main__':
    main()
