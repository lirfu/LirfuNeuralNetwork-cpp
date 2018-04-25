//
// Created by lirfu on 29.03.18..
//

#ifndef NNPP_NETWORKBUILDER_H
#define NNPP_NETWORKBUILDER_H


#include <memory.h>

#include <utility>
#include "layers/InnerLayer.h"
#include "NeuralNetwork.h"
#include "functions/DerivativeFunction.h"
#include "functions/Linear.h"
#include "functions/Sigmoid.h"
#include "layers/FullyConnectedLayer.h"
#include "weightinitializers/RandomWeightInitializer.h"
#include "descentmethods/VanillaGradientDescend.h"

class NetworkBuilder {
private:
    InputLayer *inputLayer_;
    std::vector<InnerLayer *> layers_;
public:
    enum LayerType {
        FULLY_CONNECTED
    };

    enum Function {
        LINEAR, SIGMOID
    };

    enum Descend {
        VANILLA
    };

    enum Initializer {
        RANDOM
    };

    NetworkBuilder(InputLayer *inputLayer) : inputLayer_(inputLayer) {}

    NetworkBuilder &addLayer(uint neuronNumber, LayerType type = FULLY_CONNECTED, Function f = SIGMOID,
                             Descend descend = VANILLA) {
        shared_ptr<DerivativeFunction> function;
        switch (f) {
            case LINEAR:
                function = make_shared<Linear>();
                break;
            case SIGMOID:
                function = make_shared<Sigmoid>();
                break;
            default:
                throw "Unknown function!";
        }

        shared_ptr<DescendMethod> descendMethod;
        switch (descend) {
            case VANILLA:
                descendMethod = make_shared<VanillaGradientDescend>();
                break;
            default:
                throw "Unknown gradient descend method!";
        }

        MatrixDimension inputSize = layers_.empty() ?
                                    inputLayer_->getOutputDimension() : layers_.back()->getOutputDimension();
        InnerLayer *layer;
        switch (type) {
            case FULLY_CONNECTED:
                layer = new FullyConnectedLayer(inputSize.cols(), neuronNumber, function, descendMethod);
                break;
            default:
                throw "Unknown function!";
        }

        layers_.push_back(layer);
        return *this;
    }

    NeuralNetwork build(WeightInitializer *initializer) {
        for (InnerLayer *l : layers_)
            l->initialize(initializer);

        return {inputLayer_, layers_};
    }
};


#endif //NNPP_NETWORKBUILDER_H
