import numpy as np
import matplotlib.pyplot as plt

# x axis bins from 0 to 10
x = np.arange(1, 11)

y = 1e6 / (x ** 2) #inverse power law function
y_err = 0.1 * y    #10% uncertainty #UNCERTAINTY IS SQRT OF VALUE #SUBSAMPLING IS OPTIONAL BUT PREFERRED

#plot
plt.errorbar(x, y, yerr=y_err, fmt='o', label='Data with uncertainty', color='#00cc99', ecolor='#00cc99', capsize=5)
plt.xlabel('x'),
plt.ylabel('y')
plt.yscale('log') # Set y-axis to logarithmic scale
plt.title('Box Plot with Statistical Uncertainty (Logarithmic y-axis)')
plt.grid(True)
plt.legend()
plt.show()