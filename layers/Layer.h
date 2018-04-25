//
// Created by lirfu on 08.02.18..
//

#ifndef NN_LAYER_H
#define NN_LAYER_H


#include "../matrix/Matrix.h"
#include "../matrix/MatrixDimension.h"

template<typename T>
class Layer {
protected:
    explicit Layer(T &output) : output_(output) {}

    Layer(Layer &layer) : output_(layer.output_) {}

    T output_;
public:
    ~Layer() = default;

    /**
     * Returns the neuron's output matrix.
     * @return Output matrix.
     */
    T &getOutput() {
        return output_;
    }

    /**
     * Returns the dimension of the output matrix.
     * @return Output dimension.
     */
    MatrixDimension getOutputDimension() {
        return output_.getDimension();
    }

    virtual uint numberOfParameters()=0;

    virtual uint getNeuronNumber()=0;

    /**
     * Gets the neuron weights.
     * @param index Index of the neuron in layer.
     * @param values Vector to fill with values.
     */
    virtual void getNeuron(uint index, double *values)=0;

    /**
     * Sets the neuron weights.
     * @param index Index of the neuron in layer.
     * @param values Vector with weight values.
     */
    virtual void setNeuron(uint index, double *values)=0;

    virtual Layer *copy()=0;

    virtual std::string toString()=0;
};


#endif //NN_LAYER_H
