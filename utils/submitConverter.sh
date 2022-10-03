#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --gpus-per-node=0
#SBATCH --cpus-per-task=1
#SBATCH --output=R-%x.%j.out
#SBATCH --error=R-%x.%j.err
#SBATCH --account=cin_staff
#SBATCH --partition=m100_usr_prod
#SBATCH --time 02:00:00

set -e

# User defined variables
TEST_MATRICES_DIR=/m100_work/ExaF_prod22/wp2-matrices/TestMatrices/
NUM_MAT=1
NUM_VEC=1

# Internal variables 
if [ -n $SLURM_JOB_ID ] ; then
  SCRIPT_PATH=$(scontrol show job $SLURM_JOBID | awk -F= '/Command=/{print $2}')
else
  SCRIPT_PATH="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";
fi
ROOT_PATH=$(dirname $(dirname ${SCRIPT_PATH}))
DATA_DIR=${ROOT_PATH}"/data"
BIN_DIR=${ROOT_PATH}"/preProcessor/bin"

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
for filename in `find $TEST_MATRICES_DIR -type f -iname 'owner'` ; do
  relName=$(realpath --relative-to=${TEST_MATRICES_DIR} ${filename}) 
  relPath=$(dirname $(dirname ${relName}))
  prefix=`echo ${relName} | sed -e '{s/^\///};{s/\/owner//};{s/\//_/g}'`
  ownerName=${filename}
  neighName=`echo ${filename} | sed -e 's/owner/neighbour/'`;
  outDir=${DATA_DIR}"/"${relPath}
  mkdir -p ${outDir}
  ${BIN_DIR}"/FOAM2H5MatConverter" -o ${ownerName} -n ${neighName} -p ${prefix} -d ${outDir} -r ${NUM_MAT} -t ${NUM_VEC}
done

