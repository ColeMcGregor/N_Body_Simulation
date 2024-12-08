#!/bin/bash
#SBATCH --job-name=n_body_sim_job          # Job name
#SBATCH --output=job-output/sim_output.txt # Output file
#SBATCH --error=job-output/sim_error.txt   # Error file
#SBATCH --partition=skx-dev                # Partition name (specifically the skylake development partition)
#SBATCH --nodes=1                          # Total number of nodes
#SBATCH --ntasks=1                         # Total number of tasks
#SBATCH --cpus-per-task=16                 # Number of cores per task
#SBATCH --time=01:00:00                    # Maximum run time for script

module load gcc

# ensures threads are bound to a specific core
export OMP_PROC_BIND=true
# set num threads 
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
# check num threads
echo $OMP_NUM_THREADS

# compile simulation program
make

# run
./Simulation input.txt
