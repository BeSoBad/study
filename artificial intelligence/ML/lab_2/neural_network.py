import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.special import *
import time


class NeuralNetwork(object):
    def __init__(self, n_output, n_features, n_hidden=30,
                 l1=0.0, l2=0.0, epochs=500, eta=0.001, 
                 alpha=0.0, decrease_const=0.0, shuffle=True,
                 minibatches=1, random_state=None):
        
        np.random.seed(random_state)
        self.n_output = n_output
        self.n_features = n_features
        self.n_hidden = n_hidden
        self.w1, self.w2 = self._initialize_weights()
        self.l1 = l1
        self.l2 = l2
        self.epochs = epochs
        self.eta = eta
        self.alpha = alpha
        self.decrease_const = decrease_const
        self.shuffle = shuffle
        self.minibatches = minibatches 

    def _encode_labels(self, y, k):
        onehot = np.zeros((k + 1, y.shape[0]))
        for idx, val in enumerate(y):
            onehot[int(val), idx] = 1.0
        return onehot

    def _initialize_weights(self):
        w1 = np.random.uniform(-1.0, 1.0, size = self.n_hidden * (self.n_features + 1))
        w1 = w1.reshape(self.n_hidden, self.n_features + 1)
        w2 = np.random.uniform(-1.0, 1.0, size = self.n_output * (self.n_hidden + 1))
        w2 = w2.reshape(self.n_output, self.n_hidden + 1)
        return w1, w2

    def _sigmoid(self, z):
        return expit(z)

    def _sigmoid_gradient(self, z):
        sg = self._sigmoid(z)
        return sg * (1 - sg)

    def _add_bias_unit(self, X, how="column"):
        if how == "column":
            X_new = np.ones((X.shape[0], X.shape[1] + 1))
            X_new[:, 1:] = X
        elif how == "row":
            X_new = np.ones((X.shape[0] + 1, X.shape[1]))
            X_new[1:, :] = X
        else:
            raise AttributeError("`how` must be `column` or `row`")
        return X_new
            
    def _feedforward(self, X, w1, w2):  # Прямое распространение сигнала
        a1 = self._add_bias_unit(X, how = "column")
        z2 = w1.dot(a1.T)
        
        a2 = self._sigmoid(z2)
        a2 = self._add_bias_unit(a2, how = "row")
        
        z3 = w2.dot(a2)
        a3 = self._sigmoid(z3)
        return a1, z2, a2, z3, a3

    def _L2_reg(self, lambda_, w1, w2):
        return (lambda_ / 2.0) * (np.sum(w1[:, 1:] ** 2)
                                + np.sum(w2[:, 1:] ** 2))
    
    def _L1_reg(self, lambda_, w1, w2):
        return (lambda_ / 2.0) * (np.abs(w1[:, 1:]).sum()
                                + np.abs(w2[:, 1:]).sum()) 

    def _get_cost(self, y_enc, output, w1, w2):
        y_enc = np.array(y_enc)
        term1 = -y_enc * (np.log(output))
        term2 = (1 - y_enc) * np.log(1 - output)
        cost = np.sum(term1 - term2)
        L1_term = self._L1_reg(self.l1, w1, w2)
        L2_term = self._L2_reg(self.l2, w1, w2)
        cost = cost + L1_term + L2_term
        return cost

    def _get_gradient(self, a1, a2, a3, z2, y_enc, w1, w2):  # Подсчёт градиентов
        sigma3 = a3 - y_enc  # Ошибки выходного слоя
        z2 = self._add_bias_unit(z2, how = "row")
        sigma2 = w2.T.dot(sigma3) * self._sigmoid_gradient(z2)
        sigma2 = sigma2[1:, :]
        grad1 = sigma2.dot(a1)
        grad2 = sigma3.dot(a2.T)
        
        grad1[:, 1:] += self.l2 * w1[:, 1:]
        grad1[:, 1:] += self.l1 * np.sign(w1[:, 1:])

        grad2[:, 1:] += self.l2 * w2[:, 1:]
        grad2[:, 1:] += self.l1 * np.sign(w2[:, 1:])
        
        return grad1, grad2
        
    def predict(self, X):
        if len(X.shape) != 2:
            raise AttributeError(
                            "X must be array [n_samples, n_features].\n"
                            "Use X[:, None] to 1 - feature classification"
                            "\n or X[[i]] to 1 - point classification")
        a1, x2, a2, z3, a3 = self._feedforward(X, self.w1, self.w2)
        y_pred = np.argmax(a3, axis=0)
        return y_pred

    def fit(self, X, y, print_progress=False):  # Обучение
        self.cost_ = []
        X_data, y_data = X.copy(), y.copy()
        y_enc = self._encode_labels(y, self.n_output)
        
        delta_w1_prev = np.zeros(self.w1.shape)
        delta_w2_prev = np.zeros(self.w2.shape)
        
        for i in range(self.epochs):
            self.eta /= (1 + self.decrease_const * 1)
            
            if print_progress:
                print("Epoch: " + str(i + 1) + '/' + str(self.epochs))
            
            if self.shuffle:
                idx = np.random.permutation(y_data.shape[0])
                X_data, y_enc = X_data[idx], y_enc[:, idx]
            
            mini = np.array_split(range(y_data.shape[0]), self.minibatches)
            
            for idx in mini:  # Метод обратного распространения ошибки
                a1, z2, a2, z3, a3 = self._feedforward(X_data[idx], self.w1, self.w2)
                cost = self._get_cost(y_enc = y_enc[:, idx], output = a3, w1 = self.w1, w2 = self.w2)
                
                self.cost_.append(cost)
                
                grad1, grad2 = self._get_gradient(a1 = a1, a2 = a2,  # Градиенты слоёв
                                                  a3 = a3, z2 = z2,
                                                  y_enc = y_enc[:, idx],
                                                  w1 = self.w1,
                                                  w2 = self.w2)
                                                  
                delta_w1, delta_w2 = self.eta * grad1, self.eta * grad2
                self.w1 -= (delta_w1 + (self.alpha * delta_w1_prev))
                self.w2 -= (delta_w2 + (self.alpha * delta_w2_prev))
                delta_w1_prev, delta_w2_prev = delta_w1, delta_w2
            
        return self

