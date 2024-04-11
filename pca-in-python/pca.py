# https://www.datacamp.com/blog/introduction-to-unsupervised-learning

from sklearn.datasets import load_iris  # Dataset
from sklearn.decomposition import PCA  # Algorithm
import matplotlib.pyplot as plt  # Visualization

# Load the data
iris_data = load_iris(as_frame=True)

# Preview
if False:
    print(type(iris_data))
    # class 'sklearn.utils._bunch.Bunch'
    print(type(iris_data.data))
    # class 'pandas.core.frame.DataFrame'

    print(iris_data)
    print(iris_data.data)
    print(iris_data.target)

# Plot input in 3D with color
if True:
    plt.rcParams["figure.figsize"] = [7.00, 3.50]
    plt.rcParams["figure.autolayout"] = True

    fig = plt.figure()
    # 111: number of rows, number of columns, and index
    ax = fig.add_subplot(111, projection="3d")
    sepal_length = iris_data.data["sepal length (cm)"]
    sepal_width = iris_data.data["sepal width (cm)"]
    petal_length = iris_data.data["petal length (cm)"]
    petal_width = iris_data.data["petal width (cm)"]

    ax.scatter(sepal_length, sepal_width, petal_length, c=petal_width)
    plt.show()

# Instantiate PCA with 2 components
#   Why 2? We had 4 initially. How did we come up with 2?
#   Choosing the right number of components: https://www.baeldung.com/cs/pca
pca = PCA(n_components=2)

# Train the model
#   This step doesn't seem to be necessary.
#   How long does it take? Doesn't seem like there is a good way other than measuring the system metrics with psutil
# pca.fit(iris_data.data)

iris_data_reduced = pca.fit_transform(iris_data.data)

print(iris_data.data)
print(iris_data_reduced)

# print(iris_data_reduced)
#   Looks like a 2D array
#   What do the 2 dimensions mean? They don't seem to be a subset of the input data.

# Plot data
if True:
    plt.scatter(
        iris_data_reduced[:, 0],
        iris_data_reduced[:, 1],
        # Target is a part of the input
        c=iris_data.target,
    )
# Try 3D plot without color. Doesn't provide as much insight as 2D with color
if False:
    plt.rcParams["figure.figsize"] = [7.00, 3.50]
    plt.rcParams["figure.autolayout"] = True

    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    ax.scatter(iris_data_reduced[:, 0], iris_data_reduced[:, 1], iris_data.target)
plt.show()

print("Done!")
