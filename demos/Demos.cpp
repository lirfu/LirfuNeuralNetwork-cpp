//
// Created by lirfu on 19.03.18..
//

#include "Demos.h"
#include "../matrix/Matrix.h"
#include "../data/Data.h"
#include "../weightinitializers/WeightInitializer.h"
#include "../weightinitializers/RandomWeightInitializer.h"
#include "../descentmethods/DescendMethod.h"
#include "../descentmethods/VanillaGradientDescend.h"
#include "../NeuralNetwork.h"
#include "../layers/FullyConnectedLayer.h"
#include "../functions/Sigmoid.h"
#include "../functions/Linear.h"
#include "../data/SimpleData.h"

void Demos::demoRegression() {
    try {
        uint numberOfPoints = 50;

        // Create the data.
        vector<Matrix *> inputs;
        vector<Matrix *> outputs;
        for (int i = 0; i < numberOfPoints; i++) {
            double input = i * 2. * M_PI / numberOfPoints;
            inputs.push_back(new Matrix(1, 2, {input, input}));
            outputs.push_back(new Matrix(1, 1, {4 * sin(input) * input + 5}));
        }
        vector<Data *> batches;
        batches.push_back(new SimpleData(&inputs, &outputs));

        // Build the network.
        WeightInitializer *initializer = new RandomWeightInitializer(-2, 2);
        DescendMethod *descendMethod = new VanillaGradientDescend();
        NeuralNetwork network(new InputLayer(2), {
                new FullyConnectedLayer(2, 3, new Sigmoid(), descendMethod, initializer),
                new FullyConnectedLayer(3, 7, new Sigmoid(), descendMethod, initializer),
                new FullyConnectedLayer(7, 4, new Sigmoid(), descendMethod, initializer),
                new FullyConnectedLayer(4, 1, new Linear(), descendMethod, initializer)
        });
        delete initializer;

        cout << "init 2" << endl; // Initialize graph with 2 curves

        // Run backpropagation.
        uint iteration = 0;
        double error = 1000;
        while (error > 1e-4) {
            error = network.backpropagate(1e-3, batches);
            iteration++;

            // Report progress.
            if (iteration % 1000 == 0) {
                std::cout << "echo Iteration " << iteration << " has error: "
                          << setprecision(16) << error << std::endl;

                cout << "clear" << endl; // Clear the graphs.
                Matrix out;
                for (uint i = 0; i < numberOfPoints; i++) {
                    out = network.getOutput(*inputs.at(i));
                    // Add the target and predicted output values.
                    std::cout << "add " << out.get(0, 0) << " " << outputs.at(i)->get(0, 0) << std::endl;
                }
            }
        }
        std::cout << "echo Finally, iteration " << iteration << " has error: "
                  << setprecision(16) << error << std::endl;

        // Cleanup data.
        delete descendMethod;
        for (Data *sd : batches) {
            delete sd;
        }
    } catch (string s) {
        std::cout << s << endl;
    }
}
