//
// Created by lirfu on 08.02.18..
//

#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(InputLayer *inputLayer, std::initializer_list<InnerLayer *> layers) :
        inputLayer_(inputLayer) {
    for (InnerLayer *l : layers)
        hiddenLayers_.push_back(l);
}

NeuralNetwork::NeuralNetwork(InputLayer *inputLayer, std::vector<InnerLayer *> &layers)
        : inputLayer_(inputLayer), hiddenLayers_(layers) {}

NeuralNetwork::~NeuralNetwork() {
//    uint size = hiddenLayers_.size();
//    for (uint i = 0; i < size; i++)
//        delete hiddenLayers_[i];
}

Matrix &NeuralNetwork::getOutput(const Matrix &input) {
    Layer *lastLayer = inputLayer_;
    ((InputLayer *) lastLayer)->setOutput(input);

    for (InnerLayer *hiddenLayer : hiddenLayers_) {
        hiddenLayer->forwardPass(*lastLayer);
        lastLayer = hiddenLayer;
    }

    return lastLayer->getOutput();
}

double NeuralNetwork::backpropagate(double learningRate, vector<Data *> &dataBatches) {
    double error = 0;
    ulong numberOfData = 0;
    // Updating for each batch.
    for (Data *batch : dataBatches) {
        // Accumulating deltas for each sample in batch.
        for (uint i = 0; i < batch->trainSize(); i++) {
            Matrix &input = *batch->getTrainingInputs()->operator[](i);
            Matrix &targetOutput = *batch->getTrainingOutputs()->operator[](i);

            // Calculate the output difference
            Matrix outDiff = ~(getOutput(input) - targetOutput);

            // Iterate through the rest of the layers (backwards)
            InnerLayer *currentLayer;
            Layer *leftLayer;
            for (uint l = hiddenLayers_.size() - 1; l != -1u; l--) {
                currentLayer = hiddenLayers_[l];
                if (l == 0)
                    leftLayer = inputLayer_;
                else
                    leftLayer = hiddenLayers_[l - 1];

                // Accumulate deltas throughout the network.
                outDiff = currentLayer->backwardPass(outDiff, leftLayer->getOutput(), learningRate);
            }
        }

        // Update weights with accumulated deltas.
        for (InnerLayer *layer : hiddenLayers_)
            layer->updateWeights(batch->trainSize());

        // Use test inputs to calculate the final error.
        error += calculateError(batch->getValidationInputs(), batch->getValidationOutputs()) / 2;
        numberOfData += batch->testSize();
    }
    return error / numberOfData; // Return normalized error
}

double NeuralNetwork::calculateError(vector<Matrix *> *inputs, vector<Matrix *> *outputs) {
    double totalError = 0;
    for (uint i = 0; i < inputs->size(); i++) {
        // Calculate the output difference
        Matrix outDiff = ((Matrix) *outputs->at(i)) - getOutput(*inputs->operator[](i));

        // Calculate the total output error
        for (uint r = 0; r < outDiff.rows(); r++)
            for (uint c = 0; c < outDiff.cols(); c++)
                totalError += outDiff.get(r, c) * outDiff.get(r, c);
    }
    return totalError / inputs->size();
}

std::string NeuralNetwork::toString() {
    std::stringstream stringstream1;
//    stringstream1 << "Input layer" << endl
//                  << inputLayer_->toString();

    uint i = 0;
    for (InnerLayer *l:hiddenLayers_) {
        stringstream1 << "Inner layer " << ++i << endl
                      << l->toString();
    }
    return stringstream1.str();
}

std::vector<InnerLayer *> NeuralNetwork::getLayers() {
    return hiddenLayers_;
}
