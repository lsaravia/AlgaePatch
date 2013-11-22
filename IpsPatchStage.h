#ifndef __PatchStage_H
#define __PatchStage_H
//#include "fortify.h"
#include "cabase.hpp"
#include "smattpl.hpp"
#include <cmath>  /* for std::abs(double) */
#include "ran.h"

// Multiespecific Hierarchical 2 stages 
//

class CellPatchStage
{
	public:

	int Specie;
	int Stage;

	CellPatchStage() { Specie=0; Stage=0;};
	int & Elem() { return Specie; };

	const CellPatchStage & Elem( int nsp, int stg ) {
								Specie=nsp;
								Stage=stg;
								return *this; };

	CellPatchStage & operator=(const CellPatchStage &src){ Specie = src.Specie;
								Stage = src.Stage;
								return *this;};

	CellPatchStage(const CellPatchStage &src){ Specie = src.Specie; Stage=src.Stage; };

};


class SpeciePatchStage
{
	public:
	float GrowthRate;					// From small patch to big patch
	float ColonizationRate;				// Bigs produce colonizers
	float ExtinctionRate;				// Small patch extinction
	float PerturbationRate;				// Big patches to small
	float CompetitionRate;				// Big patches can displace small patchs of other species with greater index
	unsigned DispersalDistance;

	SpeciePatchStage() { Init(); };
	virtual void Init(){
						GrowthRate=0;
						ExtinctionRate=0;
						DispersalDistance=0;
						ColonizationRate=0;
						PerturbationRate=0;
						CompetitionRate=0;
						};

	virtual int Scan(char * buff);

//	float PDetachment;
//	unsigned DetachmentTreshold;
//	unsigned DetachmentDistance;
//	float MaxBiomass;
};


class IPSPatchStage : public CABase
{
	protected:
		SpeciePatchStage * Sp;
		simplmat <CellPatchStage> C;
//		int ActualSp;
		CellPatchStage ActualCell ;
        double GlobalRate;
        int NumEvaluations;
        bool PrimeraEval; 
        Ranf1 ran;
	public:

	IPSPatchStage( unsigned numSp, int dimX, int dimY, int rr=0 ) : CABase(numSp,dimX,dimY,rr) {Init(numSp,dimX,dimY,rr);};
	IPSPatchStage(){};
	~IPSPatchStage();
	void Init(unsigned numSp, int dimX, int dimY, int rr=0 );
	int  ReadParms( long rndSeed, char * file );

	void ReadSetSeed( char * fname);
	void RandomSetSeed(int sp,unsigned age, int no, int minX);
	int Rand(int num);
	double Rand();

	void Evaluate();
	void EvalCell(int x,int y);
	void Reset();

	void PrintGraph();
   	void InitGraph(char * idrPal);
   	void InitGraph(){};
	int  PrintDensity(char *fname=NULL,char *iname=NULL);

	int ReadSeed( char * fname, int mode);
	int ReadSeed( char * fname){return ReadSeed( fname,0 );};

	int SaveSeed( const char * fname);

	int Convert(simplmat <double> &data);                                      // Convierte a simplmat
	
	int MFStats(simplmat <double> &data, simplmat <double> &q,
						int minBox, int maxBox, int deltaBox,const char * outFile,const char * ident);
									
};

inline double IPSPatchStage::Rand() { 
	return ran.doub(); 
	};

inline int IPSPatchStage::Rand(int num) {
		return (ran.int64() % (num+1)); // between 0 and num inclusive 
		//return ignuin(0,num);
        };

#endif // __PatchStage_H
