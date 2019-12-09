import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
import time


def get_base():
    tmp = pd.read_csv("databases/Computers.csv", header=None)
    df = pd.DataFrame({" ": tmp[0][1:], "price": tmp[1][1:], "speed": tmp[2][1:],
                       "hd": tmp[3][1:], "ram": tmp[4][1:], "screen": tmp[5][1:],
                       "cd": tmp[6][1:], "multi": tmp[7][1:], "premium": tmp[8][1:],
                       "ads": tmp[9][1:], "trend": tmp[10][1:]})
    return df


df = get_base()
for i in list(df.columns):
    for j in range(1, len(df[i]) + 1):
        if df[i][j] == "yes":
            df[i][j] = 1
        elif df[i][j] == "no":
            df[i][j] = 0

np.random.seed(127)
df = np.random.permutation(df)

y = df.T[8].T
y = np.array(y, dtype='float64')
X = np.vstack((df.T[:1], df.T[1:])).T
X = np.array(X, dtype='float64')

y_train = y[:int(len(y) * 0.7)]
X_train = X[:int(len(X) * 0.7)]

print(y_train.shape)
print(y_train)
print(X_train.shape)
print(X_train)

y_test = y[int(len(y) * 0.7):]
X_test = X[int(len(X) * 0.7):]

LR = LogisticRegression()
a = time.time()
LR.fit(X_train, y_train)
b = time.time()
print("Time:", b - a)

y_test_pred = LR.predict(X_test)
acc = np.sum(y_test == y_test_pred, axis=0) / X_test.shape[0]

print("Accuracy is: %.2f%%" % (acc * 100))


