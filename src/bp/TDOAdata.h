#pragma once
#include <fstream>
#include <cmath>
#include <ctime>
#include <random>

const unsigned short int TDOA_MAXDIM = 10;
const unsigned short int TDOA_MAXNUM_ANCHOR = 30;
const unsigned short int TDOA_MAXNUM_SAMPLE = 30000;

enum DataGenerateModel
{
	Random,
	Interval3D,
	Interval2D,
	Line1D,
	Sine2D,
	Helix3D
};

class TDOAdata
{
public:
	void generateSample(char* envPATH, char* outPATH, float noiseE, float noiseSD,
						DataGenerateModel DGM, unsigned short int num, float param);
	void generatePath(char* envPATH, char* outPATH,
						DataGenerateModel DGM, unsigned short int num, float param);
private:
	unsigned short int dimSpace;
	unsigned short int numAnchor;
	unsigned short int numSample;
	float minPos[TDOA_MAXDIM];
	float maxPos[TDOA_MAXDIM];
	float anchorPos[TDOA_MAXNUM_ANCHOR][TDOA_MAXDIM];
	float samplePos[TDOA_MAXNUM_SAMPLE][TDOA_MAXDIM];
	void inputEnvironment(char* inputPath);
	void generateData(DataGenerateModel DGM, unsigned short int num, float param);
	void outputSample(float noiseE, float noiseSD, char* outputPath);
	void outputPath(char* outputPath);
};