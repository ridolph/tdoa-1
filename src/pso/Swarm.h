#pragma once
#include <fstream>
#include <cmath>

const int PSO_MAXDIM_INPUT = 30;
const int PSO_MAXDIM_OUTPUT = 10;
const int PSO_MAXNUM_PARTICLE = 100;

class Swarm
{
public:
	void ParticleSwarmOptimization(char* envPATH, char* smpPATH, char* outPATH,
		unsigned short int pNum, unsigned short int maxITER, float maxF, float maxV,
		float C1, float C2, float minW, float maxW,	bool OPTW, bool OPTH, bool OPTT);
	
private:
	unsigned short int dimIN;
	unsigned short int dimOUT;
	float minPos[PSO_MAXDIM_OUTPUT];
	float maxPos[PSO_MAXDIM_OUTPUT];
	float anchorPos[PSO_MAXDIM_INPUT][PSO_MAXDIM_OUTPUT];

	unsigned short int numParticle;
	float sampleX[PSO_MAXDIM_INPUT];

	struct Particle
	{
		float v[PSO_MAXDIM_OUTPUT];
		float p[PSO_MAXDIM_OUTPUT];
		float f;
		float bPos[PSO_MAXDIM_OUTPUT];
		float bFit;
	}par[PSO_MAXNUM_PARTICLE];
	float gPos[PSO_MAXDIM_OUTPUT];
	float gFit;
	float maxFit;
	float avgFit;

	void inputEnvironment(char* inputPath);
	void initializeState(bool OPTH, float maxV);
	void getFitnessValue();
	void updateState(bool OPTW,float C1, float C2, float minW, float maxW, float maxV);
};