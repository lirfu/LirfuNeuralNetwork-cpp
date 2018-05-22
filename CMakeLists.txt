cmake_minimum_required(VERSION 3.8)
project(NNpp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        matrix/MatrixDimension.h matrix/MatrixDimension.cpp matrix/Matrix.h
        data/Data.h data/SeparatedData.h data/SimpleData.h
        layers/Layer.h layers/InputLayer.h layers/InnerLayer.h layers/FullyConnectedLayer.h
        functions/IFunction.h functions/DerivativeFunction.h functions/Sigmoid.h functions/Linear.h
        descentmethods/DescendMethod.h descentmethods/VanillaGradientDescend.h
        weightinitializers/WeightInitializer.h weightinitializers/RandomWeightInitializer.h
        NeuralNetwork.h
        matrix/MatrixTests.h
        demos/Demos.h demos/Demos.cpp data/DataUtils.h)

set(EXECUTABLE_FILES main.cpp ${SOURCE_FILES})

add_definitions(-DDEBUG)  # Turns on asserts, extra printouts, ... !!!TURN OFF IN PRODUCTION!!!
add_definitions(-DOUTPUT_RESULTS)

add_executable(NNpp ${EXECUTABLE_FILES})
add_library(nnpp STATIC ${SOURCE_FILES})
