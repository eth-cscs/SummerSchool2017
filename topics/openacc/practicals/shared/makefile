.PHONY: all clean

VERBOSE ?= 0

override CPPFLAGS += -I../common

ifeq ($(PE_ENV), CRAY)
	CXX = CC
	FC  = ftn
	CXXFLAGS = -hstd=c++11 -O3 -hacc -dynamic
	FCFLAGS  = -O3 -homp -hacc -dynamic
	ifeq ($(VERBOSE), 1)
		CXXFLAGS += -hmsgs
		FCFLAGS  += -hmsgs
	endif
endif

ifeq ($(PE_ENV), PGI)
	CXX = pgc++
	FC  = pgf90
	CXXFLAGS = -DPGI -std=c++11 -O3 -acc -Minfo=accel -ta=tesla:cc60
	FCFLAGS  = -O3 -acc -Minfo=accel -ta=tesla:cc60
	LDFLAGS += -L$(CRAY_CUDATOOLKIT_DIR)/lib64 -lcudart -ta=tesla:cc60
endif

ifeq ($(PE_ENV), GNU)
	CXX = g++
	FC  = gfortran
	CXXFLAGS = -std=c++11 -O3 -fopenmp -fopenacc -I$(CRAY_CUDATOOLKIT_DIR)/include
	FCFLAGS  = -O3 -fopenacc -fopenmp
	LDFLAGS += -L$(CRAY_CUDATOOLKIT_DIR)/lib64 -lcudart
endif

OPENACC_BENCHMARKS= blur.openacc blur.openacc.fort dot.openacc dot.openacc.fort
BENCHMARKS=$(OPENACC_BENCHMARKS)

all: $(BENCHMARKS)

util.o: ../common/util.f03
	$(FC) $(CPPFLAGS) -c $(FCFLAGS) $<

blur.openacc: blur_openacc.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

blur.openacc.fort: blur_openacc.f90 util.o
	$(FC) $(FCFLAGS) $< util.o -o $@

dot.openacc: dot_openacc.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

dot.openacc.fort: dot_openacc.f90 util.o
	$(FC) $(CPPFLAGS) $(FCFLAGS) $< util.o -o $@

clean:
	rm -f $(BENCHMARKS) *.cub *.ptx *.o *.mod
