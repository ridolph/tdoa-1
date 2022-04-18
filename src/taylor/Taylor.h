#pragma once
#include <fstream>
#include <cmath>
#include <ctime>

const unsigned short int Taylor_MAXDIM = 3;
const unsigned short int Taylor_MAXANC = 30;

class Taylor
{
public:
    void inputAnchorPosition(char* inputPath);
	void getResult(char* inputPath, char* outputPath, unsigned short int maxITER, float maxError, bool OPT_INIT);
    void test();

private:
    float anchorPos[Taylor_MAXANC][Taylor_MAXDIM];
    float minPos[Taylor_MAXDIM];
    float maxPos[Taylor_MAXDIM];
    float detMat(float* mat, unsigned short int n);
    void invMat(float* mat, unsigned short int n, float* ans);
};