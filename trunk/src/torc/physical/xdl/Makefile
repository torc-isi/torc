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

THIS_DIR = torc/physical/xdl
SRC_DIR = ../../..
BOOST_INCLUDE_DIR = /usr/local/include/boost-1_42

# C++ flags
CCFLAGS = \
				-x c++ \
				-g \
				-Wno-trigraphs \
				-Wall \
				-Werror \
				-I$(SRC_DIR) \
				-I$(BOOST_INCLUDE_DIR) \
				-isystem $(SRC_DIR)/$(THIS_DIR) \
				$

# object files
OBJS = \
				XdlScanner.o \
				XdlParser.o \
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# generate object file lists for regular and test code
REAL_OBJS = $(filter-out %UnitTest.o %RegressionTest.o, $(OBJS))
TEST_OBJS = $(filter %UnitTest.o %RegressionTest.o, $(OBJS))

# not all targets are real
.PHONY: all flex-bison code clean

# make everything
all: flex-bison code

# Include automatically generated dependencies or silently make them.
#-include $(DEPS)

# make the scanner and parser
flex-bison: XdlScanner.cpp XdlParser.cpp

# generate the parser with bison
XdlParser.cpp: parser.yy
	/bin/sh ylwrap parser.yy y.tab.c XdlParser.cpp y.tab.h XdlParser.hpp y.output parser.output -- bison -y

# generate the scanner with flex
XdlScanner.cpp: scanner.ll
	/bin/sh ylwrap scanner.ll lex.yy.c XdlScanner.cpp -- flex -olex.yy.c

# compile the code
code: $(OBJS) RealObjectFileList TestObjectFileList

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
	gcc $(CCFLAGS) -c -o $@ $<

# build dependency files
%.d: %.cpp
	@set -e; rm -f $@; \
	gcc -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;

# remove everything that was built
clean:
	rm -f parser.cc parser.h
	rm -f scanner.cc
	rm -f $(OBJS) $(DEPS)
	rm -f ObjectFileList RealObjectFileList TestObjectFileList

