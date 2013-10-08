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

THIS_DIR = sandbox
SRC_DIR = .
BOOST_INCLUDE_DIR = /usr/local/include/boost-1_42

# C++ flags
CCFLAGS = \
				-x c++ \
				-g \
				-Wall \
				-I$(SRC_DIR) \
				-I$(BOOST_INCLUDE_DIR) \
				$

# object files
OBJS = \
				ArchitectureExample.o \
				BitstreamExample.o \
				GenericExample.o \
				PhysicalExample.o \
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# not all targets are real
.PHONY: all code clean

sandbox: architecture_sandbox bitstream_sandbox generic_sandbox physical_sandbox

# make the architecture example executable
architecture_sandbox: torc code references
	$(CC) \
				$(REAL_OBJECTS) \
				ArchitectureExample.o \
				$(LDFLAGS) \
				-o $@ \
				$

# make the example example executable
bitstream_sandbox: torc code references
	$(CC) \
				$(REAL_OBJECTS) \
				BitstreamExample.o \
				$(LDFLAGS) \
				-o $@ \
				$

# make the generic example executable
generic_sandbox: torc code references
	$(CC) \
				$(REAL_OBJECTS) \
				GenericExample.o \
				$(LDFLAGS) \
				-o $@ \
				$

# make the XDL example executable
physical_sandbox: torc code references
	$(CC) \
				$(REAL_OBJECTS) \
				PhysicalExample.o \
				$(LDFLAGS) \
				-o $@ \
				$

# Include automatically generated dependencies or silently make them.
-include $(DEPS)

# build the code
code: $(OBJS)

# compile c++ sources 
%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

# build dependency files
%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;

# compile c sources 
%.o: %.c
	$(CC) $(CCFLAGS) -c -o $@ $<

# build dependency files
%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;

# include the main Torc Makefile
include Makefile.src
