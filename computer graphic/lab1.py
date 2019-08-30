import sys
import string
import matplotlib
import matplotlib.pyplot
import numpy 
x_value = list()
y_value = list()
yy_value = list()
xx_value = list()
def func(a, x):
    return numpy.sqrt(x**2 * numpy.sqrt((a - x) / (a + x)))
def funcc(a,x):
    return -numpy.sqrt(x**2 * numpy.sqrt((a-x)/(a+x))) 

A = float(input("Print A: "))
B = float(input("Print B: "))
a = int(input("Print a: "))
i = A
while i <= B:
    x_value.append(i)
    y_value.append(func(int(a), i))
    i += 0.001
    
matplotlib.pyplot.plot(x_value, y_value, 'black')
matplotlib.pyplot.xlabel('x axis')
matplotlib.pyplot.ylabel('y axis')
i = A
while i <= B:
    xx_value.append(i)
    yy_value.append(funcc(int(a), i))
    i += 0.001
matplotlib.pyplot.plot(xx_value, yy_value,'black')

matplotlib.pyplot.show()
