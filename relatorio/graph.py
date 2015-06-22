import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0, 5, 1)
y = np.sin(x)

plt.bar(x, y, 0.5)
plt.xlabel("Philosopher")
plt.ylabel("Food")
plt.show()

