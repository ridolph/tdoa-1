#pragma once
#include <fstream>
#include <cmath>

const unsigned short int Chan_MAXDIM = 3;
const unsigned short int Chan_MAXANC = 4;

class Chan
{
public:
    void testChan(char* envPATH, char* smpPATH, char* outPATH);
private:
    float anchorPos[Chan_MAXANC][Chan_MAXDIM];
    void inputAnchorPosition(char* inputPath);
	void getResult(char* inputPath, char* outputPath);
};