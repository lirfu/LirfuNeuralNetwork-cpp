//
// Created by lirfu on 08.02.18..
//

#ifndef NN_NEURALNETWORK_H
#define NN_NEURALNETWORK_H

#include<memory>
#include "layers/InnerLayer.h"
#include "layers/InputLayer.h"
#include "data/SeparatedData.h"

class NeuralNetwork {
private:
    InputLayer *inputLayer_;
    std::vector<InnerLayer *> hiddenLayers_;

public:
    NeuralNetwork(InputLayer *inputLayer, std::initializer_list<InnerLayer *> layers);

    NeuralNetwork(InputLayer *inputLayer, std::vector<InnerLayer *> &layers);

    ~NeuralNetwork();

    Matrix &getOutput(const Matrix &input);

    double backpropagate(double learningRate, vector<Data *> &dataBatches);

    double calculateError(vector<Matrix *> *inputs, vector<Matrix *> *outputs);

    std::string toString();

    std::vector<InnerLayer*> getLayers();
};


#endif //NN_NEURALNETWORK_H
