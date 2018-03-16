
#include <chrono>
#include "matrix/Matrix.h"
#include "data/Data.h"
#include "data/SeparatedData.h"
#include "weightinitializers/WeightInitializer.h"
#include "weightinitializers/RandomWeightInitializer.h"
#include "descentmethods/DescendMethod.h"
#include "descentmethods/VanillaGradientDescend.h"
#include "NeuralNetwork.h"
#include "layers/FullyConnectedLayer.h"
#include "functions/Sigmoid.h"
#include "functions/Linear.h"

int main() {
    try {
        uint numberOfPoints = 50;

        // Create the data.
        vector<Matrix *> inputs;
        vector<Matrix *> outputs;
        for (int i = 0; i < numberOfPoints; i++) {
            double input = i * 2. * M_PI / numberOfPoints;
            inputs.push_back(new Matrix(1, 1, {input}));
            outputs.push_back(new Matrix(1, 1, {4 * sin(2 * input) + 5}));
        }
        vector<Data *> batches;
        batches.push_back(new SeparatedData(&inputs, &outputs, &inputs, &outputs));

        // Build the network.
        WeightInitializer *initializer = new RandomWeightInitializer(-1, 1);
        DescendMethod *descendMethod = new VanillaGradientDescend();
        NeuralNetwork network(new InputLayer(1), {
                new FullyConnectedLayer(1, 6, new Sigmoid(), descendMethod, initializer),
                new FullyConnectedLayer(6, 1, new Linear(), descendMethod, initializer)
        });
        delete initializer;

        // Run backpropagation.
        uint iteration = 0;
        double error = 1000;
        while (error > 1e-4) {
            error = network.backpropagate(1e-3, batches);
            iteration++;

            // Report progress.
            if (iteration % 1000 == 0) {
                std::cout << "Iteration " << iteration << " has error: " << setprecision(16) << error << std::endl;
            }
        }
        std::cout << "Iteration " << iteration << " has error: " << setprecision(16) << error << std::endl;

        // Output results.
        for (int i = 0; i < numberOfPoints; i++) {
            std::cout << network.getOutput(*inputs.at(i)).get(0, 0) << std::endl;
        }

        // Cleanup data.
        delete descendMethod;
        for (Data *sd : batches) {
            delete sd;
        }
    } catch (string s) {
        std::cout << s << endl;
    }

    return 0;
}