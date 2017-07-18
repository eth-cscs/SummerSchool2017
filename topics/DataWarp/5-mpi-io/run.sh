tmp=$SCRATCH/dwtest
if [ ! -d $tmp ]
then
    mkdir $tmp
fi
cp a.out $tmp
cd $tmp

sbatch -C gpu <<\EOF
#!/bin/bash
#SBATCH --job-name="DW-MPI/IO"
#SBATCH --nodes=3
#SBATCH --time=0:05:00
#SBATCH --output=mpi-io.out%j
#DW jobdw access_mode=striped capacity=200GiB type=scratch
#-----------------------------------------------------------

export MPICH_MPIIO_HINTS="*out.dat:cb_nodes=8"

echo "================================================"
srun -N 3 ./a.out
echo "================================================"
cat -v $DW_JOB_STRIPED/out.dat

EOF
