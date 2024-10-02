import numpy as np
import matplotlib.pyplot as plt

# Generate 1000 samples from a Gaussian distribution
data = np.random.normal(3, 0.5, 10000)

# Plot the histogram
plt.hist(data, bins=30, alpha=0.7, color='#1bc3a9', edgecolor='white')
plt.xlabel('Events')
plt.ylabel('Entries')
plt.title('Gaussian Histogram')
plt.show()