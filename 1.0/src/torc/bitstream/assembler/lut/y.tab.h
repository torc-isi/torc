// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

// This program is free software: you can redistribute it and/or modify it under the terms of the 
// GNU General Public License as published by the Free Software Foundation, either version 3 of the 
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this program.  If 
// not, see <http://www.gnu.org/licenses/>.

/// \file
/// \brief Header forwarding LutScanner includes to the LutParser.

/* When using automake the bison parser file "xyz.yy" is processed by the
 * ylwrap script. It calls bison in a separate directory, which outputs source
 * to the default names "y.tab.c" and "y.tab.h". The ylwrap script then renames
 * these files into "xyz.cc" and "xyz.h" and tries to update include references
 * using sed. However this does not work for the C++ parser skeleton, so the
 * source file "xyz.cc" still refers to the default "y.tab.h". The easiest
 * work-around is to use this forwarding include file. */

#include "torc/bitstream/assembler/lut/LutScanner.hpp"

