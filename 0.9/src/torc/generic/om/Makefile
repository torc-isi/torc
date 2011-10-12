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

THIS_DIR = torc/generic/om
SRC_DIR = ../../..
BOOST_INCLUDE_DIR = /usr/local/include/boost-1_42

# C++ flags
CCFLAGS = \
				-x c++ \
				-g \
				-Wall \
				-Wno-error \
				-I$(SRC_DIR) \
				-I$(BOOST_INCLUDE_DIR) \
				$

# object files
OBJS = \
				Cell.o \
				Commentable.o \
				Connectable.o \
				ConnectionHandler.o \
				Design.o \
				DumpRestoreData.o \
				EdifVersion.o \
				Extern.o \
				Flattening.o \
				Instance.o \
				InstanceArray.o \
				InstanceArrayMember.o \
				Library.o \
				Nameable.o \
				Net.o \
				NetBundle.o \
				ObjectFactory.o \
				Parameter.o \
				ParameterArray.o \
				ParameterArrayElement.o \
				ParameterMap.o \
				Port.o \
				PortAttributes.o \
				PortBundle.o \
				PortBundleReference.o \
				PortDelay.o \
				PortList.o \
				PortReference.o \
				Property.o \
				PropertyContainer.o \
				Renamable.o \
				Root.o \
				ScalarNet.o \
				ScalarPort.o \
				ScalarPortReference.o \
				ScaleFactor.o \
				SingleInstance.o \
				SingleParameter.o \
				Value.o \
				VectorNet.o \
				VectorNetBit.o \
				VectorPort.o \
				VectorPortBit.o \
				VectorPortBitReference.o \
				VectorPortReference.o \
				View.o \
				Visitable.o \
				VisitorType.o \
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# generate object file lists for regular and test code
REAL_OBJS = $(filter-out %UnitTest.o %RegressionTest.o, $(OBJS))
TEST_OBJS = $(filter %UnitTest.o %RegressionTest.o, $(OBJS))

# not all targets are real
.PHONY: all code clean

# make everything
all: code

# clean everything
clean:
	rm -f $(OBJS) $(DEPS)
	rm -f ObjectFileList RealObjectFileList TestObjectFileList

# Include automatically generated dependencies or silently make them.
-include $(DEPS)

# build the code
code: $(OBJS) RealObjectFileList TestObjectFileList

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
%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;
