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

#include "torc/packer/WritePrimitive.hpp"

namespace torc {
namespace physical {
	
	
 void WritePrimitive::WritePrimitiveFile(const char *name, PrimitiveSetSharedPtr primitives) {

   outFileName = name;

   if (!(outFile = fopen(outFileName, "w")))
      printf("Cannot open file '%s'.\n", outFileName);
     
   fprintf(outFile, "(primitive_defs %i\n", (int)(primitives->getPrimitiveCount()));
   //write primitives
   for(PrimitiveSet::PrimitiveSharedPtrConstIterator it = primitives->primitivesBegin(); it != primitives->primitivesEnd(); ++it){
   		fprintf(outFile, "\t(primitive_def %s %d %d\n", (*it)->getName().c_str(), ((int)((*it)->getPrimitivePinCount())), ((int)((*it)->getElementCount())));
   		//write PrimitivePins
   		for(Primitive::PrimitivePinSharedPtrConstIterator pp = (*it)->PrimitivePinsBegin(); pp != (*it)->PrimitivePinsEnd(); ++pp){
   			string sTmp = ((*pp)->getType()==OutputP) ? "output" : "input";
   			fprintf(outFile, "\t\t(pin %s %s %s)\n", (*pp)->getElementName().c_str(), (*pp)->getName().c_str(), sTmp.c_str());
   		}
   		//write elements
   		for(Primitive::ElementSharedPtrConstIterator ep = (*it)->elementsBegin(); ep != (*it)->elementsEnd(); ++ep){
   			fprintf(outFile, "\t\t(element %s %d\n", (*ep)->getName().c_str(), (int)((*ep)->getPrimitivePinCount()));
   			//write element PrimitivePins
   			for(Element::PrimitivePinSharedPtrConstIterator pp = (*ep)->PrimitivePinsBegin(); pp != (*ep)->PrimitivePinsEnd(); ++pp){
   				string sTmp = ((*pp)->getType()==OutputP) ? "output" : "input";
   				fprintf(outFile, "\t\t\t(pin %s %s)\n", (*pp)->getName().c_str(), sTmp.c_str());
   			}
   			//write element confs
   			if((*ep)->getConfigCount()>0){
   				fprintf(outFile, "\t\t\t(cfg");
   				for(Element::ConfigConstIterator cp = (*ep)->confsBegin(); cp != (*ep)->confsEnd(); ++cp)
   					fprintf(outFile, " %s", (*cp).c_str());
   				fprintf(outFile,")\n");
   			}
   			//write element connections
   			for(Element::ConnectionSharedPtrConstIterator cp = (*ep)->connectionsBegin(); cp != (*ep)->connectionsEnd(); ++cp){
   				Connection::ConnectionPinSharedPtrConstIterator src = (*cp)->getSource();
   				Connection::ConnectionPinSharedPtrConstIterator dst = (*cp)->getSink();
   				fprintf(outFile,"\t\t\t(conn %s %s ==> %s %s)\n", (*src).getElementName().c_str(), (*src).getPinName().c_str(),																						
   																													(*dst).getElementName().c_str(), (*dst).getPinName().c_str());
   			}
   			fprintf(outFile, "\t\t)\n");
   		}
   		fprintf(outFile, ")\n");
   	}
  
   fclose(outFile);
   
   return;
}

} // namespace physical
} // namespace torc

