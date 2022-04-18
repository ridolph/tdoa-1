#include "Taylor.h"

void Taylor::inputAnchorPosition(char* inputPath)
{
	std::ifstream IN(inputPath, std::ios::in);
	if (!IN)
	{
		printf("Cannot Open TDOAdata input File\n");
		exit(0);
	}
	unsigned short int numAnchor, dimSpace;
	IN >> dimSpace;
	for (unsigned short int k = 0; k < dimSpace; k++)
		IN >> minPos[k] >> maxPos[k];
	IN >> numAnchor;
	for (unsigned short int i = 0; i < numAnchor; i++)
		for (unsigned short int k = 0; k < dimSpace; k++)
			IN >> anchorPos[i][k];
	IN.close();
}
void Taylor::getResult(char* inputPath, char* outputPath, unsigned short int maxITER, float maxError, bool OPT_INIT)
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
	for (unsigned short int s = 0; s < numSample; s++)
	{
		float d0[numAnchor], realPos[dimSpace], taylorPos[dimSpace];
		for(unsigned short int i=0; i<numAnchor; i++)
			IN >> d0[i];
		for(unsigned short int k=0; k<dimSpace; k++)
		    IN>>realPos[k];
		
		if (!OPT_INIT)
		{
			for (unsigned short int k = 0; k < dimSpace; k++)
				taylorPos[k] = (maxPos[k] - minPos[k])*rand() / float(RAND_MAX) + minPos[k];
		}
		else
		{
			float d10_2 = d0[0] * d0[0];
			float r10_2 = 0.0;
			for (unsigned short int k = 0; k < dimSpace; k++)
				r10_2 += (anchorPos[1][k] - anchorPos[0][k])*(anchorPos[1][k] - anchorPos[0][k]);
			float maxL = (r10_2 - d10_2) / (sqrt(d10_2) * 2);
			if (maxL > (maxPos[1] - minPos[1]))maxL = maxPos[1] - minPos[1];
			float L = maxL * rand() / float(RAND_MAX);
			float maxD = float(3.1415926536 / 2);
			if ((maxPos[2] - minPos[2]) < L)maxD = asin((maxPos[2] - minPos[2]) / L);
			float D = maxD * rand() / float(RAND_MAX);
			taylorPos[0] = (-d0[0] / 2)*sqrt(1 + L * L * 4 / (r10_2 - d10_2));
			taylorPos[1] = L * cos(D);
			taylorPos[2] = L * sin(D);
			for (unsigned short int k = 0; k < dimSpace; k++)
				taylorPos[k] += ((anchorPos[0][k] + anchorPos[1][k]) / 2);
		}		
		unsigned short int it;
		for(it=0; it<maxITER; it++)
		{
			float d[numAnchor+1]={0.0}, error=float(0);
			for(unsigned short int i=0; i<=numAnchor; i++)
			{
			    for(unsigned short int k=0; k<dimSpace; k++)
				    d[i]+=(taylorPos[k]-anchorPos[i][k])*(taylorPos[k]-anchorPos[i][k]);
				d[i]=sqrt(d[i]);
			}
			for(unsigned short int i=0; i<numAnchor; i++)
			    error+=(d0[i]-d[i+1]+d[0])*(d0[i]-d[i+1]+d[0]);
			error = sqrt(error);
			if(error<maxError)break;		

			float H[numAnchor], G[numAnchor][dimSpace];
			for(unsigned short int i=0; i<numAnchor; i++)
			{
				H[i]=d0[i]-(d[i+1]-d[0]);
			    for(unsigned short int k=0; k<dimSpace; k++)
                    G[i][k]=(taylorPos[k]-anchorPos[i+1][k])/d[i+1]-(taylorPos[k]-anchorPos[0][k])/d[0];
			}
			float GTG[dimSpace][dimSpace]={0.0}, invGTG[dimSpace][dimSpace];
			for(unsigned short int k1=0; k1<dimSpace; k1++)
				for(unsigned short int k2=0; k2<dimSpace; k2++)
					for(unsigned short int i=0; i<numAnchor; i++)
						GTG[k1][k2]+=G[i][k1]*G[i][k2];
			invMat((float*)GTG, dimSpace, (float*)invGTG);
			float invGTGxGT[dimSpace][numAnchor]={0.0};
			for(unsigned short int k=0; k<dimSpace; k++)
			    for(unsigned short int i=0; i<numAnchor; i++)
				    for(unsigned short int kk=0; kk<dimSpace; kk++)
					    invGTGxGT[k][i]+=invGTG[k][kk]*G[i][kk];
			float D[dimSpace]={0.0};
			for(unsigned short int k=0; k<dimSpace; k++)
			    for(unsigned short int i=0; i<numAnchor; i++)
				    D[k]+=invGTGxGT[k][i]*H[i];	
			for(unsigned short int k=0; k<dimSpace; k++)
			    taylorPos[k]+=D[k];
		}

		OUT << it << "\t";
		for(unsigned short int k=0; k<dimSpace; k++)
			OUT << taylorPos[k] << "\t";
		for(unsigned short int k=0; k<dimSpace; k++)
			OUT << realPos[k] << "\t";
		OUT << "\n";
	}
	OUT.close();
}

float Taylor::detMat(float* mat, unsigned short int n)
{
	float ans=0.0;
	if(n==1)ans=*mat;
	if(n==2)ans=*mat * *(mat+3)- *(mat+1) * *(mat+2);
	if(n==3)
	{
		for(unsigned short int i=0; i<n; i++)
		{
			float tmp1=1.0, tmp2=1.0;
			for(unsigned short int j=0; j<n; j++)
			{
				tmp1*=*(mat+j*n+(j+i)%n);
				tmp2*=*(mat+(n-1-j)*n+(j+i)%n);
			}
			ans=ans+tmp1-tmp2;
		}
	}
    return ans;
}
  
void Taylor::invMat(float* mat, unsigned short int n, float* ans)
{
	float detAns=detMat(mat,n);
    for(unsigned short int i=0; i<n; i++)
		for(unsigned short int j=0; j<n; j++)
        {
			float tmp[n-1][n-1];
            for(unsigned short int p=0; p<n-1; p++)
                for(unsigned short int q=0; q<n-1; q++)
                    tmp[p][q]=*(mat+(p>=i?p+1:p)*n+(q>=j?q+1:q));
			float aji=detMat((float*)tmp, n-1)/detAns;
            if((i+j)%2==1)aji=-aji;
            *(ans+j*n+i)=aji;
        }
}

void Taylor::test()
{
	float a[3][3];
	a[0][0]=1; a[0][1]=2; a[0][2]=3;
	a[1][0]=1; a[1][1]=0; a[1][2]=-1;
	a[2][0]=0; a[2][1]=1; a[2][2]=1;
	float b[3][3];
	invMat((float*)a,3,(float*)b);
	for(unsigned short int i=0;i<3;i++)
	{
		for(unsigned short int j=0;j<3;j++)
		    printf("%lf\t",b[i][j]);
		printf("\n");
	}
}