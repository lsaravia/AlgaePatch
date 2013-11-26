//
// CADis : CA Discreto con bordes continuos
//

#include <ctype.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include "IpsPatchStage.h"
#include "bgi.hpp"
#include "RWFile.h"

using namespace std;


struct IPSParms
{
	unsigned long nRuns;
	unsigned long nEvals;
	long rndSeed;
	char gr;
	int grBio;
	unsigned long inter;
	unsigned long init;
	char de;
	char xy;
	char sa;
	char baseName[40];
	char idrPal[40];
	char mfDim;
	float minBox;
	float maxBox;
	float deltaBox;
	char patchStat;
	char moranI;
};

int ReadParms(char * pFile, IPSParms &p);

int main(int argc, char * argv[])
{
	IPSParms p;
	
	IPSPatchStage ca;

	if( argc > 1 )
	{
		ReadParms( argv[1], p );

		ca.ReadParms( p.rndSeed, argv[2] );

		if(argc >3 )
		{
			if( strstr(argv[3],"sed")!=NULL )
				ca.ReadSeed(argv[3]);	// Lee mapa ASCII de las especies y edades
			else if( strstr(argv[3],"set")!=NULL )
				ca.ReadSetSeed(argv[3]);
		}

	}
	else
	{
		cerr << "Parameter files missing! (xx.par yy.inp zz.sed)" << endl;
		exit(1);
	}

	int privez=1;

	for(int r=0; r<p.nRuns; r++)
	{
		if( privez )
			privez = 0;
		else
		{
			ca.Reset();
			if( argc>3 )
			{
			if( strstr(argv[3],"sed")!=NULL )
				ca.ReadSeed(argv[3]);	// Lee mapa ASCII de las especies y edades
			else if( strstr(argv[3],"set")!=NULL )
				ca.ReadSetSeed(argv[3]);
			}
		}

		if( p.gr=='S' )
        {
			ca.InitGraph(p.idrPal);
//			ReadIdrisiPalette(p.idrPal);
        }

		int i;
        for(i=0; i<p.nEvals; i++)
        {
            if( i == 0 )
            {
       	        if( p.gr=='S' )
                	ca.PrintGraph();
                else
   	                cerr << "Initial Conditions \n";
                if( p.de=='S')
                    ca.PrintDensity( p.baseName, argv[1] );
            }
			ca.Evaluate();
			if( ((i+1) % p.inter)==0 || i==0 )
            {
       	        if( p.gr=='S' )
                	ca.PrintGraph();
                else
					cerr << "Time " << (i+1) << endl;
				
				if( (i+1)>=p.init)
				{	
					if( p.de=='S' )
	                    if( ca.PrintDensity( p.baseName, argv[1] )== 0 )
	                    	break;
	                    	
					if( p.sa=='S')
					{
						ostringstream name;
						name << p.baseName  << "-" << (i+1) << ".sed" << ends;
						ca.SaveSeed( name.str().c_str() );
					}
					if( p.mfDim=='S')
					{
						ostringstream name,nam1;
						name << p.baseName << "mf.txt" << ends;
						nam1 << argv[1]  << "-" << (i+1) << ends;
						simplmat <double> dat;
						simplmat <double> q(1);
						q(0) = 1;
						ca.Convert(dat);
						ca.MFStats(dat,q,p.minBox,p.maxBox,p.deltaBox,name.str().c_str(),nam1.str().c_str());
					}

				}
            }
        }
        if( p.gr=='S' )
        	EGraph();
	}
	return 0;
}


int ReadParms(char * pFile, IPSParms &p)
{
	string buff;
	ifstream parms(pFile);
    if( !parms )
    	{
    	cerr << "Can't open parms file" << endl;
        exit(1);
        }
	parms >> buff;
	while( !parms.eof() )
	{
		if(buff=="nRuns") // numero de Corridas
    	{
    		parms >> p.nRuns;
    	}
    	else if(buff=="nEvals") // numero de evaluaciones (max time)
    	{
    		parms >> p.nEvals;
    	}
    	else if(buff=="rndSeed") // random number seed
    	{
    		parms >> p.rndSeed;
    	}
    	else if(buff=="gr") // Graficos? N/S
    	{
    		parms >> p.gr;
    		p.gr = toupper( p.gr );
    	}
    	else if(buff=="grBio") // Grafica: 0 = Species, 1=Biomasa
    	{
    		parms >> p.grBio;
    	}
	    else if(buff=="inter") // Intervalo de Graficacion/Muestreo
	    {
    		parms >> p.inter;
	    }
	    else if(buff=="init") // tiempo de Inicio de muestreo
	    {
    		parms >> p.init;
	    }
	    else if(buff=="de") // Calcula densidad por especie S/N
	    {
    		parms >> p.de;
			p.de = toupper( p.de );
	    }
	    else if(buff=="sa") // Salva seed en cada muestreo
	    {
    		parms >> p.sa;
			p.sa = toupper( p.sa );
	    }
	    else if(buff=="baseName") // Salva seed en cada muestreo
	    {
    		parms >> p.baseName;
	    }
	    else if(buff=="idrPal") // utiliza una palette de Idrisi
	    {
    		parms >> p.idrPal;
	    }
   	    else if(buff=="mfDim") // Calcula generalized dimension S/N
	    {
    		parms >> p.mfDim;
			p.mfDim = toupper( p.mfDim );
		}
   	    else if(buff=="minBox") // q inicial para genDim
	    {
    		parms >> p.minBox;
		}
   	    else if(buff=="maxBox") // q final para genDim
	    {
    		parms >> p.maxBox;
		}
   	    else if(buff=="deltaBox") // Delta q para genDim
	    {
    		parms >> p.deltaBox;
		}
		else if( !buff.empty() )
		{
			cerr << "Error in input file, unrecognized token" << endl;
			exit(1);
		}
		parms >> buff;
    }
    return 1;
}
