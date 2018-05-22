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

    double backpropagate(double learningRate, std::vector<IData<T> *> &dataBatches) {
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
                InnerLayer<T> *currentLayer;
                Layer<T> *leftLayer;
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
            for (InnerLayer<T> *layer : hiddenLayers_)
                layer->updateWeights(batch->trainSize());

            // Use test inputs to calculate the final error.
            error += calculateError(batch->getValidationInputs(), batch->getValidationOutputs()) / 2;
            numberOfData += batch->testSize();
        }
        return error / numberOfData; // Return normalized error
    }

    double calculateError(std::vector<T *> *inputs, std::vector<T *> *outputs) {
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

    void initialize(WeightInitializer *initializer) {
        for (InnerLayer<T> *l:hiddenLayers_)
            l->initialize(initializer);
    }

    std::string toString() {
        std::stringstream stringstream1;
//    stringstream1 << "Input layer" << endl
//                  << inputLayer_->toString();

        uint i = 0;
        for (InnerLayer<T> *l:hiddenLayers_) {
            if (i == hiddenLayers_.size() - 1) stringstream1 << "Output layer";
            else stringstream1 << "Inner layer " << i;
            stringstream1 << endl << l->toString();
            ++i;
        }
        return stringstream1.str();
    }

    std::vector<InnerLayer<T> *> getLayers() {
        return hiddenLayers_;
    }
};

typedef INeuralNetwork<Matrix> NeuralNetwork;


#endif //NN_NEURALNETWORK_H
