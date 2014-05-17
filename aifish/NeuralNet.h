#ifndef NEURALNET_H
#define NEURALNET_H

#include<vector>
#include<math.h>
//#include "random.h"
#include<stdlib.h>
const double LEARNRATE=.075;

class Neuron{

public:
	double calc(std::vector<double> &x);
	Neuron(int inputs);
	double bias;
		std::vector<double> w;
};
class Layer{
public:
	int xcount;
	int ncount;
	std::vector<Neuron> neurons;
	std::vector<double> y;//output of neurons
	std::vector<double> deltas;//back prop stuff
	//std::vector<double> &x;//reference to inputs from either other layer or input

	void feedLayer(std::vector<double> &x);
	//void setInput(std::vector<double> &x);
	Layer(int inputcount,int neuroncount,std::vector<double> &input);

};
class NeuralNet{
public:
	NeuralNet(int layerc,int *layersizes, std::vector<double> &input);
	void feed(std::vector<double> &input);
	//void backProp();
	void backProp(std::vector<double> &input,std::vector<double> &training);
	std::vector<double> getOutput();
	double weightedDeltaSum(int cnode,Layer &next);
private:
	int xcount;
	int ycount;
	int lcount;
	std::vector<Layer> layers;
	std::vector<double> *x;
};

#endif
