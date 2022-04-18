#include "Chan.h"

void Chan::testChan(char* envPATH, char* smpPATH, char* outPATH)
{
    inputAnchorPosition(envPATH);
    getResult(smpPATH, outPATH);
}

void Chan::inputAnchorPosition(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open TDOAdata input File\n");
		exit(0);
	}
	unsigned short int numAnchor, dimSpace;
	float tmp;
	IN >> dimSpace;
	for (unsigned short int k = 0; k < dimSpace; k++)
		IN >> tmp >> tmp;
	IN >> numAnchor;
	for (unsigned short int i = 0; i < numAnchor; i++)
		for (unsigned short int k = 0; k < dimSpace; k++)
			IN >> anchorPos[i][k];
	IN.close();
}

void Chan::getResult(char* inputPath, char* outputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open TDOAdata input File\n");
		exit(0);
	}
	std::ofstream OUT(outputPath, std::ios::out);

	unsigned short int numSample, numAnchor, dimSpace;
	IN >> numSample >> numAnchor >> dimSpace;
	for (unsigned short int t = 0; t < numSample; t++)
	{
		float d0[numAnchor+1], realPos[dimSpace], chanPos[dimSpace];
		for(unsigned short int i=1; i<=numAnchor; i++)
			IN >> d0[i];
		for(unsigned short int k=0; k<dimSpace; k++)
		    IN>>realPos[k];

		float a[dimSpace], b[dimSpace];
		float A=float(-1), B=float(0), C=float(0);
		for(unsigned short int k=0; k<dimSpace; k++)
		{
			a[k]=-d0[k+1]/anchorPos[k+1][k];
			b[k]=(anchorPos[k+1][k]-d0[k+1]*d0[k+1]/anchorPos[k+1][k])*0.5;
			A+=a[k]*a[k];
			B+=a[k]*b[k]*2;
			C+=b[k]*b[k];
		}
		float d00=(-B-sqrt(B*B-A*C*4))/(A*2);
		float error=float(0);
		for(unsigned short int k=0; k<dimSpace; k++)
		{
			chanPos[k]=a[k]*d00+b[k];
			error+=(chanPos[k]-realPos[k])*(chanPos[k]-realPos[k]);
		}
		error = sqrt(error);
		OUT << error << "\t";
		for(unsigned short int k=0; k<dimSpace; k++)
			OUT << chanPos[k] << "\t";
		for(unsigned short int k=0; k<dimSpace; k++)
			OUT << realPos[k] << "\t";
		OUT << "\n";
	}
	OUT.close();
}