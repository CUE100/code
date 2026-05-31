import numpy as np
try:
    import nnfs
    from nnfs.datasets import spiral_data
    nnfs.init()
except Exception:
    # Fallback if nnfs is not installed: provide a simple spiral_data generator
    import math
    import numpy as np

    def spiral_data(samples, classes):
        X = np.zeros((samples * classes, 2))
        y = np.zeros(samples * classes, dtype='uint8')
        for class_number in range(classes):
            ix = range(samples * class_number, samples * (class_number + 1))
            r = np.linspace(0.0, 1, samples)
            t = np.linspace(class_number * 4, (class_number + 1)
                            * 4, samples) + np.random.randn(samples) * 0.2
            X[ix] = np.c_[r * np.sin(t * 2.5), r * np.cos(t * 2.5)]
            y[ix] = class_number
        return X, y
# Dense layer


class Layer_Dense:
    # Layer initialization
    def __init__(self, n_inputs, n_neurons):
        # Initialize weights and biases
        self.weights = 0.01 * np.random.randn(n_inputs, n_neurons)
        self.biases = np.zeros((1, n_neurons))


# Forward pass


    def forward(self, inputs):
        # Calculate output values from inputs, weights and biases
        self.output = np.dot(inputs, self.weights) + self.biases


# Create dataset
X, y = spiral_data(samples=100, classes=3)
# Create Dense layer with 2 input features and 3 output values
dense1 = Layer_Dense(2, 3)
# Perform a forward pass of our training data through this layer
dense1.forward(X)
# Let's see output of the first few samples:
print(dense1.output[: 5])
