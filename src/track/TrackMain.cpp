#include "PathTracking.h"
#include "TDOAdata.h"

PathTracking trackTest;
TDOAdata tdoaTest;

int main()
{
	srand((unsigned)time(NULL));
	
	char groupPath[]= "data\\0group.txt";
	char bpPath[]	= "data\\0bp-10-0.txt";
	char envPath[]	= "data\\0env.txt";
	char trackPath[]= "data\\1track.txt";
	char realPath[]	= "data\\2real.txt";
	char locaPath[]	= "data\\3loca.txt";
	tdoaTest.generatePath(envPath, trackPath, Line1D, 200, float(0.6));
	trackTest.trakingSim(trackPath, groupPath, bpPath, realPath, locaPath);

	return 0;
}