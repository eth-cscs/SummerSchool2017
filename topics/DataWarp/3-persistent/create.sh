
cd $SCRATCH/dwtest

sbatch -C mc <<\EOF
#!/bin/bash
#SBATCH --job-name="DW-create-permanent"
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --output=/dev/null
#BB create_persistent name=mvalleBB capacity=100GiB access=striped type=scratch 
EOF

