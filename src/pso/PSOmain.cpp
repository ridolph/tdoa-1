#include "Swarm.h"
#include "TDOAdata.h"

Swarm psoTest;
TDOAdata tdoaTest;

int main()
{
	char envPath[] = "data\\1env.txt";
	char smpPath[] = "data\\2smp.txt";
	char psoPath[] = "data\\3pso.txt";

	srand((unsigned)time(NULL));
	//tdoaTest.generateSample(envPath, smpPath, float(0.05), float(0.2), Random, 150, float(0));
	psoTest.ParticleSwarmOptimization(envPath, smpPath, psoPath, 30, 200, float(9999), float(1),
									float(2), float(2),	float(0.4), float(0.9),	false, true, false);

	return 0;
}