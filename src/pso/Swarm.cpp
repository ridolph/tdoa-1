#include "Swarm.h"

void Swarm::inputEnvironment(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open PSO env File\n");
		exit(0);
	}
	IN >> dimOUT;
	for (unsigned short int k = 0; k < dimOUT; k++)
		IN >> minPos[k] >> maxPos[k];
	IN >> dimIN;
	for (unsigned short int i = 0; i < dimIN; i++)
		for (unsigned short int k = 0; k < dimOUT; k++)
			IN >> anchorPos[i][k];
	dimIN--;
	IN.close();
}

void Swarm::initializeState(bool OPTH, float maxV)
{
	for (unsigned short int j = 0; j < numParticle; j++)
	{
		for (unsigned short int k = 0; k < dimOUT; k++)
			par[j].v[k] = maxV * rand() / float(RAND_MAX);
		if (!OPTH)
		{
			for (unsigned short int k = 0; k < dimOUT; k++)
				par[j].p[k] = (maxPos[k] - minPos[k])*rand() / float(RAND_MAX) + minPos[k];
		}
		else
		{
			float d10_2 = sampleX[1] * sampleX[1];
			float r10_2 = 0.0;
			for (unsigned short int k = 0; k < dimOUT; k++)
				r10_2 += (anchorPos[1][k] - anchorPos[0][k])*(anchorPos[1][k] - anchorPos[0][k]);
			float maxL = (r10_2 - d10_2) / (sqrt(d10_2) * 2);
			if (maxL > (maxPos[1] - minPos[1]))maxL = maxPos[1] - minPos[1];
			float L = maxL * rand() / float(RAND_MAX);
			float maxD = float(3.1415926536 / 2);
			if ((maxPos[2] - minPos[2]) < L)maxD = asin((maxPos[2] - minPos[2]) / L);
			float D = maxD * rand() / float(RAND_MAX);
			par[j].p[0] = (-sampleX[1] / 2)*sqrt(1 + L * L * 4 / (r10_2 - d10_2));
			par[j].p[1] = L * cos(D);
			par[j].p[2] = L * sin(D);
			for (unsigned short int k = 0; k < dimOUT; k++)
				par[j].p[k] += ((anchorPos[0][k] + anchorPos[1][k]) / 2);
		}
	}
}

void Swarm::getFitnessValue()
{
	avgFit = 0.0;
	maxFit = 0.0;
	for (unsigned short int j = 0; j < numParticle; j++)
	{
		float d0 = 0.0;
		for (unsigned short int k = 0; k < dimOUT; k++)
			d0 += (par[j].p[k] - anchorPos[0][k])*(par[j].p[k] - anchorPos[0][k]);
		d0 = sqrt(d0);
		
		par[j].f = 0.0;
		for (unsigned short int i = 1; i <= dimIN; i++)
		{
			float di = 0.0;
			for (unsigned short int k = 0; k < dimOUT; k++)
				di += (par[j].p[k] - anchorPos[i][k])*(par[j].p[k] - anchorPos[i][k]);
			di = sqrt(di);
			par[j].f += ((sampleX[i] + d0 - di)*(sampleX[i] + d0 - di));
		}
		par[j].f = 1 / par[j].f;
		avgFit += par[j].f;
		if (par[j].f > maxFit)maxFit = par[j].f;
		if (par[j].f > par[j].bFit)
		{
			par[j].bFit = par[j].f;
			for (unsigned short int k = 0; k < dimOUT; k++)
				par[j].bPos[k] = par[j].p[k];
		}
		if (par[j].f > gFit)
		{
			gFit = par[j].f;
			for (unsigned short int k = 0; k < dimOUT; k++)
				gPos[k] = par[j].p[k];
		}
	}
	avgFit /= float(numParticle);
}

void Swarm::updateState(bool OPTW, float C1, float C2, float minW, float maxW, float maxV)
{
	for (unsigned short int j = 0; j < numParticle; j++)
	{
		float w = (minW + maxW) / 2;
		if (OPTW)
		{
			if (par[j].f <= avgFit)w = maxW;
			else w = minW + (maxW - minW)*(maxFit - par[j].f) / (maxFit - avgFit);
		}
		
		float r1 = float(rand()) / float(RAND_MAX);
		float r2 = float(rand()) / float(RAND_MAX);
		float speed = 0;
		for (unsigned short int k = 0; k < dimOUT; k++)
		{
			par[j].v[k] *= w;
			par[j].v[k] += r1*C1*(par[j].bPos[k] - par[j].p[k]);
			par[j].v[k] += r2*C2*(gPos[k] - par[j].p[k]);
			speed += (par[j].v[k] * par[j].v[k]);
		}
		speed = sqrt(speed);
		if (speed > maxV)
		{
			for (unsigned short int k = 0; k < dimOUT; k++)
				par[j].v[k] *= (maxV / speed);
		}
		
		for (unsigned short int k = 0; k < dimOUT; k++)
		{
			par[j].p[k] += par[j].v[k];
			if (par[j].p[k] > maxPos[k])par[j].p[k] = maxPos[k];
			if (par[j].p[k] < minPos[k])par[j].p[k] = minPos[k];
		}
	}
}

void Swarm::ParticleSwarmOptimization(char* envPATH, char* smpPATH, char* outPATH,
		unsigned short int pNum, unsigned short int maxITER, float maxF, float maxV,
		float C1, float C2, float minW, float maxW,	bool OPTW, bool OPTH, bool OPTT)
{	
	inputEnvironment(envPATH);
	std::ifstream IN(smpPATH, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open PSO smp File\n");
		exit(0);
	}
	std::ofstream OUT(outPATH, std::ios::out);

	numParticle = pNum;
	unsigned short int sampleN;
	IN >> sampleN >> dimIN >> dimOUT;
	unsigned short int it = maxITER;
	for (unsigned short int s = 0; s < sampleN; s++)
	{
		float expY[PSO_MAXDIM_OUTPUT];
		for (unsigned short int i = 1; i <= dimIN; i++)
			IN >> sampleX[i];
		for (unsigned short int k = 0; k < dimOUT; k++)
			IN >> expY[k];

		gFit = 0.0;
		for (unsigned short int j = 0; j < numParticle; j++)
			par[j].bFit = 0.0;
		if ((!OPTT) || (it == maxITER))initializeState(OPTH, maxV);
		for (it = 0; it < maxITER; it++)
		{
			getFitnessValue();
			if (gFit > maxF)break;
			updateState(OPTW, C1, C2, minW, maxW, maxV);
		}

		OUT << it << "\t";
		for (unsigned short int k = 0; k < dimOUT; k++)
			OUT << gPos[k] << "\t";
		for (unsigned short int k = 0; k < dimOUT; k++)
			OUT << expY[k] << "\t";
		OUT << "\n";
	}

	IN.close();
	OUT.close();
}