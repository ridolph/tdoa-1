#include "Taylor.h"
#include "TDOAdata.h"

TDOAdata tdoaTest;
Taylor taylorTest;

int main()
{
	char envPath[] = "data\\1env.txt";
	char testSamplePath[] = "data\\2smp.txt";
	char testResultPath[] = "data\\3pso.txt";

	srand((unsigned)time(NULL));
	float noiseStandardDeviation = float(0.2);
	tdoaTest.inputEnvironment(envPath);
	DataGenerateModel testDataModel = Sine2D;
	unsigned short int numTestSample = 200;
	float testParameter = float(1.26);
	tdoaTest.generateData(testDataModel, numTestSample, testParameter);
	tdoaTest.outputSample(noiseStandardDeviation, testSamplePath);
    taylorTest.test();
	/**/
	unsigned short int maxIteration = 200;
	float maxError = 0.25;
	bool initOPT = true;
    taylorTest.inputAnchorPosition(envPath);
    taylorTest.getResult(testSamplePath, testResultPath, maxIteration, maxError, initOPT);

	return 0;
}