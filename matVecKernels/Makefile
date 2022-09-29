.PHONY: clean

#
# You need modules for:
# - gcc
# - hdf5
# - mpi
#



RM=rm -rf
CXXC=mpiCC -c
CXXL=mpiCC
CC=gcc -c

CCFLAGS=-O3 -funroll-loops  -mcpu=native -mtune=native -ftree-vectorize
# CCFLAGS=-O0 -g3
CLFALGS=


INCDIR= \
-I ./src \
-I ./include \
-I ./src/matVec \
-I ./src/matVec/COO_COO \
-I ./src/matVec/CSC_CSR \
-I ./src/matVec/BlockRowPerm \
-I ./src/matVec/BlockRowPermCC \
-I ./src/matVec/BlockRowPerm_sELL \
-I ./src/matVec/CSR_CSR \
-I ./src/matVec/CSR \
-I ./src/implicitDiv \
-I ./src/implicitDiv/LDU-orderedCOO \
-I ./src/implicitDiv/MSR \
-I ./src/implicitDiv/LDU-CSR \
-I ${HDF5_INC}


HEADERS= \
./tests/blockCompressor.H \
./src/matVec/CSR/CSR_kernels.H \
./src/matVec/CSR/CSR_IO.H \
./src/matVec/CSC_CSR/CSC_CSR_IO.H \
./src/matVec/CSC_CSR/CSC_CSR_kernels.H \
./src/matVec/BlockRowPerm/BlockRowPerm_IO.H \
./src/matVec/BlockRowPerm/BlockRowPerm_kernels.H \
./src/matVec/BlockRowPerm/BlkRowPerm.H \
./src/matVec/BlockRowPermCC/BlockRowPermCC_IO.H \
./src/matVec/BlockRowPermCC/BlockRowPermCC_kernels.H \
./src/matVec/BlockRowPermCC/BlkRowPermCC.H \
./src/matVec/CSR_CSR/CSR_CSR_IO.H \
./src/matVec/CSR_CSR/CSR_CSR_kernels.H \
./src/matVec/BlockRowPerm_sELL/blockCompressor.H \
./src/matVec/BlockRowPerm_sELL/BlockRowPerm_sELL_kernels.H \
./src/matVec/BlockRowPerm_sELL/BlockRowPerm_sELL_IO.H \
./src/matVec/BlockRowPerm_sELL/BlkRowPerm_sELL.H \
./src/matVec/BlockRowPerm_sELL/parser_sELL.H \
./src/matVec/COO_COO/COO_COO_IO.H \
./src/matVec/COO_COO/COO_COO_kernels.H \
./src/matVec/matVecTestFactory.H \
./src/matVec/unitTestMatVec_IO.H \
./src/implicitDiv/implicitDivTestFactory.H \
./src/implicitDiv/unitTestimplicitDiv_IO.H \
./src/implicitDiv/LDU-orderedCOO/LDU-orderedCOO-implicitDiv_IO.H \
./src/implicitDiv/MSR/MSR-implicitDiv_IO.H \
./src/implicitDiv/LDU-CSR/LDU-CSR-implicitDiv_IO.H \
./src/unitTests.H \
./include/singleTestDevel.H \
./include/dataBase.H \
./include/unitTestFactory.H \
./include/sqlite3ext.h \
./include/unitTestBase.H \
./include/dataTypes.H \
./include/timing.H \
./include/LDUdataTypes.H \
./include/sqlite3.h \
./include/UnitTestsIOManager.H


all: ./bin/matVecKernels.x


./bin/matVecKernels.x: ./obj/matVecKernels.o ./obj/sqlite3.o
	$(CXXL) $^ -o $@ -L ${HDF5_LIB} -lhdf5 -ldl -lpthread

./obj/matVecKernels.o: ./src/main.C ./obj/sqlite3.o $(HEADERS)
	$(CXXC) $(CCFLAGS) $(INCDIR) -D__MPI__ -D__TEST_CPU__ -D__COMPILER__=gcc -D__MACHINE__=schrodinger -D__FLAVOUR__=release $< -o $@


./obj/sqlite3.o: ./src/sqlite3.c ./include/sqlite3.h ./include/sqlite3ext.h
	$(CC) $< -o $@



clean:
	${RM} ./bin/*.x
	${RM} ./obj/*.o
