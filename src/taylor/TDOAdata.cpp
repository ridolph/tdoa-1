#include "TDOAdata.h"

void TDOAdata::generateSample(char* envPATH, char* outPATH, float noiseE, float noiseSD,
							DataGenerateModel DGM, unsigned short int num, float param)
{	
	inputEnvironment(envPATH);
	generateData(DGM, num, param);
	outputSample(noiseE, noiseSD, outPATH);
}

void TDOAdata::generatePath(char* envPATH, char* outPATH,
							DataGenerateModel DGM, unsigned short int num, float param)
{	
	inputEnvironment(envPATH);
	generateData(DGM, num, param);
	outputPath(outPATH);
}


void TDOAdata::inputEnvironment(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open TDOAdata input File\n");
		exit(0);
	}
	IN >> dimSpace;
	for (unsigned short int k = 0; k < dimSpace; k++)
		IN >> minPos[k] >> maxPos[k];
	IN >> numAnchor;
	for (unsigned short int i = 0; i < numAnchor; i++)
		for (unsigned short int k = 0; k < dimSpace; k++)
			IN >> anchorPos[i][k];
	IN.close();
}

void TDOAdata::generateData(DataGenerateModel DGM, unsigned short int num, float param)
{
	switch (DGM)
	{
	case Interval3D:{
		numSample = 1;
		for (unsigned short int k = 0; k < dimSpace; k++)
			numSample *= int((maxPos[k] - minPos[k]) / param);
		unsigned short int delta = 1;
		for (unsigned short int k = 0; k < dimSpace; k++)
		{
			unsigned short int T = int((maxPos[k] - minPos[k]) / param);
			for (unsigned short int t = 0; t < T; t++)
				for (unsigned short int st = t * delta; st < numSample; st += delta * T)
					for (unsigned short int cnt = st; cnt < st + delta; cnt++)
						samplePos[cnt][k] = minPos[k] + param * t + param / 2;
			delta *= T;
		}
		for (unsigned short int s1 = 0; s1 < numSample; s1++)
		{
			unsigned short int s2 = rand() % numSample;
			for (unsigned short int k = 0; k < dimSpace; k++)
			{
				float temp = samplePos[s1][k];
				samplePos[s1][k] = samplePos[s2][k];
				samplePos[s2][k] = temp;
			}
		}
		break;
	    }
	case Interval2D:{
		numSample=0;
		float h = (maxPos[2]-minPos[2]) * float(rand()/float(RAND_MAX));
		for(unsigned short int i = 0; i < int((maxPos[0]-minPos[0])/param); i++)
			for(unsigned short int j = 0; j < int((maxPos[1]-minPos[1])/param); j++)
			{
				samplePos[numSample][0] = param * i + param / 2;
				samplePos[numSample][1] = param * j + param / 2;
				samplePos[numSample][2] = h;
				numSample++;
			}
		for (unsigned short int s1 = 0; s1 < numSample; s1++)
		{
			unsigned short int s2 = rand() % numSample;
			for (unsigned short int k = 0; k < dimSpace; k++)
			{
				float temp = samplePos[s1][k];
				samplePos[s1][k] = samplePos[s2][k];
				samplePos[s2][k] = temp;
			}
		}
		break;
	    }
	case Line1D:{
		numSample = num;
		float d = (maxPos[0] - minPos[0]) / numSample;
		for (unsigned short int s = 0; s < numSample; s++)
		{
			samplePos[s][0] = -d * s + maxPos[0];
			samplePos[s][1] = param*(maxPos[1]-minPos[1])+minPos[1];
			samplePos[s][2] = param*(maxPos[2]-minPos[2])+minPos[2];
		}
		break;
		}
	case Sine2D:{
		numSample = num;
		float d = (maxPos[0] - minPos[0]) / numSample;
		for (unsigned short int s = 0; s < numSample; s++)
		{
			samplePos[s][0] = -d * s + maxPos[0];
			samplePos[s][1] = sin(samplePos[s][0]*2)*(maxPos[1]-minPos[1])*0.2+(minPos[1]+maxPos[1])/2;
			samplePos[s][2] = param*(maxPos[2]-minPos[2])+minPos[2];
		}
		break;
	    }
	case Helix3D:{
		numSample = num;
		float pi = float(3.141592654);
		float d = param * pi * 2 /numSample;
		float r = (maxPos[2]-minPos[2])*0.2;
		if(r > (maxPos[1]-minPos[1])*0.2)r = (maxPos[1]-minPos[1])*0.2;
		for (unsigned short int s = 0; s < numSample; s++)
		{
			samplePos[s][2] = r*sin(d*s)+(minPos[2]+maxPos[2])/2;
			samplePos[s][1] = r*cos(d*s)+(minPos[1]+maxPos[1])/2;
			samplePos[s][0] = -d*s*(maxPos[0]-minPos[0])/param/(pi*2)+maxPos[0];
		}
		break;
	    }
	default:{
		numSample = num;
		for (unsigned short int k = 0; k < dimSpace; k++)
			for (unsigned short int s = 0; s < numSample; s++)
				samplePos[s][k] = (maxPos[k] - minPos[k])*rand() / float(RAND_MAX) + minPos[k];
		break;
	    }
	}
}

void TDOAdata::outputSample(float noiseE, float noiseSD, char* outputPath)
{
	std::default_random_engine genNoise((unsigned)time(NULL));
	std::normal_distribution<float> disNoise(noiseE, noiseSD*noiseSD);	
	std::ofstream OUT(outputPath, std::ios::out);
	OUT << numSample << "\t" << numAnchor - 1 << "\t" << dimSpace << "\n";
	for (unsigned short int s = 0; s < numSample; s++)
	{
		float d0 = 0.0;
		for (unsigned short int k = 0; k < dimSpace; k++)
			d0 += (samplePos[s][k] - anchorPos[0][k])*(samplePos[s][k] - anchorPos[0][k]);
		d0 = sqrt(d0);
		for (unsigned short int i = 1; i < numAnchor; i++)
		{
			float di0 = 0.0;
			for (unsigned short int k = 0; k < dimSpace; k++)
				di0 += (samplePos[s][k] - anchorPos[i][k])*(samplePos[s][k] - anchorPos[i][k]);
			di0 = sqrt(di0) - d0;
			if (noiseSD > float(0)||noiseE != float(0))di0 += disNoise(genNoise);
			OUT << di0 << "\t";
		}
		for (unsigned short int k = 0; k < dimSpace; k++)
			OUT << samplePos[s][k] << "\t";
		OUT << "\n";
	}
	OUT.close();
}

void TDOAdata::outputPath(char* outputPath)
{
	std::ofstream OUT(outputPath, std::ios::out);
	OUT << numSample << "\t" << dimSpace << "\n";
	for (unsigned short int s = 0; s < numSample; s++)
	{
		for (unsigned short int k = 0; k < dimSpace; k++)
			OUT << samplePos[s][k] << "\t";
		OUT << "\n";
	}
	OUT.close();
}