import pandas as pd
import matplotlib.pyplot as plt


def first_distribution(dataset):
	x, y_max = [], []
	price_dict = dict()
	for i in range(1, len(dataset)):
		price_dict[dataset["trend"][i]] = []
	for i in range(1, len(dataset)):
		price_dict[dataset["trend"][i]].append(int(dataset["price"][i]))

	for i in price_dict.keys():
		x.append(int(i))
		y_max.append(max(price_dict[i]))

	plt.plot(x, y_max, 'b', label='Max price')
	plt.legend()
	plt.xlabel("Month from 1993")
	plt.ylabel("Prices (USD)")
	plt.grid()

	plt.show()


def second_distribution(dataset):
	x, y_max, y_min = [], [], []
	price_dict = dict()
	for i in range(1, len(dataset)):
		price_dict[int(dataset["ram"][i])] = []
	for i in range(1, len(dataset)):
		price_dict[int(dataset["ram"][i])].append(int(dataset["price"][i]))

	print(sorted(price_dict.keys()))
	for i in sorted(price_dict.keys()):
		x.append(int(i))
		y_min.append(min(price_dict[i]))

	plt.plot(x, y_min, label='Min price')
	plt.legend()
	plt.xlabel("RAM (MB)")
	plt.ylabel("Prices (USD)")
	plt.grid()

	plt.show()


if __name__ == '__main__':
	tmp = pd.read_csv("databases/Computers.csv", header=None)
	dataset = pd.DataFrame({" ": tmp[0][1:], "price": tmp[1][1:], "speed": tmp[2][1:],
						"hd": tmp[3][1:], "ram": tmp[4][1:], "screen": tmp[5][1:],
						"cd": tmp[6][1:], "multi": tmp[7][1:], "premium": tmp[8][1:],
						"ads": tmp[9][1:], "trend": tmp[10][1:]})
	second_distribution(dataset)
	first_distribution(dataset)
