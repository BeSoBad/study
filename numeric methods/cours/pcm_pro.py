from numpy import linalg
import matplotlib.pyplot as plt
import numpy
from sympy import diff
import sys
import logging
from math import *
# останавливается на первом решении
# не находит комплексные корни
# начальное приближение должно быть близко к ответу

def feval(expr, x):
    return eval(expr)


def PC_method(F, DF, x0, dt, eps):
    t = dt
    x = x0
    tout = [t]
    xout = [x0]
    n = 1
    dxout = [0 for i in range(0, n)]
    m = 0
    it = 0
    while t <= 1:
        ndx = 1
        nh = 1
        dx = 1
        while max(ndx, nh) > eps:
            h = -feval(F, x0) * t
            dx = h / feval(DF, x0)
            x += dx
            m += 1
            ndx = linalg.norm(dx)
            nh = linalg.norm(h)
            if m > 10:
                # print(max(ndx, nh), dt)
                t = t - dt
                dt = dt / 2
                t = t + dt
                x = x0
                m = 0
        x0 = x
        tout.append(t)
        xout.append(x)
        dxout.append(dx)
        if m < 4:
            dt = dt * 2
        t = dt + t
        it = it + 1
        if it % 2000 == 0:
            print("iterations:", it)
            #sys.exit(1)
    print("it:", it)
    print("t:", t)
    return xout, dxout, m, x, it, t


if __name__ == '__main__':
    logging.basicConfig(filename="log.txt", level=logging.INFO)
    full_expr = input("Enter equation (F(x) = 0): ").split("=")
    expr = full_expr[0]
    if len(full_expr) > 1:
        expr += "-" + full_expr[1]
    print("Transformed equation: " + expr + " = 0")
    d_expr = None
    try:
        d_expr = diff(expr)
    except:
        print("Error expression")
        sys.exit(1)
    dt = 0.1
    x0 = float(input("Enter the initial approximation (x0) (for example 0.5): "))
    eps = float(input("Enter a valid error (epsilon) (for example 0.01): "))
    xout, dxout, m, x, it, t = PC_method(expr, str(d_expr), x0, dt, eps)
    logging.info(xout)
    logging.info(dxout)
    logging.info("lens: {0} {1}".format(len(xout), len(dxout)))
    segment = numpy.linspace(0, 1, num=len(xout), endpoint=True, retstep=False, dtype=None)
    #logging.info(segment)
    logging.info(m)

    plt.subplot(2, 1, 1)
    plt.title("equation: "+ expr + " = 0\n"
                + "x = {0}\n".format(str(x))
                + "number of iterations: {0}".format(str(it)))
    #plt.text(0.5, 0.5, 's', horizontalalignment='center', verticalalignment='center')
    plt.plot(segment, xout, color='b', label='Solution')
    plt.grid(True)
    plt.legend()
    plt.subplot(2, 1, 2)
    plt.plot(segment, dxout, color='r', label='Errors')
    plt.grid(True)
    plt.legend()
    plt.show()
    print(str(d_expr))