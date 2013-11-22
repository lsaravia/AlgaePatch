#include "IpsPatchStage.h"
#include "mf.h"

using namespace std;

int IPSPatchStage::Convert(simplmat <double> &data)
{
	int dx,dy;
	dx = data.getRows();
	dy = data.getCols();

	if( dx!=DimX || dy!=DimY )
		data.resize(DimX, DimY, 0.0);
		
	for(dy=0;dy<DimY; dy++)
		for(dx=0;dx<DimX; dx++)
			data(dx,dy) = C(dx,dy).Specie;

	return 1;
}


int IPSPatchStage::MFStats(simplmat <double> &data, simplmat <double> &q,
	int minBox, int maxBox, int deltaBox,const char * outFile,const char * ident)
{
	return 	MultifractalSBA(data, q,const_cast<char *>(outFile) ,minBox, maxBox, deltaBox, 'S',const_cast<char *>(ident));
}



