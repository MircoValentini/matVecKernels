#
# Definition of a class used to select data from a Database of timings
#
#
class timings:
	def __init__( self, name ):



#
#
# Configurazione della macchina su cui sono stati fatti i tests
machine 
primary, machine, node, compiler, flags, hostname, mpiIndex, mpiSize


#
#
# Configurazione della macchina su cui sono stati fatti i tests
testCase 
primary, prefix, isRCM, procID, caseIndex, nRowsNominal, nRowsTrue, bandWidth, jumpParameter

#
#
# Tempi di warmup
kernelType
primary, kernelName, 



#
#
# Tempi di warmup
warmup
primary, elapsedTime, clockTime, cudaTime



#
#
# Tempi a regime
race
primary, nLaps, elapsedTime, clockTime, cudaTime



#
#
# Tabella che mete tutto in relazione
profiling
primary, machine, testCase, warmup, race
