#pragma once
#include <fstream>
#include <cmath>
#include <ctime>
#include <random>

const unsigned short int PT_MAXDIM = 10;
const unsigned short int PT_MAXNUM_GROUP = 10;
const unsigned short int PT_MAXLEN_PATH = 100;
const unsigned short int PT_MAXNUM_ANCHOR = 30;
const unsigned short int PT_MAXNUM_POINT = 30000;

class PathTracking
{
public:
	void trakingSim(char* trackPATH, char* groupPATH, char* bpPATH, char* realPATH, char* locaPATH);

private:
	unsigned short int numGroup;
	unsigned short int numPoint;
	unsigned short int dimSpace;
	unsigned short int referDim;
	unsigned short int numAnchor[PT_MAXNUM_GROUP];
	char pathBP[PT_MAXNUM_GROUP][PT_MAXLEN_PATH];
	float preTrack[PT_MAXNUM_POINT][PT_MAXDIM];
	float lowerLimit[PT_MAXNUM_GROUP][PT_MAXDIM];
	float upperLimit[PT_MAXNUM_GROUP][PT_MAXDIM];
	float posAnchor[PT_MAXNUM_GROUP][PT_MAXNUM_ANCHOR][PT_MAXDIM];
	float noiseE, noiseSD;
	void inputTrack(char* inputPath);
	void inputGroup(char* inputPath);
	void inputBP(char* inputPath);
	void trackPath(char* outRealPath, char* outLocalize);
};