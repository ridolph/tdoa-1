#include "PathTracking.h"
#include "BPnetwork.h"

void PathTracking::trakingSim(char* trackPATH, char* groupPATH, char* bpPATH, char* realPATH, char* locaPATH)
{
	inputTrack(trackPATH);
	inputGroup(groupPATH);
	inputBP(bpPATH);
	trackPath(realPATH, locaPATH);
}

void PathTracking::inputBP(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open PathTracking bp input File\n");
		exit(0);
	}
	IN >> numGroup >> noiseE >> noiseSD;
	for (unsigned short int i = 0; i < numGroup; i++)
		IN >> pathBP[i];
	IN.close();
}

void PathTracking::inputGroup(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open PathTracking group input File\n");
		exit(0);
	}
	IN >> dimSpace >> referDim >> numGroup;
	for (unsigned short int i = 0; i < numGroup; i++)
	{
		for (unsigned short int k = 0; k < dimSpace; k++)
			IN >> lowerLimit[i][k] >> upperLimit[i][k];
		IN >> numAnchor[i];
		for (unsigned short int j = 0; j < numAnchor[i]; j++)
			for (unsigned short int k = 0; k < dimSpace; k++)
				IN >> posAnchor[i][j][k];
	}
	IN.close();
}

void PathTracking::inputTrack(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open PathTracking track input File\n");
		exit(0);
	}
	IN >> numPoint >> dimSpace;
	for (unsigned short int p = 0; p < numPoint; p++)
		for (unsigned short int k = 0; k < dimSpace; k++)
			IN >> preTrack[p][k];
	IN.close();
}

void PathTracking::trackPath(char* outRealPath, char* outLocalize)
{
	std::default_random_engine genNoise((unsigned)time(NULL));
	std::normal_distribution<float> disNoise(noiseE, noiseSD*noiseSD);	
	std::ofstream OUTReal(outRealPath, std::ios::out);
	std::ofstream OUTLoca(outLocalize, std::ios::out);
	static BPnetwork bpTest;
	float posNow[PT_MAXDIM];
	
	for (unsigned short int k = 0; k < dimSpace; k++)
	{
		posNow[k] = preTrack[0][k];
		OUTReal << posNow[k] << "\t";
	}
	for (unsigned short int p = 1; p < numPoint; p++)
	{
		unsigned short int GP;
		for (GP = 0; GP < numGroup; GP++)
			if(lowerLimit[GP][referDim] < posNow[referDim] && posNow[referDim] <= upperLimit[GP][referDim])
				break;
		
		float posBPNN[BP_MAXDIM_LAYER];
		float d0 = float(0);
		for (unsigned short int k = 0; k < dimSpace; k++)
			d0 += (posNow[k]-posAnchor[GP][0][k])*(posNow[k]-posAnchor[GP][0][k]);
		d0 = sqrt(d0);
		for (unsigned short int j = 1; j < numAnchor[GP]; j++)
		{
			float dj_2 = float(0);
			for (unsigned short int k = 0; k < dimSpace; k++)
				dj_2 += (posNow[k]-posAnchor[GP][j][k])*(posNow[k]-posAnchor[GP][j][k]);
			posBPNN[j-1] = sqrt(dj_2) - d0;
			if (noiseSD > float(0)||noiseE != float(0))posBPNN[j-1] += disNoise(genNoise);
		}
		bpTest.callBP(pathBP[GP], posBPNN);
		for (unsigned short int k = 0; k < dimSpace; k++)
			posNow[k] += (preTrack[p][k] - posBPNN[k]);
		
		OUTReal << "\n";
		for (unsigned short int k = 0; k < dimSpace; k++)
		{
			OUTReal << posNow[k] << "\t";
			OUTLoca << posBPNN[k] << "\t";
		}
		OUTLoca << "\n";
	}
	
	OUTReal.close();
	OUTLoca.close();
}