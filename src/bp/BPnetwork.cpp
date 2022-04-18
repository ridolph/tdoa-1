#include "BPnetwork.h"

void BPnetwork::trainBP(char* trainPATH, char* logPATH, char* outPATH,
						unsigned int maxITER, float RMSE, float* adamParam)
{	
	inputSample(trainPATH);
	dataNormalize();
	trainWeight(logPATH, maxITER, RMSE, adamParam);
	saveBPnetwork(outPATH);
}

void BPnetwork::testBP(char* inPATH, char* testPATH, char* resultPATH)
{
	loadBPnetwork(inPATH);
	inputSample(testPATH);
	getResult(resultPATH);
}

void BPnetwork::callBP(char* inPATH, float* X)
{
	loadBPnetwork(inPATH);
	forwardCal(X);
}

float BPnetwork::activateNet(float x, ActivationFunction aFunc)
{
	switch (aFunc)
	{
	case Sigmoid:
		return float(1 / (exp(-x) + 1));
	case Tanh:
		return float(1 - 2 / (exp(x * 2) + 1));
	case ReLU:
		if (x > 0)return x;
		else return float(0);
	case LeakyReLU:
		if (x > 0)return x;
		else return x * float(0.01);
	case ELU:
		if (x > 0)return x;
		else return exp(x) - float(1);
	case Softplus:
		return log(exp(x) + 1);
	default:
		return x;
	}
}

float BPnetwork::acFunDiff(float y, ActivationFunction aFunc)
{
	switch (aFunc)
	{
	case Sigmoid:
		return y * (1 - y);
	case Tanh:
		return float(1 - y * y);
	case ReLU:
		if (y > 0)return float(1);
		else return float(0);
	case LeakyReLU:
		if (y > 0)return float(1);
		else return float(0.01);
	case ELU:
		if (y > 0)return float(1);
		else return y + 1;
	case Softplus:
		return float(1 - 1 / exp(y));
	default:
		return float(1);
	}
}

void BPnetwork::dataNormalize()
{
	for (unsigned short int j = 0; j < dimLayer[0]; j++)
	{
		xSD[j] = float(0);
		xMean[j] = float(0);
	}
	for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
	{
		ySD[k] = float(0);
		yMean[k] = float(0);
	}
	for (unsigned short int s = 0; s < numSample; s++)
	{
		for (unsigned short int j = 0; j < dimLayer[0]; j++)
			xMean[j] += sampleX[s][j]/numSample;
		for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
			yMean[k] += sampleY[s][k]/numSample;
	}
	for (unsigned short int s = 0; s < numSample; s++)
	{
		for (unsigned short int j = 0; j < dimLayer[0]; j++)
			xSD[j] += (sampleX[s][j]-xMean[j])*(sampleX[s][j]-xMean[j]);
		for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
			ySD[k] += (sampleY[s][k]-yMean[k])*(sampleY[s][k]-yMean[k]);
	}
	for (unsigned short int j = 0; j < dimLayer[0]; j++)
		xSD[j] = sqrt(xSD[j]/numSample);
	for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
		ySD[k] = sqrt(ySD[k]/numSample);

	for (unsigned short int s = 0; s < numSample; s++)
	{
		for (unsigned short int j = 0; j < dimLayer[0]; j++)
			sampleX[s][j] = (sampleX[s][j]-xMean[j])/xSD[j];
		for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
			sampleY[s][k] = (sampleY[s][k]-yMean[k])/ySD[k];
	}
}

void BPnetwork::inputSample(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open BPnetwork train input File\n");
		exit(0);
	}
	IN >> numSample >> dimLayer[0] >> dimLayer[numHidden + 1];
	for (unsigned short int s = 0; s < numSample; s++)
	{
		for (unsigned short int j = 0; j < dimLayer[0]; j++)
			IN >> sampleX[s][j];
		for (unsigned short int k = 0; k < dimLayer[numHidden + 1]; k++)
			IN >> sampleY[s][k];
	}
	IN.close();
}

