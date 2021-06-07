#pragma once
#pragma once
#include "MatrixMath.hpp"
class NeuralNetwork {
private:
	Matrix* inputNodes;
	Matrix* hiddenNodes;
	Matrix* outputNodes;

	Matrix* weightsHI;
	Matrix* weightsOH;

	Matrix* biasesH;
	Matrix* biasesO;

public:
	/** 
	A basic toy neural network with one hidden layer.
	Example code bit:
    NeuralNetwork* nn = new NeuralNetwork(2, 2, 1);
    float* input = new float[2];
    input[0] = 0.2f;
    input[1] = 1.0f;
    float* targets = new float[1];
    targets[0] = 1.0f;
    nn->train(input, 2, targets, 1, 0.05f, 100000);
    nn->feedForward(input, 2)->print();
	**/
	NeuralNetwork(int inputNodeCount, int hiddenNodeCount, int outputNodeCount) {
		inputNodes = nullptr;
		hiddenNodes = nullptr;
		outputNodes = nullptr;

		//inputHidden Weight-Matrix: rows = hidden, cols = input
		weightsHI = new Matrix(hiddenNodeCount, inputNodeCount);
		//hiddenOutput Weight-Matrix: rows = output, cols = hidden
		weightsOH = new Matrix(outputNodeCount, hiddenNodeCount);
		//randomize between -1 and 1
		randomize(weightsHI);
		randomize(weightsOH);

		biasesH = new Matrix(hiddenNodeCount, 1);//biases to hidden nodes
		randomize(biasesH);
		biasesO = new Matrix(outputNodeCount, 1);//biases to output nodes
		randomize(biasesO);
	}
	bool firstIteration = true;
	Matrix* feedForward(float* imputArray, int inputSize) {
		if (firstIteration == false) {
			delete hiddenNodes;
			delete inputNodes;
			delete outputNodes;
		}
		else {
			firstIteration = false;
		}
		inputNodes = new Matrix(imputArray, inputSize);
		//multiply imputnodes with weights between inputs and hiddens to get hiddenNodes
		hiddenNodes = dotProduct(weightsHI, inputNodes, false);
		//add hiddenBiases to hidden
		hiddenNodes = addMatrices(hiddenNodes, biasesH, true);
		hiddenNodes = mapWithSigmoid(hiddenNodes, true);

		outputNodes = dotProduct(weightsOH, hiddenNodes, false);
		outputNodes = addMatrices(outputNodes, biasesO, true);
		outputNodes = mapWithSigmoid(outputNodes, true);
		return outputNodes;
	}

	void train(float* inputs, int inputSize, float* traindata, int targetSize, float learningRate, int iterations) {
		Matrix* targets = fromArray(traindata, targetSize);
		for (int i = 0; i < iterations; i++) {
			this->feedForward(inputs, inputSize);
			backpropagation(targets, learningRate);
		}
		delete targets;
	}

	void backpropagation(Matrix* targets, float learningRate) {
		//output errors

		Matrix* oErrors = substractMatrices(targets, outputNodes, false);

		Matrix* oGradients = mapWithDsigmoid(outputNodes, false);
		oGradients = elementWiseMult(oGradients, oErrors, true);
		multiplyWithNumber(oGradients, learningRate);


		Matrix* hiddenTrans = transpose(hiddenNodes, false);
		Matrix* ohDeltas = dotProduct(oGradients, hiddenTrans, false);
		weightsOH = addMatrices(weightsOH, ohDeltas, true);
		biasesO = addMatrices(biasesO, oGradients, true);

		//hidden errors

		Matrix* wOHTrans = transpose(weightsOH, false);
		Matrix* hErrors = dotProduct(wOHTrans, oErrors, false);

		Matrix* hGradients = mapWithDsigmoid(hiddenNodes, false);
		hGradients = elementWiseMult(hGradients, hErrors, true);
		multiplyWithNumber(hGradients, learningRate);

		Matrix* inputTrans = transpose(inputNodes, false);
		Matrix* hiDeltas = dotProduct(hGradients, inputTrans, false);
		weightsHI = addMatrices(weightsHI, hiDeltas, true);
		biasesH = addMatrices(biasesH, hGradients, true);


		delete oGradients;
		delete oErrors;
		delete hiddenTrans;
		delete ohDeltas;
		delete wOHTrans;
		delete hErrors;
		delete hGradients;
		delete inputTrans;
		delete hiDeltas;
	}
};