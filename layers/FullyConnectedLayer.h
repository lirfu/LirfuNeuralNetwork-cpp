//
// Created by lirfu on 08.02.18..
//

#ifndef NNPP_FULLYCONNECTEDLAYER_H
#define NNPP_FULLYCONNECTEDLAYER_H


#include "InnerLayer.h"
#include "../functions/DerivativeFunction.h"
#include "../descentmethods/DescendMethod.h"
#include "../weightinitializers/WeightInitializer.h"

class FullyConnectedLayer : public InnerLayer {
protected:
    uint inputSize_;
    DerivativeFunction *function_;

    Matrix net_;

    Matrix weights_;
    Matrix biases_;

    Matrix weightDeltas_;
    Matrix biasDeltas_;

private:
    DescendMethod *descendMethod_;

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
    FullyConnectedLayer(uint inputSize, uint neuronNumber, DerivativeFunction *function,
                        DescendMethod *descendMethod, WeightInitializer *initializer)
            : InnerLayer(*new Matrix(1, neuronNumber)) {
        inputSize_ = inputSize;
        function_ = function;
        descendMethod_ = descendMethod;

        Matrix biases(1, neuronNumber);
        biases_ = biases;
        Matrix weights(inputSize, neuronNumber);
        weights_ = weights;

        Matrix biasDeltas(1, neuronNumber);
        biasDeltas_ = biasDeltas;
        Matrix weightDeltas(inputSize, neuronNumber);
        weightDeltas_ = weightDeltas;

        initializer->initialize(biases_);
        initializer->initialize(weights_);
    }

    ~FullyConnectedLayer() {
        delete function_;
        delete descendMethod_;
    }

    void forwardPass(Layer &leftLayer) override {
        // sigm(x * w + w0)
        net_ = leftLayer.getOutput() * weights_;
        net_ += biases_;
        output_ = net_ ^ [=](double v) -> double { return function_->calculate(v); };
    }

    Matrix &backwardPass(Matrix &outputDifferences, Matrix &leftOutputs, double learningRate) {
        // Layer differences
        Matrix differences = ~net_;
        differences ^= [=](double v) -> double { return function_->calculateDerivative(v); };
        differences.HadamardProduct(outputDifferences);

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
        FullyConnectedLayer *layer = new FullyConnectedLayer(*this);
        return layer;
    }

    std::string toString() {
        std::stringstream str;
        str << biases_.toString(2) << '\n' << weights_.toString(2);
        return str.str();
    }

    uint getNeuronNumber() override {
        return output_.cols();
    }

    void getNeuron(uint index, std::vector<double> &values) override {
        values.push_back(biases_[0][index]);
        for (uint r = 0; r < weights_.rows(); r++)
            values.push_back(weights_[r][index]);
    }

    void setNeuron(uint index, std::vector<double> &values) override {
        biases_.set(0, index, values[0]);

        for (uint r = 0; r < weights_.rows(); r++)
            weights_.set(r, index, values[r + 1]);
    }
};


#endif //NNPP_FULLYCONNECTEDLAYER_H
