import matplotlib.pyplot as plt

# Data for the bar plot
categories = ['Bar1', 'Bar2', 'Bar3', 'Bar4']
values = [3500, 2389, 2298, 4578]

# Plot the bar chart
plt.bar(categories, values)
plt.xlabel('Bars')
plt.ylabel('Values')
plt.title('Bar Plot')
plt.show()