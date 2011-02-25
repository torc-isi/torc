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

ifneq ($(TORC_X), 1)
LDFLAGS = \
				-L/usr/local/lib64 \
				-L/usr/local/lib \
				-dead_strip \
				-lboost_filesystem \
				-lboost_regex \
				-lboost_signals \
				-lboost_system \
				-lboost_thread \
				$
LDTESTFLAGS = \
				-lboost_unit_test_framework \
				$
else
LDFLAGS = \
				-arch x86_64 \
				-L/usr/local/lib \
				-dead_strip \
				-lboost_filesystem \
				-lboost_regex \
				-lboost_signals \
				-lboost_system \
				-lboost_thread \
				$
LDTESTFLAGS = \
				-lboost_unit_test_framework \
				$
endif


REAL_OBJECTS = \
				@torc/architecture/RealObjectFileList \
				@torc/bitstream/RealObjectFileList \
				@torc/bitstream/build/RealObjectFileList \
				@torc/common/RealObjectFileList \
				@torc/externals/md5/RealObjectFileList \
				@torc/externals/zlib/RealObjectFileList \
				@torc/generic/RealObjectFileList \
				@torc/generic/decompiler/RealObjectFileList \
				@torc/generic/om/RealObjectFileList \
				@torc/generic/parser/RealObjectFileList \
				@torc/generic/util/RealObjectFileList \
				@torc/physical/RealObjectFileList \
				@torc/physical/xdl/RealObjectFileList \
				@torc/placer/RealObjectFileList \
				@torc/router/RealObjectFileList \
				@torc/utils/RealObjectFileList \
				$

TEST_OBJECTS = \
				@torc/architecture/TestObjectFileList \
				@torc/bitstream/TestObjectFileList \
				@torc/bitstream/build/TestObjectFileList \
				@torc/common/TestObjectFileList \
				@torc/externals/md5/TestObjectFileList \
				@torc/externals/zlib/TestObjectFileList \
				@torc/generic/TestObjectFileList \
				@torc/generic/decompiler/TestObjectFileList \
				@torc/generic/om/TestObjectFileList \
				@torc/generic/parser/TestObjectFileList \
				@torc/generic/util/TestObjectFileList \
				@torc/physical/TestObjectFileList \
				@torc/physical/xdl/TestObjectFileList \
				@torc/placer/TestObjectFileList \
				@torc/router/TestObjectFileList \
				@torc/utils/TestObjectFileList \
				$

DEVICES = $(wildcard torc/devices/*.db)

DEBUG_BITSTREAMS = $(wildcard torc/bitstream/regression/*.debug.bit)

BITSTREAM_REFERENCES = \
				Spartan3EUnitTest.reference.bit \
				Spartan6UnitTest.reference.bit \
				VirtexUnitTest.reference.bit \
				VirtexEUnitTest.reference.bit \
				Virtex2UnitTest.reference.bit \
				Virtex2PUnitTest.reference.bit \
				Virtex4UnitTest.reference.bit \
				Virtex5UnitTest.reference.bit \
				Virtex6UnitTest.reference.bit \
				$

EXAMPLE_REFERENCES = \
				GenericExample.reference.edf \
				$

PHYSICAL_REFERENCES = \
				DesignUnitTest.reference.xdl \
				$

ROUTER_REFERENCES = \
				TraceRegressionTest.Virtex5.xdl \
				TraceRegressionTest.VirtexTbuf.xdl \
				$

.PHONY: torc references databases clean

# make everything

all: databases test architecture_example bitstream_example edif_obfuscator generic_example \
	physical_example xdldiff browser

# make the test executable
test: torc references databases
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				$(TEST_OBJECTS) \
				$(LDTESTFLAGS) \
				-o $@ \
				$

# make the architecture example executable
architecture_example: torc references
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/examples/ArchitectureExample.o \
				-o $@ \
				$

# make the example example executable
bitstream_example: torc references
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/examples/BitstreamExample.o \
				-o $@ \
				$

# make the generic example executable
generic_example: torc references
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/examples/GenericExample.o \
				-o $@ \
				$

# make the EDIF obfuscator example executable
edif_obfuscator: torc references
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/examples/EdifObfuscator.o \
				-o $@ \
				$

# make the XDL example executable
physical_example: torc references
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/examples/PhysicalExample.o \
				-o $@ \
				$

# make the xdldiff executable
xdldiff: torc
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/utils/XdlDiffMain.o \
				-o $@ \
				$

# make the architecture browser executable
browser: torc databases
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/utils/BrowserMain.o \
				-o $@ \
				$

# make the XDL extractor executable
markextract: torc databases
	g++ \
				$(LDFLAGS) \
				$(REAL_OBJECTS) \
				torc/utils/XdlExtractMain.o \
				-o $@ \
				$


# make the torc subdirectory
torc:
	make -C torc

# copy all of the regression test references into place
references: \
				regression \
				${DEBUG_BITSTREAMS:torc/bitstream/%=%} \
				${BITSTREAM_REFERENCES:%=regression/%} \
				${EXAMPLE_REFERENCES:%=regression/%} \
				${PHYSICAL_REFERENCES:%=regression/%} \
				${ROUTER_REFERENCES:%=regression/%} \
				$

# make the regression directory
regression: 
	mkdir regression

# copy physical regression test references into place
regression/${PHYSICAL_REFERENCES}: regression
	cp ${PHYSICAL_REFERENCES:%=torc/physical/%} regression

# copy bitstream regression test references into place
regression/${BITSTREAM_REFERENCES}: regression
	cp ${BITSTREAM_REFERENCES:%=torc/bitstream/%} regression

# copy example references into place
regression/${EXAMPLE_REFERENCES}: regression
	cp ${EXAMPLE_REFERENCES:%=torc/examples/%} regression

# copy router regression test references into place
regression/${ROUTER_REFERENCES}: regression
	cp ${ROUTER_REFERENCES:%=torc/router/%} regression

# copy all of the family and device databases into place
databases: devices $(DEVICES:torc/%=%)

devices: 
	mkdir devices

devices/%.db: torc/devices/%.db
	cp $< $@

regression/%.debug.bit: torc/bitstream/regression/%.debug.bit
	cp $< $@

# clean everything
clean:
	make -C torc clean
	rm -f $(OBJS)
	rm -f test architecture_example bitstream_example edif_obfuscator generic_example \
		physical_example xdldiff
	rm -rf regression
	rm -rf devices
