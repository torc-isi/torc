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

#include "torc/packer/ParsePrimitive.hpp"
#include "torc/packer/RcFactory.hpp"
#include <stdarg.h>

namespace torc {
namespace physical {
	

	static const char delimiters[] = " \n\t\f";	  /* token delimiters */

void ParsePrimitive::readCurrentLine() {
   static const unsigned initial_size = 1024;

   if (!line) {
      lineSize = initial_size;
      line = new char[lineSize];
   }

   unsigned offset = 0;

   for (;;) {

      if (!fgets(line + offset, lineSize - offset, inFile)) { if 
      (ferror(inFile)) { perror("read error"); exit(1); } if (!offset) {
      strcpy(line, ".end\n"); } break; }

      unsigned len = offset + strlen(line + offset);

      if (line[len-1] == '\n') {
	 ++lineNumber;
	 if (line[len-2] == '\\') len -= 2; else break;
      }
      else {
	 if (len != lineSize - 1) break;

	 char *new_line = new char[lineSize << 1];
	 for (unsigned i = 0; i < lineSize; ++i)
	    new_line[i] = line[i];
	 delete [] line;
	 line = new_line;
	 lineSize <<= 1;
      }

      offset = len;
   }
   
}

	void ParsePrimitive::printError(const char *format, ...) {
   va_list args;

   if (lineNumber)
      fprintf(stderr, "Parse printError in %s:%d: ", inFileName, lineNumber);
   else fprintf(stderr, "Parse printError: ");

   va_start(args, format);
   vfprintf(stderr, format, args);
   va_end(args);

   exit(1);
	}
	
 PrimitiveSetSharedPtr ParsePrimitive::ParsePrimitiveFile(const char *name) {

   inFileName = name;

   lineNumber = 0;
   if (!(inFile = fopen(inFileName, "r")))
      printError("Cannot open file '%s'.\n", inFileName);
   lineNumber = 1;

   int  site_no, pin_no, element_no;
   string strTmp;
   string typeTmp;
   string compName;
   string element1;
   string element2;
   string PrimitivePin1;
   string PrimitivePin2;
   
	 
   PrimitiveSetSharedPtr p;
   
   readCurrentLine();	    
   
   for (;;) {
   	string token = strtok(line, delimiters);
      if (token[0] == '(') {
				 if (token == "(xdl_resource_report") {
				    string model = strtok(NULL, delimiters);
				    model = strtok(NULL, delimiters);
				   p = RcFactory::newPrimitiveSetPtr(model);
				   readCurrentLine();
				   continue;
				 }
				 else
				    if (token =="(tiles") {
				    	readCurrentLine();
				     continue;
				    }
				 		else
					    if (token =="(tile") {
					     for(int i=0; i<5; ++i)
					     	strTmp = strtok(NULL, delimiters);
					     site_no = atoi(strTmp.c_str());
					     for(int i=0; i<site_no; ++i)
					     	readCurrentLine();
					     readCurrentLine();
					     continue;	     	
					    }
				    else 
				       if (token == "(primitive_defs") {
				       	readCurrentLine();
				       	continue;
				      }
				      else 
				       if (token == "(primitive_def") {
				       	compName = strtok(NULL, delimiters);
				       	strTmp = strtok(NULL, delimiters);
				       	pin_no = atoi (strTmp.c_str());
				       	strTmp = strtok(NULL, delimiters);
				       	element_no=atoi(strTmp.c_str());
				       	PrimitiveSharedPtr pp = RcFactory::newPrimitivePtr(compName);
				       	p->addPrimitive(pp);
				       					   
				       	for(int i=0; i<pin_no; ++i){
				       		readCurrentLine();
				       		strTmp = strtok(line, delimiters);
					       	strTmp = strtok(NULL, delimiters);
					       	compName = strtok(NULL, delimiters);
					       	typeTmp = strtok(NULL, delimiters);
							typeTmp.resize(typeTmp.length() - 1);
				       		PrimitivePinSharedPtr 	newPrimitivePin = RcFactory::newPrimitivePinPtr (strTmp, compName, ((typeTmp == "input") ? InputP : OutputP));
				       		pp->addPrimitivePin(newPrimitivePin);
				       	}
				       	
				       	for(int i=0; i<element_no; ++i){
				       		readCurrentLine();
				       		strTmp = strtok(line, delimiters);
					       	compName = strtok(NULL, delimiters);
					       	strTmp = strtok(NULL, delimiters);
					       	pin_no = atoi(strTmp.c_str());
					       	ElementSharedPtr ep = RcFactory::newElementPtr(compName);
					       	pp->addElement(ep);
					       	
					       	// read element pins
					       	for(int i=0; i<pin_no; ++i){
					       		readCurrentLine();
					       		strTmp = strtok(line, delimiters);
						       	compName = strtok(NULL, delimiters);
						       	typeTmp = strtok(NULL, delimiters);                                                                                                     
								typeTmp.resize(typeTmp.length() - 1);
					       		PrimitivePinSharedPtr 	newPrimitivePin = RcFactory::newPrimitivePinPtr (strTmp, compName, ((typeTmp == "input") ? InputP : OutputP));
					       		ep->addPrimitivePin(newPrimitivePin);
					       	}
					       	readCurrentLine();
					       	strTmp = strtok(line, delimiters);
					       	
					       	// read element configs
					       	if (strTmp == "(cfg"){
					       		for(;;){
					       			string confTmp = strtok(NULL, delimiters);
					       			if(confTmp[confTmp.size()-1]==')'){
										confTmp.resize(confTmp.length() - 1);
					       				ep->addConfig(confTmp);
					       				break;
					       			}
					       			ep->addConfig(confTmp);
					       		}
					       		readCurrentLine();
					       		
					       		strTmp = strtok(line, delimiters);
					       	}
					       	
					       	// read element connections
					       	while(strTmp[0]!=')'){
					       		element1 = strtok(NULL, delimiters);
					       		PrimitivePin1 = strtok(NULL, delimiters);
					       		strTmp = strtok(NULL, delimiters);
					       		element2 = strtok(NULL, delimiters);
					       		PrimitivePin2 = strtok(NULL, delimiters);
								PrimitivePin2.resize(PrimitivePin2.length() - 1);
					       		if(strTmp == "<=="){
					       			swap(element1,element2);
					       			swap(PrimitivePin1,PrimitivePin2);
					       		}
					       							       			
					       		ConnectionPin pin1 = RcFactory::newConnectionPin(element1,PrimitivePin1);
					       		ConnectionPin pin2 = RcFactory::newConnectionPin(element2,PrimitivePin2);
					       		ConnectionSharedPtr newConn = RcFactory::newConnectionPtr(line);
					       		newConn->addConnectionPin(pin1);
					       		newConn->addConnectionPin(pin2);
					       		ep->addConnection(newConn);
					       		readCurrentLine();
					       		strTmp = strtok(line, delimiters);
					       	}
					       		
				       	}
				       	continue;
				      }
						  else
							if ((token == "(end") || (token == "(summary")){
								cout<<'(';
								for(int i=0; i<3; ++i){
									strTmp = strtok(NULL, delimiters);
									cout<<strTmp<<" ";
								}
								cout<<endl;
							   break;
							 }
		 }
      else
	 			if (token[0] != '\n' && token[0] != '#' &&  token[0] != ')') printError("`.' expected.\n");

      readCurrentLine();	    /* get next line */
   }

   lineSize = 0;
   if (line) delete [] line;
   line = NULL;
   fclose(inFile);
   
   return p;
}

} // namespace physical
} // namespace torc

