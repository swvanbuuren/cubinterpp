""" Generates test data for cubinterpp tests """

import numpy as np
from scipy.interpolate import RegularGridInterpolator


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


def scipy_linear_interp_1d(x, f, x_fine):
    interp2 = RegularGridInterpolator((x, ), f)
    return interp2((x_fine, ))


def scipy_linear_interp_2d(x, y, f, x_fine, y_fine):
    interp2 = RegularGridInterpolator((x, y), f)
    x_grid, y_grid = np.meshgrid(x_fine, y_fine, indexing='ij')
    return interp2((x_grid, y_grid))


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


def generate_1d_example(case='akima', size_fine=15):
    x, f = get_test_data(case=case)
    x_fine = refine_grid(x, size_fine=size_fine)
    f_fine = scipy_linear_interp_1d(x, f, x_fine)

    print_cpp_vector('Vector', 'x', x)
    print_cpp_vector('Vector', 'f', f)
    print_cpp_vector('Vector', 'x_fine', x_fine)
    print_cpp_vector('Vector', 'f_fine', f_fine)


def generate_2d_example(case='normalized', size_fine=5):
    x, y, f = get_test_data_2d(case=case)
    x_fine = refine_grid(x, size_fine=size_fine)
    y_fine = refine_grid(y, size_fine=size_fine)
    f_fine = scipy_linear_interp_2d(x, y, f, x_fine, y_fine)

    print_cpp_vector('Vector', 'x', x)
    print_cpp_vector('Vector', 'y', y)
    print_cpp_vector('Vector2', 'f', f)
    print_cpp_vector('Vector', 'x_fine', x_fine)
    print_cpp_vector('Vector', 'y_fine', y_fine)
    print_cpp_vector('Vector2', 'f_fine', f_fine)


def main():
    generate_1d_example(case='random', size_fine=10)
    # generate_2d_example(case='normalized', size_fine=5)


if __name__ == '__main__':
    main()