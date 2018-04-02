## What's this?
Extensible library for neural networks written in C++.

## How to use it?
The basic usage of the library can be found in the [demos/](demos/) folder.

## How does it work?
### Data
Data is represented as matrices with the [Matrix](matrix/Matrix.cpp) class.

These matrices are further wrapped with data wrappers defined in [data/](data/).
They are used to control the learning method and allow cross-validation within the neural network.
`SimpleData` defines data pairs which contain all files in train and test sets (dummy cross-validation), while 
`SeparatedData` uses separated lists for train and for test sets.

The `backpropagate` method expects a list of those data wrappers.
A batch descend is applied on each data wrapper.
This allows you to define how will the learning method from the outside:
* if you place all your data in a single data wrapper, you have defined batch learning
* if you place each data input/output pair in its own data wrapper, you have defined stochastic learning
* if you distribute your data in some other way, you have defined some sort of mini-batch learning

### Layers
The `NeuralNetwork` is the main class defining the structure of the neural network. 
The structure is separated into layers.
It must have 1 `InputLayer` and 1 or more `InnerLayer`-s.

### Learning
The main method for learning is with backpropagation. 
The method `backpropagate` does exactly that.
It expects the learning rate and the data batches (see [Data](/#data)).

Each layer also implements paired methods for getting/setting the weight values, allowing you to use evolutionary 
algorithms as learning algorithms.

## How do I extend it?
Here are some *planned* ideas for extending and how to achieve them.

### Use GPU
Current calculations are implemented on the CPU.
To use the power of your GPU you can simply implement your own version of `Matrix` and the rest of the library 
*should* work with it unknowingly.
You will need to implement all of the methods implemented in `Matrix` to ensure a complete feature support.
The functions that are applied to matrices should be created to use the instruction set of your GPU.

### Add new layers
You can add your own layer simply by implementing `InnerLayer`.
This way it is predicted you could achieve convolutional layers, max-pool, adapters (for serializing weights between 
convolutional and fully-connected layers), ANFIS, LSTMs, etc.

### Add different gradient descend methods
Simply by implementing the [DescendMethod](descentmethods/DescendMethod.h) interface you can define
your own descend methods with momentum, stochastics, etc.

### Add different weight initializers
Simply by implementing the [WeightInitializer](weightinitializers/WeightInitializer.h) interface you can define your
own method for initializing weights.
