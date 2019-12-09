from collections import defaultdict
from matplotlib import mlab
import bisect, pylab, math
import matplotlib.pyplot as plt
import numpy as np
 
def drange(start, stop, step):
    while start < stop: yield start; start += step;
 
class Dot:
    def __init__(self, x, y): self.x, self.y = [x, y]
 
class Tuple: a, b, c, d, x = [0., 0., 0., 0., 0.]
 
def buildSpline(dots):
    for i in range(len(dots)): splines[i].x, splines[i].a = dots[i].x, dots[i].y
 
    alpha, beta = [defaultdict(lambda: 0.), defaultdict(lambda: 0.)]
 
    for i in range(1, len(dots)-1):
        C = 4. * in_step
        F = 6. * ((dots[i + 1].y - dots[i].y) / in_step - (dots[i].y - dots[i - 1].y) / in_step)
        z = (in_step* alpha[i - 1] + C)
        alpha[i] = -in_step / z
        beta[i] = (F - in_step* beta[i - 1]) / z
 
    for i in reversed(range(1, len(dots) - 1)): splines[i].c = alpha[i] * splines[i+1].c + beta[i]
 
    for i in reversed(range(1, len(dots))):
        hi = dots[i].x - dots[i-1].x
        splines[i].d = (splines[i].c - splines[i-1].c) / hi
        splines[i].b = hi * (2.0 * splines[i].c + splines[i - 1].c) / 6.0 + (dots[i].y - dots[i-1].y) / hi
 
def calc(x):
    distribution = sorted([t[1].x for t in splines.items()])
    indx = bisect.bisect_left(distribution, x)
    if indx == len(distribution): return 0
    dx = x - splines[indx].x
    return splines[indx].a + splines[indx].b * dx + splines[indx].c * dx**2 / 2. + splines[indx].d * dx**3 / 6.
#============================================
 
in_func =  lambda x:  math.sin(x)
in_min_x = 0
in_max_x = 25 
in_step = 2.5
 
out_min_x = 0
out_max_x = 25
out_step = 0.1
 
#============================================
 
#build model
splines = defaultdict(lambda: Tuple())
buildSpline(list(map(lambda x: Dot(x, in_func(x)), [x for x in drange(in_min_x, in_max_x+1, in_step)])))

#print result
for x in drange(out_min_x, out_max_x, out_step):
    print(str(x) + ';' + str(calc(x)))
 
#build graphics
#xlist = mlab.frange(out_min_x, out_max_x, out_step)
xlist = np.linspace(out_min_x, out_max_x, 500)
plt.plot(xlist, [calc(x) for x in xlist])
plt.plot(xlist, [in_func(x) for x in xlist])
plt.show()