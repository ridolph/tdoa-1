#include "Chan.h"
#include "TDOAdata.h"

TDOAdata tdoaTest;
Chan chanTest;

int main()
{
	char envPath[] = "data\\1env.txt";
	char smpPath[] = "data\\2smp.txt";
	char chanPath[] = "data\\3chan.txt";

	srand((unsigned)time(NULL));
   	//tdoaTest.generateSample(envPath, smpPath, float(0.05), float(0.2), Sine2D, 100, float(1.26));
    chanTest.testChan(envPath, smpPath, chanPath);

	return 0;
}