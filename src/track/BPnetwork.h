#pragma once
#include <fstream>
#include <cmath>

const unsigned short int BP_MAXDIM_LAYER = 100;
const unsigned short int BP_MAXNUM_LAYER = 10;
const unsigned short int BP_MAXNUM_SAMPLE = 30000;
const float BP_MAX = float(0x7fffffff / 2);

enum ActivationFunction
{
	noActivation,	//0
	Tanh,			//1
	Sigmoid,		//2
	ReLU,			//3
	LeakyReLU,		//4
	ELU,			//5
	Softplus		//6
};
class BPnetwork
{
public:
	void trainBP(char* trainPATH, char* logPATH, char* outPATH,
				 unsigned int maxITER, float RMSE, float* adamParam);
	void testBP(char* inPATH, char* testPATH, char* resultPATH);
	void callBP(char* inPATH, float* X);
	unsigned short int numHidden;
	unsigned short int dimLayer[BP_MAXNUM_LAYER];
	ActivationFunction acFun[BP_MAXNUM_LAYER];

private:
	unsigned short int numSample;
	float sampleX[BP_MAXNUM_SAMPLE][BP_MAXDIM_LAYER];
	float sampleY[BP_MAXNUM_SAMPLE][BP_MAXDIM_LAYER];
	float w[BP_MAXNUM_LAYER][BP_MAXDIM_LAYER][BP_MAXDIM_LAYER];
	float xSD[BP_MAXDIM_LAYER] = {0.0};
	float ySD[BP_MAXDIM_LAYER] = {0.0};
	float xMean[BP_MAXDIM_LAYER] = {0.0};
	float yMean[BP_MAXDIM_LAYER] = {0.0};
	float activateNet(float x, ActivationFunction aFunc);
	float acFunDiff(float y, ActivationFunction aFunc);
	void dataNormalize();
	void forwardCal(float* X);
	void inputSample(char* inputPath);
	void trainWeight(char* outputPath, unsigned int maxITER, float RMSE, float* adamParam);
	void saveBPnetwork(char* outputPath);
	void loadBPnetwork(char* inputPath);
	void getResult(char* outputPath);
};