#!/bin/bash
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

# Opens Eclipse from the command line.
# 
# Inspired by http://lugendal.wordpress.com/2009/07/22/eclipse-ctd-new-project-fast/ and 
# http://stackoverflow.com/questions/1087573/open-a-specific-eclipse-project-from-command-line/6777801#6777801

ECLIPSE_EXECUTABLE=eclipse
WORKSPACE_PATH=`pwd`
PROJECTS_PATH=`pwd`/..

# make sure we know where to find eclipse
if ! command -v ${ECLIPSE_EXECUTABLE} &> /dev/null; then
	echo "Eclipse is not present in PATH."
	exit 1
fi

# this script will not work if Eclipse is already running
if ps ax | grep eclipse | grep -v grep | grep -v $0 > /dev/null; then
	echo "Please quit Eclipse before invoking this script."
	exit 1
fi

# open Eclipse in this workspace
${ECLIPSE_EXECUTABLE} \
	-data ${WORKSPACE_PATH} &
