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

THIS_DIR = torc/generic/parser
SRC_DIR = ../../..
BOOST_INCLUDE_DIR = /usr/local/include/boost-1_42

# C++ flags
CCFLAGS = \
				-x c++ \
				-g \
				-Wall \
				-Werror \
				-I$(SRC_DIR) \
				-I$(BOOST_INCLUDE_DIR) \
				-isystem $(SRC_DIR)/$(THIS_DIR) \
				$

# object files
OBJS = \
				Driver.o \
				EdifContext.o \
				EdifParser.o \
				Linker.o \
				Parser.o \
				Scanner.o \
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# generate object file lists for regular and test code
REAL_OBJS = $(filter-out %UnitTest.o %RegressionTest.o, $(OBJS))
TEST_OBJS = $(filter %UnitTest.o %RegressionTest.o, $(OBJS))

# not all targets are real
.PHONY: all code clean # flex-bison

# make everything
all: code # flex-bison

# clean everything
clean:
	rm -f $(OBJS) $(DEPS)
	rm -f ObjectFileList RealObjectFileList TestObjectFileList

# make the scanner and parser
#flex-bison: Scanner.cc Parser.cc
flex-bison:

## generate the parser with bison
#Parser.cc: Parser.yy
#	/bin/sh ylwrap Parser.yy y.tab.c Parser.cc y.tab.h Parser.h y.output Parser.output -- /usr/local/bin/bison -y
#
## generate the scanner with flex
#Scanner.cc: Scanner.ll
#	/bin/sh ylwrap Scanner.ll lex.yy.c Scanner.cc -- flex -olex.yy.c

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

# compile the parser and scanner
Parser.o: Parser.cc
	gcc $(CCFLAGS) -c -o $@ $<
Scanner.o: Scanner.cc
	gcc $(CCFLAGS) -c -o $@ $<

# compile c++ sources 
%.o: %.cpp
	gcc $(CCFLAGS) -c -o $@ $<

# build dependency files
# Parser and Scanner have .cc extensions
%.d: %.cc
	@set -e; rm -f $@; \
	gcc -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;
# regular case
%.d: %.cpp
	@set -e; rm -f $@; \
	gcc -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;
