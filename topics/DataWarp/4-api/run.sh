
cp ./datawarp_stager $SCRATCH/dwtest
cd $SCRATCH/dwtest

sbatch -C mc <<\EOF
#!/bin/bash
#SBATCH --job-name="DW-api-staging"
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --output=staging-api.out%j
#DW jobdw access_mode=striped capacity=200GiB type=scratch
#-----------------------------------------------------------


echo ./datawarp_stager in ${DW_JOB_STRIPED}SIO $SCRATCH/dwtest/example.dat
./datawarp_stager in ${DW_JOB_STRIPED}SIO $SCRATCH/dwtest/example.dat
echo
echo ./datawarp_stager wfile $DW_JOB_STRIPED/SIO
./datawarp_stager wfile $DW_JOB_STRIPED/SIO
echo

ls -l $SCRATCH/dwtest/example.dat
ls -l ${DW_JOB_STRIPED}SIO

echo ./datawarp_stager qfile $DW_JOB_STRIPED/SIO
./datawarp_stager qfile $DW_JOB_STRIPED/SIO
echo

touch $DW_JOB_STRIPED/SIO

echo
echo ./datawarp_stager out $DW_JOB_STRIPED/SIO $SCRATCH/dwtest/stage_out_file.dat
./datawarp_stager out $DW_JOB_STRIPED/SIO $SCRATCH/dwtest/stage_out_file.dat

echo
echo ./datawarp_stager wfile $DW_JOB_STRIPED/SIO
./datawarp_stager wfile $DW_JOB_STRIPED/SIO
echo

ls -l ${DW_JOB_STRIPED}SIO
ls -l $SCRATCH/dwtest/stage_out_file.dat

echo
echo ./datawarp_stager qfile $DW_JOB_STRIPED/SIO
./datawarp_stager qfile $DW_JOB_STRIPED/SIO

EOF

