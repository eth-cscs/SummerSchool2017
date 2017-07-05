.PHONY: all clean

CRAY ?= 1
PGI  ?= 0
GCC  ?= 0
VERBOSE ?= 0
CPPFLAGS ?= -DNO_NAIVE

override CPPFLAGS += -I../common

ifeq ($(CRAY), 1)
CXX = CC
FC = ftn
CXXFLAGS = -hstd=c++11 -O3 -homp -hacc -dynamic
FCFLAGS  = -O3 -homp -hacc -dynamic
LDFLAGS  = -lcublas
ifeq ($(VERBOSE), 1)
CXXFLAGS += -hmsgs
FCFLAGS  += -hmsgs
endif
endif

ifeq ($(PGI), 1)
CXX = pgc++
CXXFLAGS = -DPGI -std=c++11 -O3 -mp=allcores -acc -Minfo=accel -ta=tesla:cuda8.0
FCFLAGS  = -O3 -mp=allcores -acc -Minfo=accel -ta=tesla:cuda8.0
LDFLAGS  += -L$(CRAY_CUDATOOLKIT_DIR)/lib64 -lcudart -lcublas
endif

ifeq ($(GCC), 1)
CXX = g++
FC  = gfortran
override CPPFLAGS += -I$(CRAY_CUDATOOLKIT_DIR)/include
CXXFLAGS = -std=c++11 -O3 -fopenmp -fopenacc
FCFLAGS  = -O3 -fopenacc -fopenmp
LDFLAGS  += -L$(CRAY_CUDATOOLKIT_DIR)/lib64 -lcudart -lcublas
endif

OPENACC_BENCHMARKS= gemm.openacc

BENCHMARKS=$(OPENACC_BENCHMARKS)

all: $(BENCHMARKS)

util.o: util.f03
	$(FC) -c $(FCFLAGS) $<

gemm.openacc: gemm.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# axpy.openacc.fort: axpy_openacc.f90 util.o
#	$(FC) $(FCFLAGS) $< util.o -o $@ $(LDFLAGS)

clean:
	rm -f $(BENCHMARKS) *.cub *.ptx *.o *.mod
