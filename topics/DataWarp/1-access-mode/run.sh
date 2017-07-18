tmp=$SCRATCH/dwtest
if [ ! -d $tmp ]
then
    mkdir $tmp
fi
cp srun.sh $tmp
cd $tmp

sbatch -C mc <<\EOF
#!/bin/bash
#SBATCH --job-name="DW-striped-private"
#SBATCH --nodes=2
#SBATCH --time=0:05:00
#SBATCH --output=striped-private.out%j
#DW jobdw access_mode=striped,private capacity=100GiB type=scratch
#-----------------------------------------------------------

srun ./srun.sh

EOF
