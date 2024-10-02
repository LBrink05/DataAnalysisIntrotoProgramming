import numpy as np
import matplotlib.pyplot as plt

# Generate 1000 samples from a Gaussian distribution
data1 = np.random.normal(-2,0.2,1000)
data2 = np.random.normal(2,0.2,1000000)

# Plot the histogram
plt.hist(data1, bins=30, alpha=0.7, color='#1bc3a9', edgecolor='white', density=True)
plt.hist(data2, bins=30, alpha=0.7, color='#c31b49', edgecolor='white', density=True)
plt.xlabel('Events')
plt.ylabel('Entries')
plt.title('Gaussian Histogram')
plt.show()