#include "RBFnetwork.h"

void RBFnetwork::trainRBF(char* trainPATH, char* logPATH, char* outPATH, unsigned short int dimRBF,
						RadialBasisFunction rFunc, unsigned int maxITER, float RMSE, float* adamParam)
{	
	inputSample(trainPATH);
	kmeansCluster(dimRBF, rFunc);
	updateSampleX();
	trainWeight(logPATH, maxITER, RMSE, adamParam);
	saveRBFnetwork(outPATH);
}

void RBFnetwork::testRBF(char* inPATH, char* testPATH, char* resultPATH)
{
	loadRBFnetwork(inPATH);
	inputSample(testPATH);
	updateSampleX();
	getResult(resultPATH);
}

void RBFnetwork::inputSample(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open RBFnetwork train input File\n");
		exit(0);
	}
	IN >> numSample >> dimLayer[0] >> dimLayer[2];
	for (unsigned short int s = 0; s < numSample; s++)
	{
		for (unsigned short int i = 0; i < dimLayer[0]; i++)
			IN >> sampleX[s][i];
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			IN >> sampleY[s][k];
	}
	IN.close();
}

void RBFnetwork::kmeansCluster(unsigned short int dimRBF, RadialBasisFunction rFunc)
{	
	dimLayer[1] = dimRBF;
	rbFun = rFunc;
	for (unsigned short int k = 0; k < dimLayer[1]; k++)
		for (unsigned short int j = 0; j < dimLayer[0]; j++)
			rbfCenter[k][j] = sampleX[k][j];
	while(1)
	{
		float tmpRBF[RBF_MAXDIM_LAYER][RBF_MAXDIM_LAYER] = { 0.0 };
		unsigned short int cnt[RBF_MAXDIM_LAYER] = { 0 };
		unsigned short int CLUSTER;
		for (unsigned short int s = 0; s < numSample; s++)
		{
			float minD_2 = RBF_MAX;
			for (unsigned short int k = 0; k < dimLayer[1]; k++)
			{
				float tmpD_2 = 0.0;
				for (unsigned short int j = 0; j < dimLayer[0]; j++)
					tmpD_2 += (sampleX[s][j] - rbfCenter[k][j])*(sampleX[s][j] - rbfCenter[k][j]);
				if (tmpD_2 < minD_2)
				{
					minD_2 = tmpD_2;
					CLUSTER = k;
				}
			}
			cnt[CLUSTER]++;
			for (unsigned short int j = 0; j < dimLayer[0]; j++)
				tmpRBF[CLUSTER][j] += sampleX[s][j];
		}
		for (unsigned short int k = 0; k < dimLayer[1]; k++)
			for (unsigned short int j = 0; j < dimLayer[0]; j++)
				tmpRBF[k][j] /= float(cnt[k]);
		
		bool noCHANGE = true;
		for (unsigned short int k = 0; k < dimLayer[1]; k++)
		{
			for (unsigned short int j = 0; j < dimLayer[0]; j++)
				if (tmpRBF[k][j] != rbfCenter[k][j])
				{
					noCHANGE = false;
					break;
				}
			if (noCHANGE == false)break;
		}
		if (noCHANGE)break;
		for (unsigned short int k = 0; k < dimLayer[1]; k++)
			for (unsigned short int j = 0; j < dimLayer[0]; j++)
				rbfCenter[k][j] = tmpRBF[k][j];
	}

	float maxD_2 = 0.0;
	for (unsigned short int k1 = 0; k1 < dimLayer[1] - 1; k1++)
		for (unsigned short int k2 = k1 + 1; k2 < dimLayer[1]; k2++)
		{
			float tmpD_2 = 0.0;
			for (unsigned short int j = 0; j < dimLayer[0]; j++)
				tmpD_2 += (rbfCenter[k1][j] - rbfCenter[k2][j])*(rbfCenter[k1][j] - rbfCenter[k2][j]);
			if (tmpD_2 > maxD_2)maxD_2 = tmpD_2;
		}
	rbfConst = float(dimLayer[1] / maxD_2);
}

float RBFnetwork::calRBF(float r_2)
{
	switch (rbFun)
	{
	case InverseMultiquadric:
		return float(1 / sqrt(r_2 * rbfConst + 1));
	case LaplacianKernel:
		return exp(-sqrt(r_2 * rbfConst * 2));
	case ExponentialKernel:
		return exp(-sqrt(r_2) *  rbfConst);
	default:
		return exp(-r_2 * rbfConst);
	}
}

