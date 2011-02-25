// Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
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
/// \brief Header for primitive parser.
 
#ifndef _WRITE_PRIMITIVE_H
#define _WRITE_PRIMITIVE_H

#include "torc/packer/PrimitiveSet.hpp"
#include "torc/physical/Factory.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


namespace torc {
 namespace physical {
 	
 					
     /// \brief Pare Primitives.
     
     class WritePrimitive {
     		protected:
     			
     			// Output file
     			FILE *outFile;
     			// Output file name
     			const char  *outFileName;
					     							
				public:
					WritePrimitive(){}
					
					/// \brief write an XDLRC file
					void WritePrimitiveFile(const char *outFileName, PrimitiveSetSharedPtr primitives);
		};
		
} // namespace physical
} // namespace torc
 
#endif // TORC_PHYSICAL_WRITE_PRIMITIVE_HPP


