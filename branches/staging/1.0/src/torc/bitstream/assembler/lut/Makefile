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

# Relative Path to Torc's src directory
SRC_DIR = ../../../..

## Include Torc/Boost default environment variables
-include $(SRC_DIR)/Makefile.local
TORC_DIR ?= $(shell pwd)/$(SRC_DIR)/torc

# Inlclude objects list and CFLAGS
include Makefile.objects

# Objects for this directory
OBJS := $(TORC_BITSTREAM_ASSEMBLER_LUT_OBJS)

# Include common Makefile targets
include $(TORC_DIR)/Makefile.targets
