#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --gpus-per-node=0
#SBATCH --cpus-per-task=1
#SBATCH --account=cin_staff
#SBATCH --partition=m100_usr_prod
#SBATCH --time 01:00:00


#
# Load the environment needed to run the job
cd /m100_work/ExaF_prod22/unitTests

#
# Clean environment
module purge

#
# Load needed environment
module load git
module load cmake
module load gnu/8.4.0
module load zlib/1.2.11--gnu--8.4.0
module load szip/2.1.1--gnu--8.4.0
module load hdf5/1.12.0--gnu--8.4.0


#
# Clean all the repository
for i in `find /m100_work/ExaF_prod22/wp2-matrices/TestMatrices/ -type f -iname 'owner'` ; do
  prefix=`echo ${i} | sed -e '{s/^\///};{s/\/owner//};{s/\//_/g}' | sed -e 's/^m100_work_ExaF_prod22_wp2-matrices_TestMatrices_//'`;
  ownerName=${i};
  neighName=`echo ${i} | sed -e 's/owner/neighbour/'`;
  outDir="./"
  ./converter.x -o ${ownerName} -n ${neighName} -p ${prefix} -d ${outDir} -r 2 -t 5  >> convert000.log
done


#
# Exit with no error
exit 0;
