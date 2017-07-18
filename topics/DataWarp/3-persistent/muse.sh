
cd $SCRATCH/dwtest

pidfile=/tmp/dw$$.pid

sbatch -C mc > $pidfile <<\EOF
#!/bin/bash -l
#SBATCH --job-name="DW-use1-permanent"
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --output=use1-permanent.out%j
#DW persistentdw name=mvalleBB1
#DW persistentdw name=mvalleBB2
#-----------------------------------------------------------

env | grep DW_

f1=$DW_PERSISTENT_STRIPED_mvalleBB1/example1.dat

touch $f1

f2=$DW_PERSISTENT_STRIPED_mvalleBB2/example2.dat

touch $f2

ls -l $DW_PERSISTENT_STRIPED_mvalleBB1
ls -l $DW_PERSISTENT_STRIPED_mvalleBB2

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
#DW persistentdw name=mvalleBB1
#DW persistentdw name=mvalleBB2
#-----------------------------------------------------------

env | grep DW_

ls -l $DW_PERSISTENT_STRIPED_mvalleBB1
ls -l $DW_PERSISTENT_STRIPED_mvalleBB2

EOF

