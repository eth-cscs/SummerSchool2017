#!/bin/bash

#SBATCH --job-name="petsc-miniapp-test"
#SBATCH --nodes=1
#SBATCH --output=job%j.out
#SBATCH --time=00:10:00
#SBATCH --constraint=gpu
#SBATCH --reservation=summer

srun ./main -ts_monitor -snes_monitor -ksp_monitor 
