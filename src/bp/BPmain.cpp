#include "BPnetwork.h"
#include "TDOAdata.h"

TDOAdata tdoaTest;
BPnetwork bpTest;

void setBP(char* trainPATH, char* logPATH, char* outPATH)
{
	bpTest.numHidden = 2;
	bpTest.acFun[0] = noActivation;
	bpTest.dimLayer[1] = 20;
	bpTest.acFun[1] = Tanh;
	bpTest.dimLayer[2] = 20;
	bpTest.acFun[2] = Tanh;
	bpTest.acFun[bpTest.numHidden + 1] = noActivation;
	float adamParameter[4] = { float(0.001),float(0.9),float(0.999),float(1e-8) };
	bpTest.trainBP(trainPATH, logPATH, outPATH, 1000, float(0.01), adamParameter);
}

int main()
{
	char envPath[] = "data/1env.txt";
	char trainPath[] = "data/2train.txt";
	char logPath[] = "data/3log.txt";
	char bpPath[] = "data/4bp.txt";
	char testPath[] = "data/5test.txt";
	char resultPath[] = "data/6result.txt";
	
	srand((unsigned)time(NULL));
	tdoaTest.generateSample(envPath, trainPath, float(0.0), float(0.1), Interval3D, 0, float(0.2));
	setBP(trainPath, logPath, bpPath);
	tdoaTest.generateSample(envPath, testPath, float(0.0), float(0.1), Sine2D, 80, float(0.6));
	bpTest.testBP(bpPath, testPath, resultPath);

	return 0;
}