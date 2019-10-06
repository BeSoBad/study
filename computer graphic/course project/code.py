from math import cos, pi, sin, log
import matplotlib.pyplot
import numpy 
from mpl_toolkits import mplot3d
from mpl_toolkits.mplot3d.art3d import Line3DCollection, Poly3DCollection

def zoom_factory(ax, base_scale=2.):
    def zoom_fun(event):
        cur_xlim = ax.get_xlim()
        cur_ylim = ax.get_ylim()
        cur_xrange = (cur_xlim[1] - cur_xlim[0]) * .5
        cur_yrange = (cur_ylim[1] - cur_ylim[0]) * .5
        xdata = event.xdata
        ydata = event.ydata 
        if event.button == 'up':
            scale_factor = 1 / base_scale
        elif event.button == 'down':
            scale_factor = base_scale
        else:
            scale_factor = 1
            print(event.button)
        ax.set_xlim([
            xdata - cur_xrange * scale_factor,
            xdata + cur_xrange * scale_factor
        ])
        ax.set_ylim([
            ydata - cur_yrange * scale_factor,
            ydata + cur_yrange * scale_factor
        ])
        matplotlib.pyplot.draw() 

    fig = ax.get_figure() 
    fig.canvas.mpl_connect('scroll_event', zoom_fun)
    return zoom_fun

e = 50
a = 0
b = 1000
ell = []
points = []
for i in range(a, b, e):
    points = []
    for j in range(a, b, e):
        F = (20*(1000 - i) + 30*i)*(1000 - j) + (20*(1000 - i) + 30*i)*j
        points.append((i, j, (i+j+i*j)/100))
    points = numpy.array(points)
    ell.append(points)

verts = []
for i in range(len(ell) - 1):
    for j in range(len(ell[i]) - 1):
        verts.append([
            ell[i][j], ell[(i + 1) % len(ell)][j],
            ell[(i + 1) % len(ell)][(j + 1) % len(ell[i])],
            ell[i][(j + 1) % len(ell[i])]
        ])
        
fig = matplotlib.pyplot.figure()
ax = fig.add_subplot(111, projection='3d')
matplotlib.pyplot.xlabel('x')
matplotlib.pyplot.ylabel('y')
matplotlib.pyplot.axis('off')

ax.set_xlim([-800, 800])
ax.set_ylim([-100, 800])
ax.set_zlim([-800, 800])

scale = 1.5
f = zoom_factory(ax, base_scale=scale)
ax.add_collection3d(
    Poly3DCollection(
        verts,
        facecolor="white",
        linewidths=1.3,
        edgecolor=(0, 0, 1)))

matplotlib.pyplot.show()
