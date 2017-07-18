
# Things to adjust before testing:
# Number of client nodes (here below)
# Number of datawarp nodes changing the capacity entry in the #DW directive (see table below)
# I/O blocksize (search below for bs=16M
nodes=16
tag=4dw

wd=$SCRATCH/dwtestdaint

if [ ! -d $wd ]
then
    mkdir $wd
fi

rm -rf $wd/out-vdbench-scratch $wd/out-vdbench-dw $wd/testdir-vdbench $wd/vdbench
cp -r vdbench $wd

cp multi-host-params $wd/tmpparams

cd $wd

# Sizes
# 1node:   400GiB
# 2nodes:  900GiB
# 4nodes: 1800GiB
# 6nodes: 2700GiB

sbatch -C gpu --nodes=$nodes --output=vdbench-$tag-$nodes.out%j <<\EOF
#!/bin/bash
#SBATCH --job-name="DW-vdbench"
#SBATCH --threads-per-core=1
#SBATCH --cpus-per-task=12
#SBATCH --time=0:30:00
#DW jobdw access_mode=striped capacity=1800GiB type=scratch
#DW stage_in type=directory source=/scratch/snx3000/mvalle/dwtestdaint/vdbench destination=$DW_JOB_STRIPED/vdbench
#-----------------------------------------------------------

wd=`pwd`
cd $wd

bs=16M

echo "### Blocksize: $bs on $SLURM_JOB_NUM_NODES nodes"

echo "create_anchors=yes" > tmp-multi-host    
echo "hd=default,user=mvalle,shell=ssh" >> tmp-multi-host    
for h in `scontrol show hostnames`
do
    echo "hd=$h,system=$h" >> tmp-multi-host
done
cat tmp-multi-host tmpparams > vdbench/testparams
rm tmp-multi-host tmpparams

echo ==========================================================================

cp vdbench/testparams ${DW_JOB_STRIPED}vdbench
cd ${DW_JOB_STRIPED}vdbench

srun -N $SLURM_JOB_NUM_NODES ./vdbench -f testparams -o $wd/out-vdbench-dw dir=${DW_JOB_STRIPED}testdir-vdbench bs=$bs

echo ==========================================================================

cd $wd/vdbench
srun -N $SLURM_JOB_NUM_NODES ./vdbench -f testparams -o $wd/out-vdbench-scratch dir=$wd/testdir-vdbench bs=$bs

EOF

