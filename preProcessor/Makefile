.PHONY: clean

CXXL=g++
CXXC=g++ -c

# CXXFLAGS=-O0 -g
CXXFLAGS=-O3 -funroll-loops -ftree-vectorize
HDF5_INCDIR=${HDF5_INC}
HDF5_LIBDIR=${HDF5_LIB}
MATDIR=./inc/matrices
PRMDIR=./inc/permutations
SRCDIR=./src
INCDIR=./inc
BINDIR=./bin
OBJDIR=./obj
RUNDIR=./run
RM=rm -f
LN=ln -fs

#
# Entry point
#
all: FOAM2H5MatConverter

#
# handles
#
# bench: $(RUNDIR)/benchmark.x

FOAM2H5MatConverter: $(BINDIR)/FOAM2H5MatConverter

#
# Links 
#
#$(RUNDIR)/benchmark.x: $(BINDIR)/benchmark.x
#	${LN} ../$< $@

$(RUNDIR)/FOAM2H5MatConverter: $(BINDIR)/FOAM2H5MatConverter
	${LN} ../$< $@

#
# Executables
#
#$(BINDIR)/benchmark.x: $(OBJDIR)/benchmark.o \
#                       $(OBJDIR)/IO.o
#	$(CXXL) $^ -o $@ -I$(HDF5_INCDIR) -L$(HDF5_LIBDIR) -lhdf5 -lm

$(BINDIR)/FOAM2H5MatConverter: $(OBJDIR)/converter.o \
                       $(OBJDIR)/IO.o
	$(CXXL) $^ -o $@ -I$(HDF5_INCDIR) -L$(HDF5_LIBDIR) -lhdf5 -lm


#
# Objects
#
$(OBJDIR)/benchmark.o: $(SRCDIR)/benchmark.C   \
                             $(PRMDIR)/blockRowPerm.H      \
                             $(INCDIR)/IO.H                \
                             $(INCDIR)/timing.H            \
                             $(INCDIR)/sparseMatrixBase.H  \
                             $(INCDIR)/randomMatrix.H      \
                             $(INCDIR)/connectivityIO.H    \
                             $(MATDIR)/sparseMatrices.H    \
                             $(MATDIR)/sparseStdMatrix.H   \
                             $(MATDIR)/sparseBlkMatrix.H
	$(CXXC) $(CXXFLAGS) -I$(HDF5_INCDIR) -I$(INCDIR) $< -o $@

$(OBJDIR)/converter.o: $(SRCDIR)/converter.C   \
                             $(PRMDIR)/blockRowPerm.H      \
                             $(INCDIR)/IO.H                \
                             $(INCDIR)/timing.H            \
                             $(INCDIR)/sparseMatrixBase.H  \
                             $(INCDIR)/randomMatrix.H      \
                             $(INCDIR)/connectivityIO.H    \
                             $(MATDIR)/sparseMatrices.H    \
                             $(MATDIR)/sparseStdMatrix.H   \
                             $(MATDIR)/sparseBlkMatrix.H
	$(CXXC) $(CXXFLAGS) -I$(HDF5_INCDIR) -I$(INCDIR) $< -o $@


$(OBJDIR)/IO.o: $(SRCDIR)/IO.C $(INCDIR)/IO.H
	$(CXXC) $(CXXFLAGS) -I$(HDF5_INCDIR) -I$(INCDIR) $< -o $@

#
# Phony
#
clean:
	$(RM) $(OBJDIR)/*.o
	$(RM) $(BINDIR)/*
