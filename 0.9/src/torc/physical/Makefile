# Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
# $HeadURL$
# $Id$

# This program is free software: you can redistribute it and/or modify it under the terms of the 
# GNU General Public License as published by the Free Software Foundation, either version 3 of the 
# License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
# the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with this program.  If 
# not, see <http://www.gnu.org/licenses/>.

THIS_DIR = torc/physical
SRC_DIR = ../..
BOOST_INCLUDE_DIR = /usr/local/include/boost-1_42

# C++ flags
CCFLAGS = \
				-x c++ \
				-g \
				-Wall \
				-Werror \
				-I$(SRC_DIR) \
				-I$(BOOST_INCLUDE_DIR) \
				$

# object files
OBJS = \
				Annotated.o \
				AnnotatedUnitTest.o \
				Circuit.o \
				CircuitUnitTest.o \
				Config.o \
				ConfigUnitTest.o \
				ConfigMap.o \
				ConfigMapUnitTest.o \
				Design.o \
				DesignUnitTest.o \
				DesignRegressionTest.o \
				Factory.o \
				FactoryUnitTest.o \
				Instance.o \
				InstanceUnitTest.o \
				InstancePin.o \
				InstancePinUnitTest.o \
				InstanceReference.o \
				InstanceReferenceUnitTest.o \
				Module.o \
				ModuleUnitTest.o \
				Named.o \
				NamedUnitTest.o \
				Net.o \
				NetUnitTest.o \
				OutputStreamHelpers.o \
				OutputStreamHelpersUnitTest.o \
				Pip.o \
				PipUnitTest.o \
				Port.o \
				PortUnitTest.o \
				Progenitor.o \
				ProgenitorUnitTest.o \
				Progeny.o \
				ProgenyUnitTest.o \
				Renamable.o \
				RenamableUnitTest.o \
				Routethrough.o \
				RoutethroughUnitTest.o \
				SampleCodeUnitTest.o \
				TilewirePlaceholder.o \
				TilewirePlaceholderUnitTest.o \
				XdlExporter.o \
				XdlExporterUnitTest.o \
				XdlImporter.o \
				XdlImporterUnitTest.o \
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# generate object file lists for regular and test code
REAL_OBJS = $(filter-out %UnitTest.o %RegressionTest.o, $(OBJS))
TEST_OBJS = $(filter %UnitTest.o %RegressionTest.o, $(OBJS))

# not all targets are real
.PHONY: all code xdl clean

# make everything
all: code

# clean everything
clean:
	rm -f $(OBJS) $(DEPS)
	rm -f ObjectFileList RealObjectFileList TestObjectFileList
	make -C xdl clean

# Include automatically generated dependencies or silently make them.
-include $(DEPS)

# build the code
code: xdl $(OBJS) RealObjectFileList TestObjectFileList

# build the xdl subdirectory
xdl:
	make -C xdl

# build the real object list
RealObjectFileList: $(OBJS)
	@echo $(REAL_OBJS:%=$(THIS_DIR)/%) > RealObjectFileList

# build the test object list
TestObjectFileList: $(OBJS)
	@echo $(TEST_OBJS:%=$(THIS_DIR)/%) > TestObjectFileList

# compile c++ sources 
%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

# build dependency files
# special case for XdlImporter.cpp to silence gcc -MM if xdl/XdlParser.hpp doesn't exist
XdlImporter.d: XdlImporter.cpp
	@$(if $(wildcard xdl/XdlParser.hpp), , touch xdl/XdlParser.hpp) # preempt gcc -MM error
	@set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;
# regular case
%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;
