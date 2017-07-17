#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --reservation=summer
#SBATCH --constraint=gpu
#SBATCH --output=job.out

srun hostname
