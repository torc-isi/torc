# Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
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

SRC_DIR = ./
-include $(SRC_DIR)/Makefile.local
TORC_DIR ?= $(shell pwd)/$(SRC_DIR)/torc
include $(TORC_DIR)/Makefile.objects

LDFLAGS = \
	-m64 \
	-L/usr/local/lib \
	$(CPLEX_CCLNFLAGS) \
	-dead_strip \
	$(BOOST_LIB_DIR_OPTION) \
	-lboost_filesystem \
	-lboost_regex \
	-lboost_signals \
	-lboost_system \
	-lboost_thread \
	-lstdc++ \
	-lm

LDTESTFLAGS = \
	-lboost_unit_test_framework

LDTESTFLAGS += -shared-libgcc

# Unit Test Executable
TEST_EXEC = TorcTest

# Bit Assembler Example Objects
BITASSEMBLER_EXAMPLE_EXEC := BitAssemblerExample
BITASSEMBLER_EXAMPLE_OBJS := $(TORC_EXAMPLES)/BitAssemblerExample.o

# Bit Assembler Libgen Objects
BITASSEMBLER_LIBGEN_EXEC := AssemblerLibGenExample
BITASSEMBLER_LIBGEN_OBJS := $(TORC_EXAMPLES)/AssemblerLibGenExample.o

# Achitecture Example Objects
ARCHITECTURE_EXAMPLE_EXEC := ArchitectureExample
ARCHITECTURE_EXAMPLE_OBJS := $(TORC_EXAMPLES)/ArchitectureExample.o

# Bitstream Example Objects
BITSTREAM_EXAMPLE_EXEC := BitstreamExample
BITSTREAM_EXAMPLE_OBJS := $(TORC_EXAMPLES)/BitstreamExample.o

# Generic Example Objects
GENERIC_EXAMPLE_EXEC := GenericExample
GENERIC_EXAMPLE_OBJS := $(TORC_EXAMPLES)/GenericExample.o

# EDIF Flattener Objects
EDIF_FLATTENER_EXEC := EdifFlattener
EDIF_FLATTENER_OBJS := $(TORC_EXAMPLES)/EdifFlattener.o

# EDIF Obfuscator Objects
EDIF_OBFUSCATOR_EXEC := EdifObfuscator
EDIF_OBFUSCATOR_OBJS := $(TORC_EXAMPLES)/EdifObfuscator.o

# Physical Example Objects
PHYSICAL_EXAMPLE_EXEC := PhysicalExample
PHYSICAL_EXAMPLE_OBJS := $(TORC_EXAMPLES)/PhysicalExample.o

# Verilog Example Objects
ifeq ($(TORC_1_0_ENABLE_VERILOG), 1)
VERILOG_EXAMPLE_EXEC := VerilogExample
VERILOG_EXAMPLE_OBJS := $(TORC_EXAMPLES)/VerilogExample.o
endif

# XDL Diff Objects
XDLDIFF_EXEC := XdlDiff
XDLDIFF_OBJS := $(TORC_UTILS)/XdlDiffMain.o

# Browser Applciation Objects
BROWSER_EXEC := Browser
BROWSER_OBJS := $(TORC_UTILS)/BrowserMain.o

# Mark Extract Objects
MARKEXTRACT_EXEC := XdlExtract
MARKEXTRACT_OBJS := $(TORC_UTILS)/XdlExtractMain.o

# Local objects
OBJS = \
	$(BITASSEMBLER_EXAMPLE_OBJS) \
	$(BITASSEMBLER_LIBGEN_OBJS) \
	$(ARCHITECTURE_EXAMPLE_OBJS) \
	$(BITSTREAM_EXAMPLE_OBJS) \
	$(GENERIC_EXAMPLE_OBJS) \
	$(EDIF_FLATTENER_OBJS) \
	$(EDIF_OBFUSCATOR_OBJS) \
	$(PHYSICAL_EXAMPLE_OBJS) \
	$(VERILOG_EXAMPLE_OBJS) \
	$(XDLDIFF_OBJS) \
	$(BROWSER_OBJS) \
	$(MARKEXTRACT_OBJS)

# Torc Real Objects File
TORC_REAL_OBJS_FILE = TorcRealObjects

# Torc Test Objects File
TORC_TEST_OBJS_FILE = TorcTestObjects

# Local dependencies
DEPS = $(OBJS:.o=.d)

# declare the test targets as phony
.PHONY: test regression

