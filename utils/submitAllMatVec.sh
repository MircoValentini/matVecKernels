#!/bin/bash

if [[ $# != 2  ]] ; then
  echo "ERROR: 2 arguments expected!!!	"
  exit
fi


#
# sbatch submitMatVec.sh 000000 COO_COO
declare -a algorithms=("COO_COO"  "CSC_CSR"  "CSR_CSR"  "CSR"  "BlockRowPerm_2" "BlockRowPerm_4" "BlockRowPerm_6" "BlockRowPerm_8" "BlockRowPerm_10" "BlockRowPerm_12" "BlockRowPerm_14" "BlockRowPerm_16" "BlockRowPerm_18" "BlockRowPerm_20" "BlockRowPerm_22" "BlockRowPerm_24" "BlockRowPerm_26" "BlockRowPerm_28" "BlockRowPerm_30" "BlockRowPerm_32" "BlockRowPerm_34" "BlockRowPerm_36" "BlockRowPerm_38" "BlockRowPerm_40" "BlockRowPerm_42" "BlockRowPerm_44" "BlockRowPerm_46" "BlockRowPerm_48"  "BlockRowPerm_50" "BlockRowPerm_55" "BlockRowPerm_60" "BlockRowPerm_65" "BlockRowPerm_70" "BlockRowPerm_75" "BlockRowPerm_80" "BlockRowPerm_85" "BlockRowPerm_90" "BlockRowPerm_95" "BlockRowPerm_100"  )

declare -a timings=("20:00:00" "20:00:00" "20:00:00" "20:00:00"  "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00" "10:00:00"  )      

# Length of the arrays
N=${#algorithms[@]}

# create run cases
for (( i=${1}; i<=${2}; i++ )) ; do
	for (( j=0; j<${N}; j++ )) ; do
		id=`printf "%6.6d" ${i}`	
		time=${timings[${j}]}
		# echo ${id} ${algorithms[${j}]}
                sbatch --time ${time}  submitMatVec.sh ${id} ${algorithms[${j}]}
	done
done

