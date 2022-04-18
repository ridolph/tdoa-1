#include "RBFnetwork.h"
#include "TDOAdata.h"

TDOAdata tdoaTest;
RBFnetwork RBF;

int main()
{
	char envPath[] = "data\\1env.txt";
	char trainPath[] = "data\\2train.txt";
	char logPath[] = "data\\3log.txt";
	char rbfPath[] = "data\\4rbf.txt";
	char testPath[] = "data\\5test.txt";
	char resultPath[] = "data\\6result.txt";
	float adam[4] = { float(0.001),float(0.9),float(0.999),float(1e-8) };

	srand((unsigned)time(NULL));
	//tdoaTest.generateSample(envPath, trainPath, float(0.0), float(0.1), Interval, 0, float(0.2));
	//RBF.trainRBF(trainPath, logPath, rbfPath, 150, InverseMultiquadric, 200000, float(0.1), adam);
	//tdoaTest.generateSample(envPath, testPath, float(0.0), float(0.1), Sine2D, 100, float(1.26));
	RBF.testRBF(rbfPath, testPath, resultPath);
	
	return 0;
}