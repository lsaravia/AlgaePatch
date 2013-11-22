# This file is automatically generated by RHIDE Version 1.4.7
# created from within RHIDE
vpath_src=.. ../../randlib/src ../../fortify ../../SpatialAnalysis ../../SpatialAnalysis/mfsba
vpath %.c $(vpath_src)
vpath %.cpp $(vpath_src)
vpath %.hpp $(vpath_src)
vpath %.h   $(vpath_src)

# The X11 base dir on your system
X11BASE=/usr/X11R6
# Add directories with X11 include files here
X11INCS=-I$(X11BASE)/include
# put X11 required libraries and directories here
X11LIBS=-L$(X11BASE)/lib -lX11

SDLDEFS = -D__XWIN__

I_DIRS=-I.. -I../../fortify -I../../SpatialAnalysis/mfsba -I../../SpatialAnalysis

P_DEFS=-DGRAPHICS -DPERIODIC_BOUNDARY

#CXXFLAGS = -O2 -g -Wall $(I_DIRS) $(X11INCS)  $(SDLDEFS) $(P_DEFS)
CXXFLAGS = -g -Wall $(I_DIRS) $(X11INCS)  $(SDLDEFS) $(P_DEFS)

L = -lm -lgrx20S -lSDL $(X11LIBS)


O=bgi.o cabase.o IpsPatchStage.o mfSBA.o RWFile.o\
	IpsPatchStageAux.o IpsPatchStageStats.o MainIPS.o 


MAIN_TARGET=ipsPatch
all: $(O)
	g++ -o $(MAIN_TARGET) $(O) $(L)

clean:
	rm $(MAIN_TARGET) *.o 

	
all:

cabase.o: cabase.cpp cabase.hpp makefile

bgi.o: bgi.cpp makefile

IpsPatchStage.o: IpsPatchStage.cpp makefile IpsPatchStage.h ran.hpp

IpsPatchStageAux.o: IpsPatchStageAux.cpp makefile IpsPatchStage.h

MainIPS.o: MainIPS.cpp makefile IpsPatchStage.h

IpsPatchStageStats.o: IpsPatchStageStats.cpp makefile IpsPatchStage.h

mfSBA.o: mfSBA.cpp makefile 



RWFile.o: RWFile.cpp makefile 
