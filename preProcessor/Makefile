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
all: converter


#
# handles
#
# bench: $(RUNDIR)/benchmark.x

converter: $(BINDIR)/converter.x


#
# Links 
#
#$(RUNDIR)/benchmark.x: $(BINDIR)/benchmark.x
#	${LN} ../$< $@


$(RUNDIR)/converter.x: $(BINDIR)/converter.x
	${LN} ../$< $@



#
# Executables
#
#$(BINDIR)/benchmark.x: $(OBJDIR)/benchmark.o \
#                       $(OBJDIR)/IO.o
#	$(CXXL) $^ -o $@ -I$(HDF5_INCDIR) -L$(HDF5_LIBDIR) -lhdf5 -lm

$(BINDIR)/converter.x: $(OBJDIR)/converter.o \
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
	$(RM) $(BINDIR)/*.x
