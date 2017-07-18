
tmp=$SCRATCH/dwtest
if [ ! -d $tmp ]
then
    mkdir $tmp
fi
cd $tmp

sbatch -C mc <<\EOF
#!/bin/bash
#SBATCH --job-name="DW-hello-world"
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --output=hello-world.out%j
#DW jobdw access_mode=striped capacity=100GiB type=scratch
#-----------------------------------------------------------

env | grep DW_

touch ${DW_JOB_STRIPED}/HelloWorld
mkdir ${DW_JOB_STRIPED}/tmp
ls -l ${DW_JOB_STRIPED}

EOF

