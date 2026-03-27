""" Periodic spline example in 3D """

from pathlib import Path
import random
import numpy as np
from PIL import Image
import cubinterpp.cubinterpp_py as cubinterpp  # cubinterpp_py is a pybind11 module
import mlpyqtgraph as mpg


@mpg.plotter(projection='orthographic')
def main():
    """ Interpolation of a torus to demonstrate periodic splines in 2D """
    nx, ny = 9, 9
    tx = np.linspace(0.0, 1.0, nx)
    ty = np.linspace(0.0, 1.0, ny)
    # generate torus points
    r_major = 3.0  # major radius
    r_minor = 1.0  # minor radius

    random.seed(43)
    x = np.zeros((nx, ny))
    y = np.zeros((nx, ny))
    z = np.zeros((nx, ny))
    for i in range(nx):
        for j in range(ny):
            u = 2.0 * np.pi * tx[i]
            v = 2.0 * np.pi * ty[j]
            x[i, j] = (r_major + r_minor * np.cos(v)) * np.cos(u)
            y[i, j] = (r_major + r_minor * np.cos(v)) * np.sin(u)
            if i == nx - 1:
                z[i, j] = z[0, j]
            else:
                z[i, j] = (1.0 + random.randint(-2, +2)/8.0) * r_minor * np.sin(v)

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


    mpg.figure(title='Periodic Spline Interpolation of a distorted Torus')
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
    ax.distance = 115
    ax.elevation = 30
    ax.update()
    filepath = Path(__file__).parents[1] / 'docs' / 'images' / 'periodic_spline_2D.png'
    ax.export(str(filepath))


def generate_movie(ax):
    """ Render a 360° rotation of the current axes as an animated GIF """
    filepath = Path(__file__).parents[1] / 'docs' / 'images' / 'periodic_spline_2D.png'
    frame_paths = []
    for i in range(0, 360, 3):
        ax.azimuth = i
        ax.update()
        frame_path = filepath.parent / f"{filepath.stem}_{i}{filepath.suffix}"
        ax.export(str(frame_path))
        frame_paths.append(frame_path)

    # Assemble animated GIF and remove intermediate frames
    images = [Image.open(p) for p in frame_paths]
    gif_path = filepath.with_suffix('.gif')
    images[0].save(
        gif_path,
        save_all=True,
        append_images=images[1:],
        loop=0,
        duration=40,  # ms per frame
    )
    for img in images:
        img.close()
    for p in frame_paths:
        p.unlink()


if __name__ == '__main__':
    main()