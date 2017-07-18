
cd $SCRATCH/dwtest

sbatch -C mc <<\EOF
#!/bin/bash
#SBATCH --job-name="DW-destroy-permanent"
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --output=/dev/null
#BB destroy_persistent name=mvalleBB1
#BB destroy_persistent name=mvalleBB2
EOF

