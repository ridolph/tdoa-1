#pragma once
#include <fstream>
#include <cmath>

const unsigned short int RBF_MAXDIM_LAYER = 200;
const unsigned short int RBF_MAXNUM_SAMPLE = 30000;
const float RBF_MAX = float(0x7fffffff / 2);

enum RadialBasisFunction
{
	GaussianKernel,			//0
	ExponentialKernel,		//1
	LaplacianKernel,		//2
	InverseMultiquadric,	//3
};
class RBFnetwork
{
public:
	void trainRBF(char* trainPATH, char* logPATH, char* outPATH, unsigned short int dimRBF,
				RadialBasisFunction rFunc, unsigned int maxITER, float RMSE, float* adamParam);
	void testRBF(char* inPATH, char* testPATH, char* resultPATH);

private:
	unsigned short int numSample;
	unsigned short int dimLayer[3];
	RadialBasisFunction rbFun;
	float sampleX[RBF_MAXNUM_SAMPLE][RBF_MAXDIM_LAYER];
	float sampleY[RBF_MAXNUM_SAMPLE][RBF_MAXDIM_LAYER];
	float rbfConst;
	float rbfCenter[RBF_MAXDIM_LAYER][RBF_MAXDIM_LAYER];
	float w[RBF_MAXDIM_LAYER][RBF_MAXDIM_LAYER];
	float calRBF(float r_2);
	void updateSampleX();
	void inputSample(char* inputPath);
	void kmeansCluster(unsigned short int dimRBF, RadialBasisFunction rFunc);
	void trainWeight(char* outputPath, unsigned int maxITER, float RMSE, float* learningParam);
	void saveRBFnetwork(char* outputPath);
	void loadRBFnetwork(char* inputPath);
	void getResult(char* outputPath);
};