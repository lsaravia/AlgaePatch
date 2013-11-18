//	Clase Base : CABase
//
//	Clase Asociada : Specie
//
//
//#pragma implementation

#include "IpsPatchStage.h"
#include "bgi.hpp"
#include "fortify.h"
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
    double gr1=0,gr2=0;
    if( privez )
    {
        for(i=0; i<NumSpecies; i++)
            gr1 = (Sp[i].GrowthRate+Sp[i].ExtinctionRate) > gr1 ? (Sp[i].GrowthRate+Sp[i].ExtinctionRate) : gr1;

        for(i=0; i<NumSpecies; i++)
            gr2 = (Sp[i].ColonizationRate+Sp[i].PerturbationRate) > gr2 ? (Sp[i].ColonizationRate+Sp[i].PerturbationRate) : gr2;

        GlobalRate = gr1 > gr2 ? gr1 : gr2;

        for(i=0; i<NumSpecies; i++)
        {
            Sp[i].GrowthRate/=GlobalRate;
            Sp[i].ColonizationRate/=GlobalRate;
            Sp[i].ExtinctionRate/=GlobalRate;
            Sp[i].PerturbationRate/=GlobalRate;

            Sp[i].GrowthRate+=Sp[i].ExtinctionRate;  // Convert to transition rate
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
    int dx,dy,dd,dis,x1,y1,i;


	// Colonization from the exterior
	//

	if(actSp>0)
	{
        If(actSt==0)
        {
            if(rnd<Sp[actSp].ExtinctionRate)
                    C(x,y).Specie=0;
            else if( rnd< Sp[actSp].GrowthRate) // Transition to stage 2
                    C(x,y).Stage++;

            // FALTA CONTACTO COMPETENCIA ENTRE SMALL PATCHES ESPECIES 
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
                dd=Sp[actSp].DispersalDistance;
        		dx= Rand(dd*2) - dd;
    			dis=sqrt( static_cast<double>( dd * dd - dx * dx ));
    			dy=Rand(dis*2) - dis ;
               	x1 = (x+ dx + DimX) % DimX;
            	y1 = (y+ dy + DimY) % DimY;
                if( C(x1,y1).Specie == 0 )
                    C(x1,y1).Elem(actSp,0);
            }
        }
	}
}