def get_base():
    tmp = pd.read_csv("databases/Computers.csv", header=None)
    df = pd.DataFrame({" ": tmp[0][1:], "price": tmp[1][1:], "speed": tmp[2][1:],
                            "hd": tmp[3][1:], "ram": tmp[4][1:], "screen": tmp[5][1:],
                            "cd": tmp[6][1:], "multi": tmp[7][1:], "premium": tmp[8][1:],
                            "ads": tmp[9][1:], "trend": tmp[10][1:]})
    return df


if __name__ == '__main__':
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



    print("Data to train done.")

    #   экземпляр класса нейронной сети
    nn = NeuralNetwork(n_output = np.unique(y_train).shape[0],  # выходной слой
                      n_features = X_train.shape[1],  # входной слой (признаки)
                      n_hidden = 4,  # кол-во узлов в скрытом слое
                      l2 = 1.0,
                      l1 = 0.0,
                      epochs = 20,
                      eta = 0.01,
                      alpha = 0.001,
                      decrease_const = 0.0001,
                      shuffle = True,
                      minibatches = 10,
                      random_state = 1)

    a = time.time()
    nn.fit(X_train, y_train, print_progress = False)
    b = time.time()
    print("Time:", b - a)

    # График сходимости
    plt.plot(range(len(nn.cost_)), nn.cost_)
    plt.ylabel("Cost")
    plt.xlabel("Epoches")
    plt.tight_layout()
    plt.show()


    y_test = y[int(len(y) * 0.7):]
    X_test = X[int(len(X) * 0.7):]

    y_test_pred = nn.predict(X_test)
    acc = np.sum(y_test == y_test_pred, axis = 0) / X_test.shape[0]

    print("Accuracy is: %.2f%%" % (acc * 100))










