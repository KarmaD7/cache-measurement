import sys
import math
import matplotlib.pyplot as plt

filename = sys.argv[1]

file = open(filename, 'r')
data = file.readlines()

size = [data[0] + i for i in range(len(data))]
time = [float(line.split("time:")[-1].split("ms")[0].strip()) for line in data]

plt.xticks(size)
plt.xlabel("n")
plt.ylabel("time/ms")
plt.scatter(size, time)
plt.plot(size, time)
plt.show()