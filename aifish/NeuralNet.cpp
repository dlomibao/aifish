#include "NeuralNet.h"
#include<iostream>
#include<cmath>
using namespace std;


Neuron::Neuron(int inputs){
	bias=(10.0*((double)rand()/RAND_MAX)-5);//0;//.2*((double)rand()/RAND_MAX)-.1;
	for(int i=0;i<inputs;i++){
		double wtemp=((((double)rand()/RAND_MAX))+.05)/(double)inputs;
		if(rand()%2==0){wtemp*=-1.0;}
		//double wtemp=((((double)rand()/RAND_MAX))-.5);
		w.push_back(wtemp);
	}
}
double Neuron::calc(std::vector<double> &x){
	//cout<<"hello"<<endl;
	double u=this->bias;
	//cout<<u<< endl;
	for(unsigned int i=0;i<w.size();i++){
		//cout<<"w"<<w[i]<<" x"<<x[i]<< "u"<<u<< endl;
		u+=w[i] * x[i];
	}
    double pwr=pow(2.7182818284,-u);
   // cout<<u<<endl;
    if(isnan(pwr)){if(u<0){pwr=1E2;}else{pwr=-1E2;}}
	double y=((1.0)/(1+pwr));
	//cout<<" weighted sum input"<<u<< "output: "<<y<<endl;
	return y;
}

Layer::Layer(int inputcount,int neuroncount,std::vector<double> &input){
	xcount=inputcount;
	ncount=neuroncount;
	//x=input;
	for(int i=0;i<ncount;i++){
		neurons.push_back(Neuron(xcount));
	}
	this->y=vector<double>(ncount);
	this->deltas=vector<double>(ncount);
}
NeuralNet::NeuralNet(int layerc,int *layersizes,std::vector<double> &input){
	x=&input;
	xcount=x->size();
	Layer lay=Layer(xcount,layersizes[0],input);
	layers.push_back( lay);
	lcount=layerc;
	for(int i=1;i<lcount;i++){
		lay=Layer(layersizes[i-1],layersizes[i],layers[i-1].y);
		layers.push_back( lay);
		std::cout<<layers[i].ncount<<std::endl;
	}
	std::cout<<lcount-1<<"  "<<layersizes[lcount-1]<<std::endl;

	ycount=layersizes[lcount-1];
	std::cout<<ycount;

}
double NeuralNet::weightedDeltaSum(int cnode,Layer &next){//used for back prop
    double sum=0;
    int i;
    double delta,weight;
    for(i=0;i<next.ncount;i++){
        delta=next.deltas[i];//delta for the neuron
        weight=next.neurons[i].w[cnode];
        sum+=delta*weight;
    }
    return sum;

}
void NeuralNet::backProp(std::vector<double> &input,std::vector<double> &training){
	//cout<<"back prop"<<endl;
	this->feed(input);

	//backprop outlayer
	int i,j,k;
	int outlay=lcount-1;

	double y,t,d;
	double sumsquareerror=0;
	//outlevel
		for( j=0;j<layers[outlay].ncount;j++){

			y= layers[outlay].y[j];
			t= training[j];
			cout<<"t:"<<t<<" y:"<<y<<endl;
			layers[outlay].deltas[j]=y*(1-y)*(y-t);
			d= -LEARNRATE* layers[outlay].deltas[j];
			sumsquareerror+=(y-t)*(y-t);
			for( k=0;k<layers[outlay].xcount;k++){
				//cout<<"   w="<<k<<endl;
				layers[outlay].neurons[j].w[k]+=layers[outlay].y[k]*d;
			}
			layers[outlay].neurons[j].bias+=-LEARNRATE*layers[outlay].deltas[j];
		}
		cout<<"sum square error:" << sumsquareerror<<endl;
	//hiddenlayers
	for(int i=outlay-1;i>=0;i--){
		for(int j=0;j<layers[i].ncount;j++){
			y= layers[i].y[j];
			layers[i].deltas[j]=y*(1-y)*weightedDeltaSum(i,layers[i+1]);
			d= -LEARNRATE*layers[i].deltas[j];
			for( k=0;k<layers[i].xcount;k++){
				double inp=0;
				if(i>0){
					inp=layers[i-1].y[k];
				}else{
					inp=input[k];
				}
				//cout<< "layer"<<i<<inp<<endl;

				layers[i].neurons[j].w[k]+=inp*d;

			}
			layers[i].neurons[j].bias+=-LEARNRATE*layers[i].deltas[j];

		}
	}

}
void Layer::feedLayer(vector<double> &x){
	for(int i=0;i<ncount;i++){
		//cout<<"feed layer. neuron #"<<i<<endl;
		y[i]=neurons[i].calc(x);
		//cout<<"feed layer Neuron"<<i<<" output:"<<neurons[i].calc(x)<<endl;
	}
}
void NeuralNet::feed(std::vector<double> &input){
	std::vector<double> &x=input;
	for(int i=0;i<lcount;i++){
		layers[i].feedLayer(x);

		x=layers[i].y;

	}
	x=input;
}

std::vector<double> NeuralNet::getOutput(){
	return std::vector<double>(layers[lcount-1].y);
}
