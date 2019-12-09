import matplotlib.pyplot as plt

if __name__ == '__main__':
    f = open('build/dots.txt', 'r')
    data = []
    for line in f:
        data.append(line[0:-1])
    print(data)
    i = 0
    while i < len(data):
        count_dots = int(data[i])
        i += 1
        color = data[i]
        i += 1
        label = data[i]
        i += 1
        type = data[i]
        print(count_dots)
        x, y = [], []
        for j in range(i + 1, i + count_dots + 1):
            d = data[j].split(" ")
            x_d, y_d = float(d[0]), float(d[1])
            x.append(x_d)
            y.append(y_d)
            print(x_d, y_d)
        if type == "plot":
            plt.plot(x, y, color=color, label=label)
        elif type == "scatter":
            plt.scatter(x, y, color=color, label=label)
        plt.grid(True)
        plt.legend()
        i += (count_dots + 1)
    plt.show()