# Make everything by default
all: \
	$(TEST_EXEC) \
	$(BITASSEMBLER_EXAMPLE_EXEC) \
	$(BITASSEMBLER_LIBGEN_EXEC) \
	$(ARCHITECTURE_EXAMPLE_EXEC) \
	$(BITSTREAM_EXAMPLE_EXEC) \
	$(GENERIC_EXAMPLE_EXEC) \
	$(EDIF_FLATTENER_EXEC) \
	$(EDIF_OBFUSCATOR_EXEC) \
	$(PHYSICAL_EXAMPLE_EXEC) \
	$(VERILOG_EXAMPLE_EXEC) \
	$(XDLDIFF_EXEC) \
	$(BROWSER_EXEC) \
	$(MARKEXTRACT_EXEC)

# Dependency Include
ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), clean_torc)
-include $(DEPS)
-include $(TORC_DEPS)
endif
endif

# Build Real Objects File
$(TORC_REAL_OBJS_FILE): $(TORC_REAL_OBJS) $(TORC_DEPS)
	@echo $(TORC_REAL_OBJS) > $(TORC_REAL_OBJS_FILE)

# Build Test Objects File
$(TORC_TEST_OBJS_FILE): $(TORC_TEST_OBJS) $(TORC_DEPS)
	@echo $(TORC_TEST_OBJS) > $(TORC_TEST_OBJS_FILE)

# make the test executable
$(TEST_EXEC): $(TORC_REAL_OBJS_FILE) $(TORC_TEST_OBJS_FILE)
	mkdir -p regression
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	@$(TORC_TEST_OBJS_FILE) \
	$(LDTESTFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the architecture example executable
$(BITASSEMBLER_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(BITASSEMBLER_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(BITASSEMBLER_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make assembler libgen
$(BITASSEMBLER_LIBGEN_EXEC): $(TORC_REAL_OBJS_FILE) $(BITASSEMBLER_LIBGEN_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(BITASSEMBLER_LIBGEN_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the architecture example executable
$(ARCHITECTURE_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(ARCHITECTURE_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(ARCHITECTURE_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the example example executable
$(BITSTREAM_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(BITSTREAM_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(BITSTREAM_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the generic example executable
$(GENERIC_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(GENERIC_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(GENERIC_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the EDIF flattener example executable
$(EDIF_FLATTENER_EXEC): $(TORC_REAL_OBJS_FILE) $(EDIF_FLATTENER_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(EDIF_FLATTENER_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the EDIF obfuscator example executable
$(EDIF_OBFUSCATOR_EXEC): $(TORC_REAL_OBJS_FILE) $(EDIF_OBFUSCATOR_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(EDIF_OBFUSCATOR_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the XDL example executable
$(PHYSICAL_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(PHYSICAL_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(PHYSICAL_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the Verilog example executable
#ifeq ($(strip $(TORC_1_0_ENABLE_VERILOG)),)
ifeq ($(TORC_1_0_ENABLE_VERILOG), 1)
$(VERILOG_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(VERILOG_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(VERILOG_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@
endif

# make the xdldiff executable
$(XDLDIFF_EXEC): $(TORC_REAL_OBJS_FILE) $(XDLDIFF_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(XDLDIFF_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the architecture browser executable
$(BROWSER_EXEC): $(TORC_REAL_OBJS_FILE) $(BROWSER_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(BROWSER_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the XDL extractor executable
$(MARKEXTRACT_EXEC): $(TORC_REAL_OBJS_FILE) $(MARKEXTRACT_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(MARKEXTRACT_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# build and run the unit tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# build and run the regression tests (these currently fail)
regression: $(TEST_EXEC)
	./$(TEST_EXEC) -regression

# clean everything (builds on Makefile.targets clean)
clean::
	rm -f $(TEST_EXEC)
	rm -f $(BITASSEMBLER_EXAMPLE_EXEC)
	rm -f $(BITASSEMBLER_LIBGEN_EXEC)
	rm -f $(ARCHITECTURE_EXAMPLE_EXEC)
	rm -f $(BITSTREAM_EXAMPLE_EXEC)
	rm -f $(GENERIC_EXAMPLE_EXEC)
	rm -f $(EDIF_FLATTENER_EXEC)
	rm -f $(EDIF_OBFUSCATOR_EXEC)
	rm -f $(PHYSICAL_EXAMPLE_EXEC)
	rm -f $(VERILOG_EXAMPLE_EXEC)
	rm -f $(XDLDIFF_EXEC)
	rm -f $(BROWSER_EXEC)
	rm -f $(MARKEXTRACT_EXEC)
	rm -f $(TORC_REAL_OBJS_FILE)
	rm -f $(TORC_TEST_OBJS_FILE)
	rm -rf regression

# Clean Torc Objects / Dependencies
clean_torc:
	rm -f $(TORC_REAL_OBJS_FILE) $(TORC_TEST_OBJS_FILE) $(TORC_REAL_OBJS) $(TORC_TEST_OBJS) $(TORC_DEPS)

# Include Makefile.targets
include $(TORC_DIR)/Makefile.targets