void BPnetwork::trainWeight(char* outputPath, unsigned int maxITER, float RMSE, float* adamParam)
{
	std::ofstream OUT(outputPath, std::ios::out);

	for (unsigned short int i = 0; i <= numHidden; i++)
		for (unsigned short int j = 0; j <= dimLayer[i]; j++)
			for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
				w[i][j][k] = float(rand() / float(RAND_MAX) - 0.5);
	
	static float m[BP_MAXNUM_LAYER][BP_MAXDIM_LAYER][BP_MAXDIM_LAYER] = { 0.0 };
	static float v[BP_MAXNUM_LAYER][BP_MAXDIM_LAYER][BP_MAXDIM_LAYER] = { 0.0 };
	float beta1Correct = float(1.0);
	float beta2Correct = float(1.0);
	for (unsigned int it = 0; it < maxITER; it++)
	{
		float g[BP_MAXNUM_LAYER][BP_MAXDIM_LAYER][BP_MAXDIM_LAYER] = { 0.0 };
		float loss = 0.0;
		for (unsigned short int s = 0; s < numSample; s++)
		{
			float a[BP_MAXNUM_LAYER][BP_MAXDIM_LAYER] = { 0.0 };
			float d[BP_MAXNUM_LAYER][BP_MAXDIM_LAYER] = { 0.0 };

			for (unsigned short int j = 0; j < dimLayer[0]; j++)
				a[0][j] = activateNet(sampleX[s][j], acFun[0]);
			for (unsigned short int i = 0; i <= numHidden; i++)
			{
				a[i][dimLayer[i]] = float(1);
				for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
					for (unsigned short int j = 0; j <= dimLayer[i]; j++)
						a[i + 1][k] += a[i][j] * w[i][j][k];
				for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
					a[i + 1][k] = activateNet(a[i + 1][k], acFun[i + 1]);
			}

			for (unsigned short int k = 0; k < dimLayer[numHidden + 1]; k++)
			{
				loss += (a[numHidden + 1][k] - sampleY[s][k])*(a[numHidden + 1][k] - sampleY[s][k]);
				d[numHidden + 1][k] = (a[numHidden + 1][k] - sampleY[s][k]);
				d[numHidden + 1][k] *= acFunDiff(a[numHidden + 1][k], acFun[numHidden + 1]);
			}
			for (short int i = numHidden; i >= 0; i--)
				for (unsigned short int j = 0; j <= dimLayer[i]; j++)
				{
					for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
						d[i][j] += d[i + 1][k] * w[i][j][k];
					d[i][j] *= acFunDiff(a[i][j], acFun[i]);
				}
			for (unsigned short int i = 0; i <= numHidden; i++)
				for (unsigned short int j = 0; j <= dimLayer[i]; j++)
					for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
						g[i][j][k] += (a[i][j] * d[i + 1][k] / numSample);
		}

		OUT << it << "\t" << sqrt(loss / numSample) << "\n";
		if (sqrt(loss / numSample) < RMSE)break;
		printf("%d:\t%lf\n", it, sqrt(loss / numSample));
		
		beta1Correct *= adamParam[1];
		beta2Correct *= adamParam[2];
		for (unsigned short int i = 0; i <= numHidden; i++)
			for (unsigned short int j = 0; j <= dimLayer[i]; j++)
				for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
				{
					m[i][j][k] = m[i][j][k] * adamParam[1] + g[i][j][k] * (1 - adamParam[1]);
					v[i][j][k] = v[i][j][k] * adamParam[2] + g[i][j][k] * (1 - adamParam[2]) * g[i][j][k];
					float mCorrect = m[i][j][k] / (1 - beta1Correct);
					float vCorrect = v[i][j][k] / (1 - beta2Correct);
					w[i][j][k] -= mCorrect * adamParam[0] / (adamParam[3] + sqrt(vCorrect));
				}
	}
	OUT.close();
}

