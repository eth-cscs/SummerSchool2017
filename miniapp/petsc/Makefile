# Makefile for use with Piz Daint, a Cray XC system.
#
# Also see Makefile.customPetsc to use your own build of PETSc on Piz Daint,
# or Makefile.local to use your own build locally
#
# Tested as of 2017.07.26
#
# Required modules:
#   PrgEnv-cray
#   cray-petsc

ALL: main

# We use the bash shell to allow for our checks below
SHELL=/bin/bash

OBJ = main.o system.o dump.o

main: $(OBJ)
	cc -dynamic -o $@ $(OBJ)

%.o : %.c
	cc -c -o $@ $<

clean :
	rm -f main $(OBJ) out.m output.*

# Some tests, to be run in an interactive session (see salloc)
test : run_test1 run_test2

# Some PETSc command line options
# Note that these are high-precision, so some variation
# is expected when running on other systems
TEST_OPTIONS=-ts_monitor -snes_monitor -ksp_monitor -ts_view

run_test1: ${EXNAME}
	@rm -f test1.tmp
	@echo "Running Test 1"
	@srun -n 1 ./main ${TEST_OPTIONS}  \
    2>&1 > test1.tmp
	@diff test1.tmp testref/test1.ref && \
    echo "Success" || \
    echo "Check: output does not match reference (see diff above)"
	@rm -f test1.tmp

run_test2: ${EXNAME}
	@rm -f test2.tmp
	@echo "Running Test 2"
	@srun -n 2 ./main ${TEST_OPTIONS}  \
    2>&1 > test2.tmp
	@diff  test2.tmp testref/test2.ref && \
    echo "Success" || \
    echo "Check: output does not match reference (see diff above)"
	@rm -f test2.tmp

.PHONY: ALL allclean test run_test1 run_test2
