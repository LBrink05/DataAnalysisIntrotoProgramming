import numpy as np
import matplotlib.pyplot as plt

# Generate time data
time = np.linspace(0, 10, 100)
line = np.sin(2 * np.pi * time)

# Plot the graph
plt.plot(time, line)
plt.xlabel('time (seconds)')
plt.ylabel('line (meters)')
plt.title('Sinusoidal Function')
plt.grid(True)
plt.show()