void BPnetwork::saveBPnetwork(char* outputPath)
{
	std::ofstream OUT(outputPath, std::ios::out);
	OUT << numHidden << "\n";
	for (unsigned short int i = 0; i <= numHidden + 1; i++)
		OUT << dimLayer[i] << "\t" << acFun[i] << "\n";
	
	OUT << "\n";
	for (unsigned short int j = 0; j < dimLayer[0]; j++)
		OUT << xMean[j] << "\t" << xSD[j] << "\n";
	for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
		OUT << yMean[k] << "\t" << ySD[k] << "\n";

	for (unsigned short int i = 0; i <= numHidden; i++)
	{
		OUT << "\n";
		for (unsigned short int j = 0; j <= dimLayer[i]; j++)
		{
			for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
				OUT << w[i][j][k] << "\t";
			OUT << "\n";
		}
	}
	OUT.close();
}

void BPnetwork::loadBPnetwork(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open BPnetwork net input File\n");
		exit(0);
	}
	IN >> numHidden;
	for (unsigned short int i = 0; i <= numHidden + 1; i++)
	{
		unsigned short int acModel;
		IN >> dimLayer[i] >> acModel;
		acFun[i] = noActivation;
		if (acModel == 1)acFun[i] = Tanh;
		if (acModel == 2)acFun[i] = Sigmoid;
		if (acModel == 3)acFun[i] = ReLU;
		if (acModel == 4)acFun[i] = LeakyReLU;
		if (acModel == 5)acFun[i] = ELU;
		if (acModel == 6)acFun[i] = Softplus;
	}
	
	for (unsigned short int j = 0; j < dimLayer[0]; j++)
		IN >> xMean[j] >> xSD[j];
	for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
		IN >> yMean[k] >> ySD[k];

	for (unsigned short int i = 0; i <= numHidden; i++)
		for (unsigned short int j = 0; j <= dimLayer[i]; j++)
			for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
				IN >> w[i][j][k];
	IN.close();
}

void BPnetwork::forwardCal(float* X)
{
	for (unsigned short int j = 0; j < dimLayer[0]; j++)
		X[j] = (X[j]-xMean[j])/xSD[j];
	for (unsigned short int i = 0; i <= numHidden; i++)
	{
		for (unsigned short int j = 0; j < dimLayer[i]; j++)
			X[j] = activateNet(X[j], acFun[i]);
		X[dimLayer[i]] = float(1);
		float tmp[BP_MAXDIM_LAYER] = { 0.0 };
		for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
			for (unsigned short int j = 0; j <= dimLayer[i]; j++)
				tmp[k] += X[j] * w[i][j][k];
		for (unsigned short int k = 0; k < dimLayer[i + 1]; k++)
			X[k] = tmp[k];
	}
	for (unsigned short int k = 0; k < dimLayer[numHidden+1]; k++)
	{		
		X[k] = activateNet(X[k], acFun[numHidden+1]);
		X[k] = X[k]*ySD[k] + yMean[k];
	}
}

void BPnetwork::getResult(char* outputPath)
{
	std::ofstream OUT(outputPath, std::ios::out);
	for (unsigned short int t = 0; t < numSample; t++)
	{
		forwardCal(sampleX[t]);
		float error = 0.0;
		for (unsigned short int k = 0; k < dimLayer[numHidden + 1]; k++)
			error += (sampleX[t][k] - sampleY[t][k])*(sampleX[t][k] - sampleY[t][k]);
		error = sqrt(error);

		OUT << error << "\t";
		for (unsigned short int k = 0; k < dimLayer[numHidden + 1]; k++)
			OUT << sampleX[t][k] << "\t";
		for (unsigned short int k = 0; k < dimLayer[numHidden + 1]; k++)
			OUT << sampleY[t][k] << "\t";
		OUT << "\n";
	}
	OUT.close();
}