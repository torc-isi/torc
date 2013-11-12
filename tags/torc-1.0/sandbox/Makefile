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

SRC_DIR = ../src
-include $(SRC_DIR)/Makefile.local
TORC_DIR ?= $(shell pwd)/$(SRC_DIR)/torc
include $(TORC_DIR)/Makefile.objects

# LDFLAGS
LDFLAGS = \
	-m64 \
	-L/usr/local/lib \
	-dead_strip \
	$(BOOST_LIB_DIR_OPTION) \
	-lboost_filesystem \
	-lboost_regex \
	-lboost_signals \
	-lboost_system \
	-lboost_thread \
	-lstdc++ \
	-lm \
	$

# Achitecture Example Objects
ARCHITECTURE_EXAMPLE_EXEC := ArchitectureExample
ARCHITECTURE_EXAMPLE_OBJS := ArchitectureExample.o

# Bitstream Example Objects
BITSTREAM_EXAMPLE_EXEC := BitstreamExample
BITSTREAM_EXAMPLE_OBJS := BitstreamExample.o

# Generic Example Objects
GENERIC_EXAMPLE_EXEC := GenericExample
GENERIC_EXAMPLE_OBJS := GenericExample.o

# Physical Example Objects
PHYSICAL_EXAMPLE_EXEC := PhysicalExample
PHYSICAL_EXAMPLE_OBJS := PhysicalExample.o

# Local objects
OBJS = \
	$(ARCHITECTURE_EXAMPLE_OBJS) \
	$(BITSTREAM_EXAMPLE_OBJS) \
	$(GENERIC_EXAMPLE_OBJS) \
	$(PHYSICAL_EXAMPLE_OBJS) \
	$

# Torc Real Objects File
TORC_REAL_OBJS_FILE = TorcRealObjects

# Local dependencies
DEPS = $(OBJS:.o=.d)

.PHONY: all

# Make everything by default
all: \
	$(ARCHITECTURE_EXAMPLE_EXEC) \
	$(BITSTREAM_EXAMPLE_EXEC) \
	$(GENERIC_EXAMPLE_EXEC) \
	$(PHYSICAL_EXAMPLE_EXEC) \
	$(VERILOG_EXAMPLE_EXEC) \
	$

# Dependency Include
ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), clean_torc)
-include $(DEPS)
-include $(TORC_DEPS)
endif
endif

# Create torc real objects file
$(TORC_REAL_OBJS_FILE): $(TORC_REAL_OBJS)
	@echo $(TORC_REAL_OBJS) > $(TORC_REAL_OBJS_FILE)

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

# make the XDL example executable
$(PHYSICAL_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(PHYSICAL_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(PHYSICAL_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# make the Verilog example executable
$(VERILOG_EXAMPLE_EXEC): $(TORC_REAL_OBJS_FILE) $(VERILOG_EXAMPLE_OBJS)
	$(CC) \
	@$(TORC_REAL_OBJS_FILE) \
	$(VERILOG_EXAMPLE_OBJS) \
	$(CCFLAGS) \
	$(LDFLAGS) \
	-o $@

# clean everything (builds on Makefile.targets clean)
clean::
	rm -f $(ARCHITECTURE_EXAMPLE_EXEC)
	rm -f $(BITSTREAM_EXAMPLE_EXEC)
	rm -f $(GENERIC_EXAMPLE_EXEC)
	rm -f $(PHYSICAL_EXAMPLE_EXEC)
	rm -f $(VERILOG_EXAMPLE_EXEC)
	rm -f $(TORC_REAL_OBJS_FILE)

# Clean Torc Objects / Dependencies
clean_torc:
	rm -f $(TORC_REAL_OBJS_FILE) $(TORC_REAL_OBJS) $(TORC_DEPS)

# Include Makefile.targets
include $(TORC_DIR)/Makefile.targets