void RBFnetwork::updateSampleX()
{
	for (unsigned short int s = 0; s < numSample; s++)
	{
		float tmp[RBF_MAXDIM_LAYER];
		for (unsigned short int k = 0; k < dimLayer[1]; k++)
		{
			float r_2 = 0.0;
			for (unsigned short int j = 0; j <= dimLayer[0]; j++)
				r_2 += (sampleX[s][j] - rbfCenter[k][j])*(sampleX[s][j] - rbfCenter[k][j]);
			tmp[k] = calRBF(r_2);
		}
		for (unsigned short int k = 0; k < dimLayer[1]; k++)
			sampleX[s][k] = tmp[k];
		sampleX[s][dimLayer[1]] = float(1);
	}
}

void RBFnetwork::trainWeight(char* outputPath, unsigned int maxITER, float RMSE, float* learningParam)
{
	std::ofstream OUT(outputPath, std::ios::out);
	for (unsigned short int j = 0; j <= dimLayer[1]; j++)
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			w[j][k] = float(rand() / float(RAND_MAX) - 0.5);	
	float m[RBF_MAXDIM_LAYER][RBF_MAXDIM_LAYER] = {0.0};
	float v[RBF_MAXDIM_LAYER][RBF_MAXDIM_LAYER] = {0.0};
	float beta1Correct = float(1.0);
	float beta2Correct = float(1.0);
	for (unsigned int it = 0; it < maxITER; it++)
	{
		beta1Correct *= learningParam[1];
		beta2Correct *= learningParam[2];
		float loss = 0.0;
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
		{
			float g[RBF_MAXDIM_LAYER] = { 0.0 };
			for (unsigned short int s = 0; s < numSample; s++)
			{
				float ey = - sampleY[s][k];
				for (unsigned short int j = 0; j <= dimLayer[1]; j++)
					ey += sampleX[s][j] * w[j][k];
				loss += ey * ey;
				for (unsigned short int j = 0; j <= dimLayer[1]; j++)
					g[j] += ey * sampleX[s][j] / numSample;
			}
			for (unsigned short int j = 0; j <= dimLayer[1]; j++)
			{
				m[j][k] = m[j][k] * learningParam[1] + g[j] * (1 - learningParam[1]);
				v[j][k] = v[j][k] * learningParam[2] + g[j] * g[j] * (1 - learningParam[2]);
				float mCorrect = m[j][k] / (1 - beta1Correct);
				float vCorrect = v[j][k] / (1 - beta2Correct);
				w[j][k] -= mCorrect * learningParam[0] / (learningParam[3] + sqrt(vCorrect));
			}
		}
		OUT << it << "\t" << sqrt(loss / numSample) << "\n";
		printf("%d:\t%lf\n", it, sqrt(loss / numSample));
		if (sqrt(loss / numSample) < RMSE)break;
	}
	OUT.close();
}

void RBFnetwork::saveRBFnetwork(char* outputPath)
{
	std::ofstream OUT(outputPath, std::ios::out);
	OUT << dimLayer[0] << "\t" << dimLayer[2] << "\t" << dimLayer[1] << "\t" << rbfConst << "\t" << rbFun << "\n";
	for (unsigned short int k = 0; k < dimLayer[1]; k++)
	{
		for (unsigned short int j = 0; j < dimLayer[0]; j++)
			OUT << rbfCenter[k][j] << "\t";
		OUT << "\n";
	}
	OUT << "\n";
	for (unsigned short int j = 0; j <= dimLayer[1]; j++)
	{
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			OUT << w[j][k] << "\t";
		OUT << "\n";
	}
	OUT.close();
}

void RBFnetwork::loadRBFnetwork(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open RBFnetwork net input File\n");
		exit(0);
	}
	unsigned short int funcModel;
	IN >> dimLayer[0] >> dimLayer[2] >> dimLayer[1] >> rbfConst >> funcModel;
	rbFun = GaussianKernel;
	if (funcModel == 1)rbFun = ExponentialKernel;
	if (funcModel == 2)rbFun = LaplacianKernel;
	if (funcModel == 3)rbFun = InverseMultiquadric;
	for (unsigned short int k = 0; k < dimLayer[1]; k++)
		for (unsigned short int j = 0; j < dimLayer[0]; j++)
			IN >> rbfCenter[k][j];
	for (unsigned short int j = 0; j <= dimLayer[1]; j++)
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			IN >> w[j][k];
	IN.close();
}

void RBFnetwork::getResult(char* outputPath)
{
	std::ofstream OUT(outputPath, std::ios::out);
	for (unsigned short int s = 0; s < numSample; s++)
	{
		float testY[RBF_MAXDIM_LAYER] = {0.0};
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			for (unsigned short int j = 0; j <= dimLayer[1]; j++)
				testY[k] += sampleX[s][j] * w[j][k];
		float error = float(0);
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			error += (testY[k] - sampleY[s][k])*(testY[k] - sampleY[s][k]);
		error = sqrt(error);
		OUT << error << "\t";
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			OUT << testY[k] << "\t";
		for (unsigned short int k = 0; k < dimLayer[2]; k++)
			OUT << sampleY[s][k] << "\t";
		OUT << "\n";
	}
	OUT.close();
}