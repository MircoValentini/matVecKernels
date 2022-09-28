#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --gpus-per-node=0
#SBATCH --exclusive
#SBATCH --account=cin_staff
#SBATCH --partition=m100_usr_prod
#SBATCH --time 05:00:00


#
# Load the environment needed to run the job
exe=/m100_work/ExaF_prod22/testsSoftware/matVecKernels/bin/matVecKernels.x
cd /m100_work/ExaF_prod22/unitTests/matVecData/${1}

#
# Init log file 
date > run_${2}.log

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
module load spectrum_mpi/10.4.0--binary

#
# Run all the test cases
for i in *.h5 ; do
       	prefix=`echo ${i} | sed -e 's/\_matVec.*$//'`;
	idx=`echo ${i} | sed -e '{s/^.*\_matVec_\([0-9]*\).h5$/\1/};{s/^[0]*//}'`; 
	if [[ ${#idx} ==  0 ]] ; then idx=0; fi
	RCM_FLAG=`echo ${prefix} | sed -e '{s/\_.*$//};{s/^.*\-//}'`;
	ALG=`echo ${2} | sed -e 's/\_.*$//'`;
        if [[ ${ALG} == "BlockRowPerm" ||  ${ALG} == "BlockRowPermCC" ]] ; then
		if [[ ${RCM_FLAG} == "RCM" ]] ; then
                 	date >> run_${2}.log
                 	echo ${i} ${prefix} ${idx} ${1} ${2} ${PWD} >> run_${2}.log 
                 	echo "mpirun --bind-to core -n 32 ${exe} -d "." -p "${prefix}" -n "matVec"  -i ${idx} -k ${2}  -o timings_${2}.sqlite.db" >> run_${2}.log 
                	echo "---------------------------------------------------------------------------------------------------"
                        mpirun --bind-to core -n 32 ${exe} -d "." -p "${prefix}" -n "matVec"  -k ${2} -i ${idx} -o timings_${2}.sqlite.db
		fi
	else
               	date >> run_${2}.log
               	echo ${i} ${prefix} ${idx} ${1} ${2} ${PWD} >> run_${2}.log 
               	echo "mpirun --bind-to core -n 32 ${exe} -d "." -p "${prefix}" -n "matVec"  -i ${idx} -k ${2} -o timings_${2}.sqlite.db" >> run_${2}.log
              	echo "---------------------------------------------------------------------------------------------------"
                mpirun --bind-to core -n 32 ${exe} -d "." -p "${prefix}" -n "matVec"  -k ${2} -i ${idx} -o timings_${2}.sqlite.db
	fi

done


date >> run_${2}.log

#
# Exit with no error
exit 0;
