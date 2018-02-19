cmake_minimum_required(VERSION 3.8)
project(NNpp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        MatrixDimension.h MatrixDimension.cpp Matrix.h Matrix.cpp
        Data.h SeparatedData.h SimpleData.h
        layers/Layer.h layers/InputLayer.h layers/InnerLayer.h layers/FullyConnectedLayer.h
        functions/DerivativeFunction.h functions/Sigmoid.h functions/Linear.h
        descentmethods/DescendMethod.h descentmethods/VanillaGradientDescend.h
        weightinitializers/WeightInitializer.h weightinitializers/RandomWeightInitializer.h
        NeuralNetwork.h NeuralNetwork.cpp
        MatrixTests.h
        )

#add_definitions(-DTIMES) # For printing debug outputs. Disable for production.

add_executable(NNpp ${SOURCE_FILES})