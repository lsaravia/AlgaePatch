//
// Funciones Auxiliares
//
//#pragma implementation

#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "IpsMultiContact.h"
#include "fortify.h"
#include <ctype.h>
#ifdef __GNUG__
extern int getch(void);
extern int getkey(void);
extern int kbhit(void);
#else
#include <conio.h>
#include <pc.h>
#endif
#include <time.h>
#include "bgi.hpp"

#define uchar unsigned char

using namespace std;

int SpecieMultiContact::Scan(char * buff)
	{
	int sp=0;
	istringstream ins(buff);
	ins >> sp >> BirthRate
				>> MortalityRate
				>> DispersalDistance
				>> ColonizationRate ;
//			  >> PDetachment >> DetachmentDistance >> DetachmentTreshold
//			  >> DispersalNorm;
	return sp;
	}


void IPSMultiContact::Reset()
	{
	C.fill( ActualCell.Elem(0) );
	A = 0;
	N = 1;
	T = 0;
//	ActualSp = 0;
	}

int IPSMultiContact::ReadParms( long rndSeed, char * file )
	{
	ifstream in;
	char buff[255];

	int sp=0;


	in.open(file);
	if( !in )
		{
		cerr << "Cannot open Parms file.\n";
		return 1;
		}

	in >> DimX >> DimY;

	in >> NumSpecies;

	Init( NumSpecies, DimX, DimY, rndSeed);

	in.getline(buff,255);

	for(int i=0;i<NumSpecies;i++)
		{
		in.getline(buff,254);
		sp = Sp[i].Scan(buff);
		if( sp>=NumSpecies || sp!=i )
			{
			cerr << "Error reading parameter file.\n";
			return 1;
			}

		}
	return 0;
	}


void IPSMultiContact::ReadSetSeed(char * file)
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

		if( sscanf(buff,"%i %i %i %i",&sp,&age,&cant,&minx) != 4 || sp>=NumSpecies )
			break;
		RandomSetSeed(sp,age,cant,minx);
		}
	while( fgets(buff,80,in)!=NULL );
	fclose(in);

	}

//
//	mode == 1 -> Lee sin modelo
//	
int IPSMultiContact::ReadSeed(char * fname,int mode)
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

	while( fgets(buff,4,in)!= NULL )
		{
		if( strncmp(buff,"SP",2)==0 )
			tipo = 0;
		else if(strncmp(buff,"AG",2)==0 )
			tipo = 1;
		else if(strncmp(buff,"BI",2)==0 )
			tipo = 3;
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
//					case 0:
//						C(dx,dy).Elem(A) = spe;
//						break;
//					case 1:
//						C(dx,dy).Age = spe;
//						break;
					case 3:
						C(dx,dy).Elem() = spe;
						if( spe>maxSpe )
							maxSpe = spe;
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


int IPSMultiContact::SaveSeed(const char * fname)
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

void IPSMultiContact::InitGraph(char * idrPal)
{
	IGraph(DimX,DimY,idrPal);
	char buff[20];

	for( int i=0; i<=NumSpecies; i++)
	{
		sprintf(buff,"%d",i);
		GLabel(buff,i);
	}
	
//  	BLabel();
}

void IPSMultiContact::PrintGraph()
	{
	char sa, baseName[9];
	ostringstream name;
	int sp=0;
    static bool privez=true;
    
	for(int i=0; i<DimX; i++)
		{
		for(int j=0;j<DimY;j++)
			{
			sp = C(i,j).Elem();
			PPix(i,j,sp);
			}
		}
    if( privez )
    {
    	getchar();
        privez = false;
    }
    
/*	if( kbhit() )
		{
		sa=toupper(getch());
		if( sa=='S')
			{
			EndGraph();
			cerr << "Tiempo :" << T << endl;
			cerr << "Ingrese nombre BASE de archivo : ";
			cin.width(6);
			cin >> baseName;
			name << baseName << T << ".sed" << ends;
			SaveSeed( name.str().c_str() );
			InitGraph();
			}
		else
			getch();
		}
	//getch();
	*/
	 
	}

void IPSMultiContact::RandomSetSeed(int sp,unsigned age, int no, int minX)
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
			if( C(rx,ry).Elem() == 0 )
				{
				C(rx,ry).Elem() = sp+1;
//				C(rx,ry).Age = age;
				break;
				}
			}
		}
	}


