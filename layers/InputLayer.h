//
// Created by lirfu on 08.02.18..
//

#ifndef NNPP_INPUTLAYER_H
#define NNPP_INPUTLAYER_H


#include "Layer.h"

class InputLayer : public Layer {
private:
    InputLayer(InputLayer &inputLayer) = default;

public:
    explicit InputLayer(uint neuronNumber) : Layer(*new Matrix(1, neuronNumber)) {
    }

    void setOutput(const Matrix &output) {
        if (!output_.getDimension().equals(output.getDimension())) {
            std::stringstream message;
            message << "Matrices are not the same size: " << output_.getDimension().toString() << " != " << output
                    .getDimension().toString();
            throw message.str();
        }
        output_ = output;
    }

    uint numberOfParameters() override {
        return 0;
    }

    uint getNeuronNumber() override {
        return output_.cols();
    }

    void getNeuron(uint index, double *values) override {
        // do nothing
    }

    void setNeuron(uint index, double *values) override {
        // do nothing
    }

    InputLayer *copy() override {
        return new InputLayer(*this);
    }

    std::string toString() override {
        std::stringstream stringstream1;
        stringstream1 << "O: " << output_.toString();
        return stringstream1.str();
    }
};


#endif //NNPP_INPUTLAYER_H
