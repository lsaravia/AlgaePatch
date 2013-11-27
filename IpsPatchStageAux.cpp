//
// Funciones Auxiliares
//
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctype.h>
#include <time.h>
#include "grxkeys.h"
#include "bgi.hpp"
#include "RWFile.h"
#include "IpsPatchStage.h"


#define uchar unsigned char

using namespace std;

int SpeciePatchStage::Scan(char * buff)
	{
	int sp=0;
	istringstream ins(buff);
	ins >> sp >> GrowthRate >>			
				ColonizationRate >>		
				ExtinctionRate >>		
				PerturbationRate >>		
				CompetitionRate >>		
				DispersalDistance;
	return sp;
	}


void IPSPatchStage::Reset()
	{
	C.fill( ActualCell.Elem(0,0) );
	A = 0;
	N = 1;
	T = 0;
	PrimeraEval=true;
	}

int IPSPatchStage::ReadParms( long rndSeed, char * file )
	{
	ifstream in;
	char buff[255];

	int sp=0;


	in.open(file);
	if( !in )
		{
		cerr << "Cannot open Parms file.\n";
		exit(1);
		}

	in >> DimX >> DimY;

	in >> NumSpecies;

	Init( NumSpecies, DimX, DimY, rndSeed);

	in.getline(buff,255);

	// Read the species parameters starting at 1 
	
	for(int i=1;i<=NumSpecies;i++)
		{
		in.getline(buff,254);
		sp = Sp[i].Scan(buff);
		if( sp>NumSpecies || sp!=i )
			{
			cerr << "Error reading species (inp) parameter file.\n";
			return 1;
			}
		}

	return 0;
	}


void IPSPatchStage::ReadSetSeed(char * file)
	{
	FILE *in;
	char buff[255];

	int sp=0,age=0,cant=0,minx=0;


	if ((in = fopen(file, "rt")) == NULL)
		{
		fprintf(stderr, "Cannot open Parms file.\n");
		exit(1);
		}

	fgets(buff,80,in);
	do
		{

		if( sscanf(buff,"%i %i %i %i",&sp,&age,&cant,&minx) != 4 || sp>NumSpecies )
			break;
		RandomSetSeed(sp,age,cant,minx);
		}
	while( fgets(buff,80,in)!=NULL );
	fclose(in);

	}

//
//	mode == 1 -> Lee sin modelo
//	
int IPSPatchStage::ReadSeed(char * fname,int mode)
	{
	FILE *in;
	char buff[256];
	int dx,dy;
	int spe,tipo,maxSpe=0;

	if ((in = fopen(fname, "rt")) == NULL)
		{
		fprintf(stderr, "Cannot open Seed file.\n");
		return 1;
		}
	fgets(buff,255,in);
	sscanf(buff,"%d %d",&dx,&dy);
	if( mode == 0 )
		{
		if( dx!=DimX || dy!=DimY )
			{
			fprintf(stderr, "Wrong dimension for Seed file.\n");
			exit(1);
			}
		}
	else
		{
		C.resize(dx,dy);
		DimX=dx;
		DimY=dy;
		N = A;
		Sp = NULL;
		}

	// If type SP or BI read species
	// if type ST read stages
	//
	while( fgets(buff,4,in)!= NULL )
		{
		if( strncmp(buff,"SP",2)==0 )
			tipo = 0;
		else if(strncmp(buff,"ST",2)==0 )
			tipo = 1;
		else if(strncmp(buff,"BI",2)==0 )
			tipo = 0;
		else
			continue;

		for(dy=0;dy<DimY; dy++)
			for(dx=0;dx<DimX; dx++)
				{
				int ret = fscanf(in,"%i",&spe);
				if( ret == 0 || ret ==EOF )
					{
					cerr << "Seed File invalid.\n";
					exit(1);
					}

				switch (tipo) {
					case 0:
						C(dx,dy).Specie = spe;
						if( spe>maxSpe )
							maxSpe = spe;
						break;
					case 1:
						C(dx,dy).Stage = spe;
						break;

					default:
						cerr << "Seed File invalid.\n";
						exit(1);
					}

				}
		if( mode==0 )
			{
			if( maxSpe>NumSpecies )
				{
				fprintf(stderr, "Wrong Seed file.\n");
				exit(1);
				}
			}
		else
			{
			NumSpecies=maxSpe;
			}

		}

	fclose(in);
	return 0;

	}


