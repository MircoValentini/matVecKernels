# yoctoFOAM-unitTests

Management of UnitTests for yoctoFOAM kernels


## how to work on a unit test

0) choose a unitTest to port to cuda hadrware,
1) go to the directory ./src/<nameOfTheUnitTest>,
2) try to build the unitTest with the script "<nameOfTheUnitTest>_build.sh" usign the correct flags to build the desired version (CPU,CUDA,OPEN_ACC),
3) modify the file "<nameOfTheUnitTest>_kernels.H" in order to port the kernels to the GPU
4) fix test "<nameOfTheUnitTest>_IO.H" in case of errors
5) when built run with the command: "ncu -f --set full  -o unitTestProfiling --import-source yes  ./test.x"



LDU-orderedCOO
LDU-CSC-CSR
LDU-CSR
MSR














