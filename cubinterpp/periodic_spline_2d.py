""" Periodic spline example in 3D """

import numpy as np
import cubinterpp.cubinterpp_py as cubinterpp  # cubinterpp_py is a pybind11 module
import mlpyqtgraph as mpg


@mpg.plotter(projection='orthographic')
def main():
    """ Interpolation of a torus to demonstrate periodic splines in 2D """
    nx, ny = 5, 5
    tx = np.linspace(0.0, 1.0, nx)
    ty = np.linspace(0.0, 1.0, ny)
    # generate torus points
    r_major = 3.0  # major radius
    r_minor = 1.0  # minor radius

    x = np.zeros((nx, ny))
    y = np.zeros((nx, ny))
    z = np.zeros((nx, ny))
    for i in range(nx):
        for j in range(ny):
            u = 2.0 * np.pi * tx[i]
            v = 2.0 * np.pi * ty[j]
            x[i, j] = (r_major + r_minor * np.cos(v)) * np.cos(u)
            y[i, j] = (r_major + r_minor * np.cos(v)) * np.sin(u)
            z[i, j] = r_minor * np.sin(v)

    # create periodic splines in 2D
    spline_x = cubinterpp.NaturalPeriodicSpline2D(tx, ty, x)
    spline_y = cubinterpp.NaturalPeriodicSpline2D(tx, ty, y)
    spline_z = cubinterpp.NaturalPeriodicSpline2D(tx, ty, z)
    # evaluate on finer grid
    nxi, nyi = 33, 33
    txi = np.linspace(0.0, 1.0, nxi)
    tyi = np.linspace(0.0, 1.0, nyi)
    x_fine = np.zeros((nxi, nyi))
    y_fine = np.zeros((nxi, nyi))
    z_fine = np.zeros((nxi, nyi))
    for i in range(nxi):
        for j in range(nyi):
            x_fine[i, j] = spline_x.eval(txi[i], tyi[j])
            y_fine[i, j] = spline_y.eval(txi[i], tyi[j])
            z_fine[i, j] = spline_z.eval(txi[i], tyi[j])


    mpg.figure(title='Periodic Spline Interpolation of a Torus')
    mpg.surf(x_fine, y_fine, z_fine)
    ax = mpg.gca()
    ax.azimuth = 225
    ax.xlim = [-4, 4]
    ax.ylim = [-4, 4]
    ax.zlim = [-2.5, 2.5]
    xp = x.flatten()
    yp = y.flatten()
    zp = z.flatten()
    mpg.points3(xp, yp, zp, color=(0.8, 0.1, 0.1, 1), size=5)

if __name__ == '__main__':
    main()