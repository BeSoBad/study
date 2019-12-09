from time import time
from collections import Counter
import numpy as np
import matplotlib.pyplot as plt


def load_data():
    base = Counter()
    print("[LOADER] Data started loading...")
    started_time = time()
    with open("databases/search_queries.txt", 'r', encoding='utf8') as file:
        for line in file:
            sentence = line.strip().split("\t")[1].split(" ")
            for word in sentence:
                if len(word) >= 3:
                    base[word] += 1
    end_time = time()
    print("[LOADER] Data loaded successfully. Time:", round(end_time - started_time, 3))

    return base


def plot(stat, count=10):
    x = list(np.array(stat.most_common(count)).T[0])
    y = list(map(int, np.array(stat.most_common(count)).T[1]))

    plt.bar(x, y, color='g')
    plt.ylabel('word frequency')
    plt.title("TOP 15 most common words")
    plt.grid(True)  # линии вспомогательной сетки

    plt.show()


if __name__ == '__main__':
    base = load_data()
    plot(base, count=15)