int IPSPatchStage::SaveSeed(const char * fname)
	{
	int i,j;
	ofstream sav( fname );
	if(!sav)
		{
		cerr << "Cannot open Seed file.\n";
		return 1;
		}

	sav << DimX << "\t" << DimY << endl;
	sav << "BI" << endl;
	for(i=0; i<DimY; i++)
		{
		for(j=0;j<DimX;j++)
			{
			sav<< setw(3) << int(C(j,i).Elem());
			}
		sav << endl;
		}
	sav << endl;

	return 0;
	}

void IPSPatchStage::InitGraph(char * idrPal)
{
	IGraph(DimX,DimY,idrPal);
	char buff[20];

	for( int i=1; i<=NumSpecies; i++)
	{
		int di= (i*2)-1;
		sprintf(buff,"%dS0",i);
		GLabel(buff,di);
		sprintf(buff,"%dS1",i);
		GLabel(buff,di+1);
		if(i>5) break;
	}
	
//  	BLabel();
}

void IPSPatchStage::PrintGraph()
{
	ostringstream name;
	int sp=0,st=0,di=0;
    
	for(int i=0; i<DimX; i++)
	{
		for(int j=0;j<DimY;j++)
		{

			sp = C(i,j).Specie;
			if(sp==0) di=0;
			else
			{
				st = C(i,j).Stage;
				di = (sp*2)-1+st;
			}
			PPix(i,j,di);
		}
	}

	if( GrKeyPressed() )
		{
		int sa=toupper(GrKeyRead());
		if( sa=='S')
			{
			EndGraph();
			cerr << "Tiempo :" << T << endl;
			cerr << "Ingrese nombre BASE de archivo : ";
//			cin.width(6);
//			cin >> baseName;
			name << "ipsPatch" << T << ".sed" << ends;
			SaveSeed( name.str().c_str() );
			InitGraph();
			}
		else if( sa=='Q')
			exit(1);
			
		}
}


// Distribute species at random. Species index start in 1
// 
void IPSPatchStage::RandomSetSeed(int sp,unsigned stage, int no, int minX)
	{
	int rx,ry;
	int i;
	for( i=0; i<no; i++)
		{
		while( 1 )
			{
			if( minX>0 )
				rx=Rand(minX);
			else
				rx=Rand(DimX-1);
			ry=Rand(DimY-1);
			if( C(rx,ry).Specie == 0 )
				{
				C(rx,ry).Specie = sp;
				C(rx,ry).Stage = stage;
				break;
				}
			}
		}
	}


int  IPSPatchStage::PrintDensity(char *fname,char *iname)
{
	fstream dout;
	static bool privez=false;
	double tot=0.0,totBio=0.0,totCells=DimX*DimY,freq=0.0;
	simplmat <double> den(NumSpecies+1);
	simplmat <double> stg(NumSpecies+1);
	unsigned a,i;

	if( fname!=NULL )
	{
		ostringstream name;
		name << fname << "Density.txt" << ends;
		dout.open( name.str().c_str(), ios::in );
		if( !dout )
			privez=true;

		dout.close();
        dout.clear();
		dout.open( name.str().c_str(), ios::out | ios::app );
		if( !dout )
		{
			cerr << "Cannot open density file.\n";
			return 0;
		}
	}
	else
	{
		cerr << "File name cannot be NULL\n";
		return 0;		
	}
	
	if( privez )
	{
		privez=false;

		dout << iname <<"\tSpecie";

		for( a=1; a<=NumSpecies; a++)
			{
			//dout.width(6);
			dout <<  "\t" << a;
			}
		dout << "\tTot.Dens\tTot.Num" ;


		// Proportion of stages (big/total patches)
		for( a=1; a<=NumSpecies; a++)
			{
			//dout.width(6);
			dout <<  "\t" << a;
			}

		dout << "\tStage.Big" << endl;
		}
	den.fill(0.0);
	stg.fill(0.0);


	for(i=0; i<DimY; i++)
		for(int j=0;j<DimX;j++)
		{
			a = C(j,i).Specie;
			if( a>0 )
			{
				den(a)++;
				if(C(j,i).Stage>0)		// Sum big patches = stage 1
					stg(a)++;
			}
		}
	if( iname==NULL )
		dout << T ;
	else
		dout << iname << "\t" << T ;


	for( i=1; i<=NumSpecies; i++)
		{
		freq = den(i)/totCells;
		dout << "\t" << freq;
		tot+= den(i);
		totBio+= freq;
		}
	dout << "\t" << totBio <<  "\t" << tot;


	totBio = 0;
	for( i=1; i<=NumSpecies; i++)
		{
		freq = stg(i)/den(i);
		dout << "\t" << freq;
		totBio+= stg(i);
		}
	totBio = totBio/tot;
	dout << "\t" << totBio <<  endl;
	
	dout.close();
	
	return tot;
	};

