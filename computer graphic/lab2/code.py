from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
fig.canvas.set_window_title('sizonov_aa')

v = numpy.array([[-3, 0, 0], [0, -3, 0], [3, 0, 0], \
              [0, 3, 0], [0, 0, -3], [0, 0, 3]])

ax.scatter(v[:,0], v[:, 1], v[:, 2])

verts = [[v[0], v[3], v[4]] , [v[0], v[3], v[5]],\
        [v[1], v[2], v[4]], [v[1], v[2], v[5]] , \
		[v[0], v[1], v[4]], [v[0], v[1], v[5]], \
		[v[2], v[3], v[4]], [v[2], v[3], v[5]]]

plt.tick_params(axis='x', which='both', bottom='off',  top='off',labelbottom='off')

ax.add_collection(Poly3DCollection(verts, \
    facecolors=(1, 1, 1), linewidths=1, edgecolors='black', alpha=0))
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
ax.grid(False)

plt.axis('scaled')
plt.show()
