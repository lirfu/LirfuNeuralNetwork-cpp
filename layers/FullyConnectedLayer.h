//
// Created by lirfu on 08.02.18..
//

#ifndef NNPP_FULLYCONNECTEDLAYER_H
#define NNPP_FULLYCONNECTEDLAYER_H


#include "InnerLayer.h"
#include "../functions/DerivativeFunction.h"
#include "../descentmethods/DescendMethod.h"
#include "../weightinitializers/WeightInitializer.h"

template<typename T>
class FullyConnectedLayer : public InnerLayer<T> {
private:
    uint inputSize_;
    std::shared_ptr<DerivativeFunction> activation_;
    std::shared_ptr<DescendMethod> descendMethod_;

    T net_;
    T weights_;
    T biases_;

    T weightDeltas_;
    T biasDeltas_;

    FullyConnectedLayer(FullyConnectedLayer<T> &fullyConnectedLayer) : InnerLayer<T>(fullyConnectedLayer) {
        inputSize_ = fullyConnectedLayer.inputSize_;
        activation_ = fullyConnectedLayer.activation_;
        descendMethod_ = fullyConnectedLayer.descendMethod_;
        net_ = T(fullyConnectedLayer.net_);

        weights_ = T(fullyConnectedLayer.weights_);
        biases_ = T(fullyConnectedLayer.biases_);
        weightDeltas_ = T(fullyConnectedLayer.weightDeltas_);
        biasDeltas_ = T(fullyConnectedLayer.biasDeltas_);
    }

public:
    FullyConnectedLayer(uint inputSize, uint neuronNumber, std::shared_ptr<DerivativeFunction> activationFunction,
                        std::shared_ptr<DescendMethod> descendMethod, WeightInitializer *initializer)
            : InnerLayer<T>(*new T(1, neuronNumber)),
              inputSize_(inputSize), activation_(activationFunction), descendMethod_(descendMethod),
              biases_(1, neuronNumber), weights_(inputSize, neuronNumber),
              biasDeltas_(1, neuronNumber), weightDeltas_(inputSize, neuronNumber) {

        initializer->initialize(biases_);
        initializer->initialize(weights_);
    }

    ~FullyConnectedLayer() {
        delete activation_;
        delete descendMethod_;
    }

    void forwardPass(Layer<T> &leftLayer) override {
        // sigm(x * w + w0)
        net_ = leftLayer.getOutput() * weights_ + biases_;
        this->output_ = net_ ^ activation_->getFunction();
    }

    T &backwardPass(T &outputDifferences, T &leftOutputs, double learningRate) override {
        // Layer differences
        T differences = outputDifferences % (~(net_ ^ (activation_->getDerivative())));

        // Update the differences for the next iteration
        outputDifferences = weights_ * differences;

        // Accumulate weight and bias deltas.
        descendMethod_->performDescend(weightDeltas_, ~(differences * leftOutputs) *= learningRate);
        descendMethod_->performDescend(biasDeltas_, ~differences *= learningRate);

        return outputDifferences;
    }

    void updateWeights() override {
        // Update weights.
        weights_ += weightDeltas_;
        biases_ += biasDeltas_;

        // Reset deltas.
        weightDeltas_ *= 0.;
        biasDeltas_ *= 0.;
    }

    uint numberOfParameters() override {
        return biases_.rows() * biases_.cols() + weights_.rows() * weights_.cols();
    }

    FullyConnectedLayer *copy() override {
        return new FullyConnectedLayer(*this);
    }

    std::string toString() {
        std::stringstream str;
        str << "B: " << biases_.toString(17)
            << "W: " << weights_.toString(17);
        return str.str();
    }

    uint getNeuronNumber() override {
        return this->output_.cols();
    }

    void getNeuron(uint index, double *values) override {
        uint i = 0;
        values[i++] = biases_.get(0, index);
        for (uint r = 0; r < weights_.rows(); r++)
            values[i++] = weights_.get(r, index);
    }

    void setNeuron(uint index, double *values) override {
        biases_.set(0, index, values[0]);
        for (uint r = 0; r < weights_.rows(); r++)
            weights_.set(r, index, values[r + 1]);
    }
};


#endif //NNPP_FULLYCONNECTEDLAYER_H
