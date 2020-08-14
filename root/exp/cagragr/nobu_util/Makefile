TARGETS   = gadd gh_to_th

CXX       = g++
CXXFLAGS  = $(shell root-config --cflags)
LFLAGS    = 
ROOTLIBS  = $(shell root-config --libs)
INCLUDE   = -I$(HOME)/mnt/GRUTinizer/include
GRUTLIBS  = -L$(HOME)/mnt/GRUTinizer/lib -lAllGrutinizer -lGROOT -lTKinematics -lTNucleus -lTGEBFormat -lTHistogramming -lTGRUTUtil -lTGRUTint -lSourceData -lTLenda -lTS800 -lTS800Sim -lTGretina -lTCaesar -lTFastScint -lTJanus -lTDetector -lTSega -lTCagra -lTGrandRaiden -lTPhosWall -lTNSCLScalers -lTGretSim -lTSRIM -lTReaction -lTRawFormat -lTNSCLFormat -lTLoops


all:	$(TARGETS)
gadd: gadd.o
	$(CXX)     $(LFLAGS) -o $@ $^ $(ROOTLIBS) $(GRUTLIBS)
gh_to_th: gh_to_th.o
	$(CXX)     $(LFLAGS) -o $@ $^ $(ROOTLIBS) $(GRUTLIBS)
%.o: %.cxx
	$(CXX)     $(INCLUDE) $(CXXFLAGS) -c $<
.PHONY : clean
clean:
	rm -rf *.o *~ $(TARGETS)
