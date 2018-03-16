cmake_minimum_required(VERSION 3.8)
project(NNpp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES demo.cpp
        matrix/MatrixDimension.h matrix/MatrixDimension.cpp matrix/Matrix.h matrix/Matrix.cpp
        data/Data.h data/SeparatedData.h data/SimpleData.h
        layers/Layer.h layers/InputLayer.h layers/InnerLayer.h layers/FullyConnectedLayer.h
        functions/DerivativeFunction.h functions/Sigmoid.h functions/Linear.h
        descentmethods/DescendMethod.h descentmethods/VanillaGradientDescend.h
        weightinitializers/WeightInitializer.h weightinitializers/RandomWeightInitializer.h
        NeuralNetwork.h NeuralNetwork.cpp
        matrix/MatrixTests.h
        matrix/IMatrix.h)

#add_definitions(-DTIMERS) # For printing timers. Disable for production!
#add_definitions(-DDEBUG) # For printing in iterations. Disable for production!

add_executable(NNpp ${SOURCE_FILES})