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

THIS_DIR = torc/bitstream
SRC_DIR = ../..
BOOST_INCLUDE_DIR = /usr/local/include/boost-1_42

# C++ flags -- enable or disable -O3 as needed for sanity
CCFLAGS = \
				-x c++ \
				-g \
				-O3 \
				-Wall \
				-Werror \
				-I$(SRC_DIR) \
				-I$(BOOST_INCLUDE_DIR) \
				$

# object files
OBJS = \
				Bitstream.o \
				BitstreamUnitTest.o \
				DeviceInfo.o \
				DeviceInfoUnitTest.o \
				OutputStreamHelpers.o \
				Spartan3E.o \
				Spartan3EUnitTest.o \
				Spartan6.o \
				Spartan6UnitTest.o \
				SpartanBitstream.o \
				SpartanBitstreamUnitTest.o \
				Spartan16Bitstream.o \
				Spartan16BitstreamUnitTest.o \
				SpartanPacket.o \
				SpartanPacketUnitTest.o \
				Spartan16Packet.o \
				Spartan16PacketUnitTest.o \
				Virtex.o \
				VirtexUnitTest.o \
				VirtexE.o \
				VirtexEUnitTest.o \
				Virtex2.o \
				Virtex2UnitTest.o \
				Virtex2P.o \
				Virtex2PUnitTest.o \
				Virtex4.o \
				Virtex4UnitTest.o \
				Virtex5.o \
				Virtex5DeviceInfo.o \
				Virtex5UnitTest.o \
				Virtex6.o \
				Virtex6UnitTest.o \
				VirtexBitstream.o \
				VirtexBitstreamUnitTest.o \
				VirtexPacket.o \
				VirtexPacketUnitTest.o \
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# generate object file lists for regular and test code
REAL_OBJS = $(filter-out %UnitTest.o %RegressionTest.o, $(OBJS))
TEST_OBJS = $(filter %UnitTest.o %RegressionTest.o, $(OBJS))

# not all targets are real
.PHONY: all code build clean

# make everything
all: code

# Include automatically generated dependencies or silently make them.
-include $(DEPS)

# build the code
code: build $(OBJS) RealObjectFileList TestObjectFileList

# build the build subdirectory
build:
	make -C build

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

# clean everything
clean:
	rm -f $(OBJS) $(DEPS)
	rm -f ObjectFileList RealObjectFileList TestObjectFileList
	make -C build clean
