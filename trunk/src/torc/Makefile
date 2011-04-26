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

THIS_DIR = torc
SRC_DIR = ..
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
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# generate object file lists for regular and test code
REAL_OBJS = $(filter-out %UnitTest.o %RegressionTest.o, $(OBJS))
TEST_OBJS = $(filter %UnitTest.o %RegressionTest.o, $(OBJS))

# not all targets are real
.PHONY: all code architecture bitstream common examples externals generic packer physical \
	placer router utils clean

# make everything
all: code

# build the code
code: architecture bitstream common examples externals generic physical packer placer \
	router utils $(OBJS) RealObjectFileList TestObjectFileList

# build the subdirectories
architecture:
	make -C architecture
bitstream:
	make -C bitstream
common:
	make -C common
examples:
	make -C examples
externals:
	make -C externals
generic:
	make -C generic
packer:
	make -C packer
physical:
	make -C physical
placer:
	make -C placer
router:
	make -C router
utils:
	make -C utils

# build the real object list
RealObjectFileList:
	@echo $(REAL_OBJS:%=$(THIS_DIR)/%) > RealObjectFileList

# build the test object list
TestObjectFileList:
	@echo $(TEST_OBJS:%=$(THIS_DIR)/%) > TestObjectFileList

# compile c++ sources 
%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

# build dependency files
%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;

clean:
	make -C architecture clean
	make -C bitstream clean
	make -C common clean
	make -C examples clean
	make -C externals clean
	make -C generic clean
	make -C packer clean
	make -C physical clean
	make -C placer clean
	make -C router clean
	make -C utils clean
	rm -f $(OBJS) $(DEPS)
	rm -f ObjectFileList RealObjectFileList TestObjectFileList
