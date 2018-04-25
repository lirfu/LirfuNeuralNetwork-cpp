//
// Created by lirfu on 08.02.18..
//

#ifndef NNPP_FULLYCONNECTEDLAYER_H
#define NNPP_FULLYCONNECTEDLAYER_H


#include <memory>
#include "InnerLayer.h"
#include "../functions/DerivativeFunction.h"
#include "../descentmethods/DescendMethod.h"
#include "../weightinitializers/WeightInitializer.h"

class FullyConnectedLayer : public InnerLayer {
private:
    uint inputSize_;
    std::shared_ptr<DerivativeFunction> function_;
    std::shared_ptr<DescendMethod> descendMethod_;

    Matrix net_;
    Matrix weights_;
    Matrix biases_;

    Matrix weightDeltas_;
    Matrix biasDeltas_;

//    FullyConnectedLayer(FullyConnectedLayer &fullyConnectedLayer) {
//        InnerLayer::InnerLayer(fullyConnectedLayer);
//
//        inputSize_ = fullyConnectedLayer.inputSize_;
//        function_ = fullyConnectedLayer.function_;
//        weights_ = fullyConnectedLayer.weights_.copy();
//        biases_ = fullyConnectedLayer.biases_.copy();
//        weightDeltas_ = fullyConnectedLayer.weightDeltas_.copy();
//        biasDeltas_ = fullyConnectedLayer.biasDeltas_.copy();
//
//        descendMethod_ = fullyConnectedLayer.descendMethod_.copy();
//        net_ = fullyConnectedLayer.net_.copy();
//    }

public:
    FullyConnectedLayer(uint inputSize, uint neuronNumber, std::shared_ptr<DerivativeFunction> function,
                        std::shared_ptr<DescendMethod> descendMethod)
            : InnerLayer(*new Matrix(1, neuronNumber)),
              inputSize_(inputSize), function_(function), descendMethod_(descendMethod),
              biases_(1, neuronNumber), weights_(inputSize, neuronNumber),
              biasDeltas_(1, neuronNumber), weightDeltas_(inputSize, neuronNumber) {
    }

    void initialize(WeightInitializer *initializer) override {
        initializer->initialize(biases_);
        initializer->initialize(weights_);
    }

    ~FullyConnectedLayer() = default;

    void forwardPass(Layer &leftLayer) override {
        // sigm(x * w + w0)
        net_ = leftLayer.getOutput() * weights_;
        net_ += biases_;
        output_ = net_ ^ [=](double v) -> double { return function_->calculate(v); };
    }

    Matrix &backwardPass(Matrix &outputDifferences, Matrix &leftOutputs, double learningRate) override {
        // Layer differences
        Matrix differences = ~net_;
        differences ^= [=](double v) -> double { return function_->calculateDerivative(v); };
        differences %= outputDifferences;

        // Update the differences for the next iteration
        outputDifferences = weights_ * differences;

        // Accumulate weight and bias deltas.
        descendMethod_->performDescend(weightDeltas_, ~(differences * leftOutputs) *= learningRate);
        descendMethod_->performDescend(biasDeltas_, ~differences *= learningRate);

        return outputDifferences;
    }

    void updateWeights(ulong batchSize) override {
        // Batch size correction.
        weightDeltas_ *= (1. / batchSize);
        biasDeltas_ *= (1. / batchSize);

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
        return output_.cols();
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
