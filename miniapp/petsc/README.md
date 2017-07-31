This is a version of the miniapp which uses PETSc. It is different in spirit from
the other versions of the miniapp, in that it is not intended to be an
essentially-identical piece of code with to-be-ported kernels, but is rather
an example of writing a functionally-similar code using a higher-level library.

For more information, see the notes in the .c files here

Quick start on Piz Daint (updated 2017.07.26)
---------------------------------------------

1. Log in, via ssh with X tunneling, to Piz Daint

    ssh -Y courseNN@ela.cscs.ch   # replace "courseNN" with your CSCS username
    ssh -Y daint

2. Load modules

    module list   # confirm that you have the PrgEnv-cray module loaded
    module load daint-gpu cray-petsc

3. Clone this repository and navigate to this directory (if needbe).

    cd $SCRATCH   # or choose another location
    git clone https://github.com/eth-cscs/SummerSchool2017
    cd SummerSchool2017/miniapp/petsc

3. Build the executable

    make

4. Test in an interactive session

    salloc -C gpu
    make test

 You should see

    Running Test 1
    Success
    Running Test 2
    Success

5. Run your own experiments in the interactive session

    srun -n 4 ./main -nx 99 -ny 88 -ts_monitor -snes_monitor -ksp_monitor
    srun -n 1 ./main -nx 16 -ny 16 -ts_monitor -snes_monitor -ksp_monitor -assemble 1 -pc_type gamg -dump 1


More Information
----------------

 You can also modify the included job.daint and submit with sbatch.

 To view the .bov file that is generated (only for single-processor runs with the -dump option), we borrow the procedure from the miniapp:
    srun ./main -dump
    ../../scripts/plot.sh
