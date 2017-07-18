
cd $SCRATCH/dwtest

pidfile=/tmp/dw$$.pid

sbatch -C mc > $pidfile <<\EOF
#!/bin/bash -l
#SBATCH --job-name="DW-use1-permanent"
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --output=use1-permanent.out%j
#DW persistentdw name=mvalleBB
#-----------------------------------------------------------

env | grep DW_

f=$DW_PERSISTENT_STRIPED_mvalleBB/example.dat

touch $f

ls -l $DW_PERSISTENT_STRIPED_mvalleBB

EOF

cat $pidfile
pid=`awk '{print $4}' $pidfile`
rm $pidfile

sbatch -C mc -d afterok:$pid <<\EOF
#!/bin/bash -l
#SBATCH --job-name="DW-use2-permanent"
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --output=use2-permanent.out%j
#DW persistentdw name=mvalleBB
#-----------------------------------------------------------

env | grep DW_

ls -l $DW_PERSISTENT_STRIPED_mvalleBB

EOF

