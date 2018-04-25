//
// Created by lirfu on 08.02.18..
//

#ifndef NN_NEURALNETWORK_H
#define NN_NEURALNETWORK_H

#include<memory>
#include "layers/InnerLayer.h"
#include "layers/InputLayer.h"
#include "data/SeparatedData.h"

template<typename T>
class INeuralNetwork {
private:
    InputLayer<T> *inputLayer_;
    std::vector<InnerLayer<T> *> hiddenLayers_;

public:
    INeuralNetwork(InputLayer<T> *inputLayer, std::initializer_list<InnerLayer<T> *> layers)
            : inputLayer_(inputLayer) {
        for (InnerLayer<T> *l : layers)
            hiddenLayers_.push_back(l);
    }

    INeuralNetwork(InputLayer<T> *inputLayer, std::vector<InnerLayer<T> *> &layers)
            : inputLayer_(inputLayer), hiddenLayers_(layers) {}

    ~INeuralNetwork() {
        delete inputLayer_;
        for (InnerLayer<T> *l : hiddenLayers_)
            delete l;
    }

    T &getOutput(const T &input) {
        Layer<T> *lastLayer = inputLayer_;
        ((InputLayer<T> *) lastLayer)->setOutput(input);

        for (InnerLayer<T> *hiddenLayer : hiddenLayers_) {
            hiddenLayer->forwardPass(*lastLayer);
            lastLayer = hiddenLayer;
        }

        return lastLayer->getOutput();
    }

    double backpropagate(double learningRate, vector<IData<T> *> &dataBatches);

    double calculateError(vector<T *> *inputs, vector<T *> *outputs);

    std::string toString();

    std::vector<InnerLayer *> getLayers();
};

typedef INeuralNetwork<Matrix> NeuralNetwork;


#endif //NN_NEURALNETWORK_H