int  IPSMultiContact::PrintDensity(char *fname,char *iname)
{
	fstream dout;
	static int privez=0;
	double tot=0,totBio=0,totCells=DimX*DimY;
	double * den = new double[NumSpecies];
	int a,i;

	if( fname!=NULL )
	{
		ostringstream name;
		name << fname << "Density.txt" << ends;
		dout.open( name.str().c_str(), ios::in );
		if( !dout )
			privez=1;

		dout.close();
        dout.clear();
		dout.open( name.str().c_str(), ios::app );
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
		privez=0;
		dout << "Class";
		for( a=0; a<NumSpecies; a++)
			{
			//dout.width(6);
			dout <<  "\t" << (a+1);
			}
		dout << "\tTot.Dens\tTot.Num" << endl;
		}

	for(i=0; i<NumSpecies; i++)
		den[i]=0;

	for(i=0; i<DimY; i++)
		for(int j=0;j<DimX;j++)
			{
			a = C(j,i).Elem();
			if( a>0 )
				den[ a-1 ]++;
			}
	if( iname==NULL )
		dout << T ;
	else
		dout << iname << "\t" << T ;

	for( i=0; i<NumSpecies; i++)
		{
		dout << "\t" << den[i]/totCells;
		tot+= den[i];
		totBio+= den[i]/totCells;
		}
	dout << "\t" << totBio <<  "\t" << tot << endl;

	delete []den;
	dout.close();
	return tot;
	};

int IPSMultiContact::ReadIdrisi( char * fname, int mode )
	{
	ostringstream dname,iname;
	dname << fname << ".rdc" << ends;
	iname << fname << ".rst" << ends;

	char buff[255],type[10], *ptr;
	ifstream in;
	int cols=0,rows=0,dx,dy;

	in.open(dname.str().c_str());
	if( !in )
		{
		fprintf(stderr, "Cannot open doc file.\n");
		return 1;
		}
	while( !in.eof() )
		{
		in.read(buff,255);
		//in.width(20);
		//in >> buff
		ptr =strstr(buff,"columns");
		if( ptr!=NULL )
			cols = atoi(ptr+13);

		ptr=strstr(buff,"rows");
		if( ptr!=NULL )
			rows = atoi(ptr+13);

		ptr=strstr(buff,"data type");
		if( ptr!=NULL )
			{
			strncpy(type,ptr+14,10);
			type[9]='\0';
			}
		}

	in.close();
	if( mode == 0 )
		{
		if( cols!=DimX || rows!=DimY)
			return 1;
		}
	else
		{
		C.resize(cols,rows);
		DimX=cols;
		DimY=rows;
		N = A;
		}

	int spe=0,maxSpe=0;
	uchar echa=0;

#ifdef STREAMIO
	in.open(iname.str(), ios::binary | ios::in );

	if( !in )
		{
		cerr << "Cannot open img file.\n";
		return 1;
		}

	if( strstr( type,"integer")!=NULL || strstr( type,"byte")!=NULL)
		{
		for(dy=0;dy<DimY; dy++)
			for(dx=0;dx<DimX; dx++)
			{
			in.read(&echa,1);
			spe = echa;
			if( spe>maxSpe )
					maxSpe = spe;
			C(dx,dy).Elem(A) = spe;
			}
		}
	else
		{
		cerr << "Cannot read this file type\n";
		return 1;
		}
#else
	FILE * inn;
	if ((inn = fopen(iname.str().c_str(), "rb")) == NULL)
		{
		fprintf(stderr, "Cannot img file.\n");
		exit(1);
		}
	if( strstr( type,"integer")!=NULL || strstr( type,"byte")!=NULL)
		{
		for(dy=0;dy<DimY; dy++)
			for(dx=0;dx<DimX; dx++)
			{
			fread(&echa,1,1,inn);
			spe = echa;
			if( spe>maxSpe )
					maxSpe = spe;
			C(dx,dy).Elem() = spe;
			}
		}
	else
		{
		cerr << "Cannot read this file type\n";
		return 1;
		}


#endif

	NumSpecies = maxSpe;
	return 0;
	}


