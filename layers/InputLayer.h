//
// Created by lirfu on 08.02.18..
//

#ifndef NNPP_INPUTLAYER_H
#define NNPP_INPUTLAYER_H


#include "Layer.h"


template<typename T>
class InputLayer : public Layer<T> {
private:
    InputLayer(InputLayer<T> &inputLayer) = default;

public:
    explicit InputLayer(uint neuronNumber) : Layer<T>(*new T(1, neuronNumber)) {
    }

    virtual void setOutput(const T &output) {
        if (!this->output_.getDimension().equals(output.getDimension())) {
            std::stringstream message;
            message << "Matrices are not the same size: " << this->output_.getDimension().toString() << " != " << output
                    .getDimension().toString();
            throw message.str();
        }
        this->output_ = output;
    }

    uint numberOfParameters() override {
        return 0;
    }

    uint getNeuronNumber() override {
        return this->output_.cols();
    }

    void getNeuron(uint index, double *values) override {
        // do nothing
    }

    void setNeuron(uint index, double *values) override {
        // do nothing
    }

    InputLayer<T> *copy() override {
        return new InputLayer<T>(*this);
    }

    std::string toString() override {
        std::stringstream stringstream1;
        stringstream1 << "O: " << this->output_.toString();
        return stringstream1.str();
    }
};


#endif //NNPP_INPUTLAYER_H
