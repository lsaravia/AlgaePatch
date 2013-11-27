//	Clase Base : CABase
//
//	Clase Asociada : Specie
//
//
//#pragma implementation

#include "IpsPatchStage.h"
#include "bgi.hpp"
//#include "fortify.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <time.h>

using namespace std;

IPSPatchStage::~IPSPatchStage()
	{
	if(Sp!=NULL)
		delete Sp;
	}

void IPSPatchStage::Init( unsigned numSp, int dimX, int dimY, int rr )
	{
	CABase::Init(numSp, dimX, dimY,rr);

	C.resize(dimX,dimY);

    GlobalRate= 0;
    NumEvaluations = 0;

	Sp	= new SpeciePatchStage[NumSpecies+1];

	}

void IPSPatchStage::Evaluate()
	{
	int x,y,i;
    static bool privez=true;
    if( privez )
    {
        double gr1=0;
        // Find the maximun rate 
        //
        for(i=1; i<=NumSpecies; i++)
            gr1 = (Sp[i].ExtinctionRate) > gr1 ? (Sp[i].ExtinctionRate) : gr1;

        for(i=1; i<=NumSpecies; i++)
            gr1 = (Sp[i].GrowthRate) > gr1 ? (Sp[i].GrowthRate) : gr1;

        for(i=1; i<=NumSpecies; i++)
            gr1 = (Sp[i].ColonizationRate+Sp[i].PerturbationRate) > gr1 ? (Sp[i].ColonizationRate+Sp[i].PerturbationRate) : gr1;

        for(i=1; i<=NumSpecies; i++)
            gr1 = (Sp[i].CompetitionRate) > gr1 ? (Sp[i].CompetitionRate) : gr1;

        GlobalRate = gr1;

        
        for(i=1; i<=NumSpecies; i++)
        {
            Sp[i].GrowthRate/=GlobalRate;
            Sp[i].ColonizationRate/=GlobalRate;
            Sp[i].ExtinctionRate/=GlobalRate;
            Sp[i].PerturbationRate/=GlobalRate;
            Sp[i].CompetitionRate/=GlobalRate;

            Sp[i].ColonizationRate+=Sp[i].PerturbationRate;  // Convert to transition rate

        }

    	NumEvaluations = DimX*DimY*GlobalRate;
        privez=false;
    }

	for(i=0; i<NumEvaluations; i++)
		{
		x = Rand(DimX-1);
		y = Rand(DimY-1);
		EvalCell(x,y);
		}
	T++;

	}

// Acordarse de empezar el vector de parametros de especies en 1 *************************
//
void IPSPatchStage::EvalCell(int x,int y)
{
    int actSp = C(x,y).Specie;
	int actSt = C(x,y).Stage;
	double rnd = Rand();
    int dx,dy,dd,dis,x1,y1;


	// Colonization from the exterior
	//

	if(actSp>0)
	{
        if(actSt==0)
        {
            if(rnd<Sp[actSp].ExtinctionRate)
                    C(x,y).Specie=0;
        }
        else
        {
            if(rnd<Sp[actSp].PerturbationRate)
                C(x,y).Stage--;
            else if(rnd<Sp[actSp].ColonizationRate)
            {
            //
            // Dispersal using Euclidean distance, Norm 2
            //
                ExpDispersal(Sp[actSp].DispersalDistance,x,y,x1,y1);
                int target = C(x1,y1).Specie;
                if( target == 0 )
                    C(x1,y1).Elem(actSp,0);
                else if(target == actSp) // If in the position of dispersal there is the same specie 
                {
                    if(C(x1,y1).Stage==0 )
                    {
                        rnd = Rand();
                        if(rnd<Sp[actSp].GrowthRate)
                            C(x1,y1).Stage=1;     // Pass to stage 1
                    }
                }
                else if(target>actSp)       // Specie 1 can replace 2 or 3, specie 2 can Replace 3, specie 3 can't replace any.
                {
                    if(C(x1,y1).Stage==0 )
                    {
                        rnd = Rand();
                        if(rnd<Sp[actSp].CompetitionRate)
                            C(x1,y1).Specie=actSp;     // Actual specie replace target specie
                    }

                }
            }
        }
	}
}





