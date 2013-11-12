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
/// \brief Unit test for the SiteTypes class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Sites.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XdlImporter.hpp"
#include "torc/physical/Design.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/Circuit.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/Named.hpp"
#include <boost/regex.hpp>
#include <string>
#include <map>
#include <fstream>
#include <iostream>


namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)



/// \brief Unit test to iterate through the ConfigMaps of a XDL file.
BOOST_AUTO_TEST_CASE(iterate_configmaps) {

	using namespace torc::physical;

	// create the appropriate file paths                                                                
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "physical" / "DesignUnitTest.reference.xdl";
  
	// import the XDL design                                                                            
	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	XdlImporter importer;
	
	importer(fileStream, referencePath.string());

	// look up the design (and do something with it ...)                                                
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);

	Circuit::InstanceSharedPtrConstIterator p = designPtr->instancesBegin();
	Circuit::InstanceSharedPtrConstIterator e = designPtr->instancesEnd();

	while(p < e) { 
		InstanceSharedPtr instancePtr = *p++;
		std::cout << "Instance:" << instancePtr->getName() << std::endl;
		ConfigMap::const_iterator cp = instancePtr->configBegin();
		ConfigMap::const_iterator ce = instancePtr->configEnd();
		while(cp != ce) {	
			std::cout << "\tConfig:" << cp->first << "---->" << cp->second.getName() << ":" << cp->second.getValue() << std::endl;
			cp++;
		}
	}
}





bool findLUTbyCfg(const std::string isit);
bool findLUTbyCfg(const std::string isit)  
{    
	static const boost::regex e("(^(<eqn>)|((#LUT:|#RAM:|#ROM:)<eqn>)$)");    
	return regex_match(isit, e);  
}
bool findFFbyCfg(const std::string isit);
bool findFFbyCfg(const std::string isit)  
{    
	static const boost::regex e("(^#(FF|LATCH)$)");    
	return regex_match(isit, e);  
}
bool findINV(const std::string isit);
bool findINV(const std::string isit)
{
        static const boost::regex e("^.*_B$");
	return regex_match(isit, e);
}
bool findROUTETHROUGH(const std::string isit);
bool findROUTETHROUGH(const std::string isit)
{
        static const boost::regex e("^_ROUTETHROUGH.*");
	return regex_match(isit, e);
}
bool findAND(const std::string isit);
bool findAND(const std::string isit)
{
        static const boost::regex e("^.*AND.*");
	return regex_match(isit, e);
}
bool findVCC(const std::string isit);
bool findVCC(const std::string isit)
{
        static const boost::regex e("^.*VCC$");
	return regex_match(isit, e);
}
bool findGND(const std::string isit);
bool findGND(const std::string isit)
{
        static const boost::regex e("^.*GND$");
	return regex_match(isit, e);
}



///\brief Unit test to iterate through a database and classify all existing elements into pseudo-types
BOOST_AUTO_TEST_CASE(classify_elements){  
//xc6vhx380t xc5vlx30 xc4vfx60 xc6slx9l xc2vp40 xc2v40 xc3s250e
    
	// open and initialize a database  
	DDB ddb("xc5vlx30");  
	// look up the logic sites in the device  
	const Sites& sites = ddb.getSites();  

	// look up the primitive def types  
	typedef const Array<const PrimitiveDef> PrimitiveDefArray;  
	PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();  
	PrimitiveDefArray::const_iterator p = primitiveDefs.begin();  
	PrimitiveDefArray::const_iterator e = primitiveDefs.end();

	/// \brief Map of all elements.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllElements;
	AllElements allElements;
	/// \brief Map of all terminals.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllTerminals;
	AllTerminals allTerminals;
	/// \brief Map of all switches.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllSwitches;
	AllSwitches allSwitches;
	/// \brief Map of all orphans.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllOrphans;
	AllOrphans allOrphans;
	/// \brief Map of all muxes.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllMuxes;
	AllMuxes allMuxes;
	/// \brief Map of all inverters.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllInverters;
	AllInverters allInverters;
	/// \brief Map of all luts.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllLuts;
	AllLuts allLuts;
	/// \brief Map of all flops.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllFlops;
	AllFlops allFlops;
	/// \brief Map of all main elements.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllMains;
	AllMains allMains;
	/// \brief Map of all main elements.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllAnds;
	AllAnds allAnds;
	/// \brief Map of all main elements.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllVccs;
	AllVccs allVccs;
	/// \brief Map of all main elements.                                                          
	typedef std::map<std::string, PrimitiveElement*> AllGnds;
	AllGnds allGnds;

	//int i = 0;  
	// iterate over the primitive defs  
	while(p < e) {    
	// look up the current primitive def    
		const PrimitiveDef& primitiveDef = *p++;    
		//std::cout << "Inside Primitive Def:" << primitiveDef.getName() << std::endl << std::endl;
		typedef const Array<const PrimitivePin> PrimitivePinArray;
		// look up the primitive def pins
		const PrimitivePinArray& primitivePins = primitiveDef.getPins();
		PrimitivePinArray::const_iterator sp = primitivePins.begin();
		PrimitivePinArray::const_iterator se = primitivePins.end();
		const PrimitivePin primitivePin;
		// keep a track of all the primitive pins (to find the main elements, we need to compare elements with site pins)
		std::vector<std::string> primitivePinNames;	
		std::vector<std::string>::const_iterator spe = unique(primitivePinNames.begin(), primitivePinNames.end());
        (void) spe;

		// iterate over the primitive def pins
		if(primitivePins.getSize() > 0) {
		  //std::cout << "    ";
			while(sp < se) {
				const PrimitivePin& primitivePin = *sp++;
				primitivePinNames.push_back(primitivePin.getName());		  
			}
			//std::cout << std::endl;
		}

		// look up the primitive def elements    
		const PrimitiveElementArray& elements = primitiveDef.getElements();    
		PrimitiveElementArray::const_iterator ep = elements.begin();    
		PrimitiveElementArray::const_iterator ee = elements.end();    
		
		
		// iterate over the primitive def elements    
		if(elements.getSize() > 0) {      
			while(ep < ee) {	
				const PrimitiveElement& element = *ep++;	

				//std::cout << "Inside ELEMENT NAME:" << element.getName() << std::endl;
				/// \brief Map of all elements.                                                          
				allElements[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 

				/// \brief Capture the 'main' elements.                                                 
				if(primitiveDef.getName() == element.getName()){ 
					//std::cout << "\tMAIN element:" << element.getName() << std::endl;
					allMains[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
					allElements.erase(element.getName());
				}					
				//typedef const Array<const Sites::ElementPin> ElementPinArray;	
				const PrimitiveElementPinArray& elementPins = element.getPins();	
				PrimitiveElementPinArray::const_iterator pp = elementPins.begin();	
				PrimitiveElementPinArray::const_iterator pe = elementPins.end();

				//Count input pins of Muxes/Invs	
				int countPins = 0;	
				int isINV = 0;	
				std::string cfgValue;
				int totalPinCount = 0;			
				int cfgSize = 0;
				int notEmpty = 0;
				int isLUT = 0;
				int isFF = 0;
				int isVCC = 0;
				int isGND = 0;
				int isAND = 0;

				std::vector<std::string> elementPinNames;	
				std::vector<std::string>::const_iterator ppe = unique(elementPinNames.begin(), elementPinNames.end());
                (void) ppe;
	
				//Iterate through Element PINS	
				while(pp < pe) {	  
					const PrimitiveElementPin& elementPin = *pp++;	  
					//(void) elementPin;
	  	  
					//Check if the pin is an Input pin	  
					if(elementPin.isInput() == 1){	    
						typedef std::set<std::string> StringSet;	    
						const StringSet elementCfgs = element.getCfgs();	    
						StringSet::const_iterator cp = elementCfgs.begin();	    
						StringSet::const_iterator ce = elementCfgs.end();
	    	    
						//Iterate through the Cfgs	    
						while(cp != ce){
	      
							const std::string& isit = *cp++; 	      
							//Check if the pin names and cfgs match 	      
							if(elementPin.getName() == isit){		
								if(findINV(isit) == 1){		  
									elementPinNames.push_back(elementPin.getName());		  
									isINV = 1;		  
									countPins++;		
								}else{		  
									elementPinNames.push_back(elementPin.getName());		  
									countPins++;		
								}
								break;
							}
							cfgValue = isit;
							//trying to check if the # of cgfs and # of element pins match
							cfgSize = elementCfgs.size();
							if(!elementCfgs.empty()) notEmpty = 1;
								if(findLUTbyCfg(isit) == 1){
								isLUT = 1;
							}else if(findFFbyCfg(isit) == 1){
								isFF = 1;
							}							  					      
						}
					}
					if((findVCC(element.getName()) == 1) && (elementPin.getName() == "1")){
						isVCC = 1;
					}else if((findGND(element.getName()) == 1) && (elementPin.getName() == "0")){
						isGND = 1;
					}else if((findAND(element.getName()) == 1) && ((elementPin.getName() == "0") || (elementPin.getName() == "1"))){
						isAND = 1;
					}
					totalPinCount++;
				}	
				
			        if(countPins > 0) {				
				//If the pin names and cfgs match, it is a MUX (or INV)					
					if(countPins == 1){
						//std::cout << "\tSWITCH: " << element.getName() << "    "; //std::endl;
						allMuxes[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
						allSwitches[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
						allElements.erase(element.getName());
				        }else if(isINV == 1){	    
						//std::cout << "\tINV: " << element.getName() << "    ";
						allMuxes[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
						allInverters[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
						allElements.erase(element.getName());
					}else if(isINV == 0){	    
						//std::cout << "\tMUX: " << element.getName() << "    ";	    
						allMuxes[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
						allElements.erase(element.getName());
					}	  	  
					//std::cout << countPins << "    "; // << std::endl;	  
					//print the PINs that are matched with cfg within that element	  
					for(std::vector<std::string>::const_iterator b = elementPinNames.begin(); b != elementPinNames.end(); b++) {	    
						//std::cout << *b << "    ";	    
					}
					// check if the number of pins is equal to the number of cfgs 
					if(notEmpty == 1){
						if(countPins != cfgSize) throw 0;
					}

				}else if(isLUT == 1){
					//std::cout << "\tLUT: " << element.getName() << std::endl;//"    ";
					allLuts[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
					allElements.erase(element.getName());
				}else if(isFF == 1){
					//std::cout << "\tFLOP: " << element.getName() << std::endl;//"    ";
					allFlops[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
					allElements.erase(element.getName());	   
				}
				if(totalPinCount == 0){
					//std::cout << "\tORPHAN:" << element.getName() <<std::endl;
					allOrphans[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
					allElements.erase(element.getName());
				}
				if(findROUTETHROUGH(element.getName()) == 1){
					//std::cout << "ROUTETHROUGH FOUND! ---- d e l e t i n g !" << std::endl;
					allElements.erase(element.getName());
				}
				if(isAND == 1){//if(findAND(element.getName()) == 1){
					allAnds[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
					allElements.erase(element.getName());
				}
				if(isVCC == 1){
					allVccs[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
					allElements.erase(element.getName());
				}
				if(isGND == 1){
					allGnds[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
					allElements.erase(element.getName());
				}
				for(std::vector<std::string>::const_iterator s = primitivePinNames.begin(); s != primitivePinNames.end(); s++) {	    
					if(element.getName() == *s){
						//std::cout << "\tTERMINAL:" << element.getName() << std::endl;
						allTerminals[element.getName()] = const_cast <PrimitiveElement*>(&element); //element.getName(); 
						allElements.erase(element.getName());
						break;
					}
				}
			}
		}
		
		//std::cout << "-----------------------------------------------------------" <<std::endl;      		
		//std::cout << std::endl;	
	}
	std::map<std::string, PrimitiveElement*>::const_iterator pos;
	std::cout << "------ UNCLASSIFIED ELEMENTS -------" <<std::endl;
	for(pos = allElements.begin(); pos != allElements.end(); ++pos) {
		std::cout << "Element: " << pos->first << std::endl;// "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "------ MUXES -------" <<std::endl;
	for(pos = allMuxes.begin(); pos != allMuxes.end(); ++pos) {
		std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "------ LUTS -------" <<std::endl;
	for(pos = allLuts.begin(); pos != allLuts.end(); ++pos) {
        	std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}	
	std::cout << std::endl;
	std::cout << "------ FLOPS -------" <<std::endl;
	for(pos = allFlops.begin(); pos != allFlops.end(); ++pos) {
		std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "------ TERMINALS -------" <<std::endl;
	for(pos = allTerminals.begin(); pos != allTerminals.end(); ++pos) {
		std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "------ SWITCHES -------" <<std::endl;
	for(pos = allSwitches.begin(); pos != allSwitches.end(); ++pos) {
		std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "------ ANDS -------" <<std::endl;
	for(pos = allAnds.begin(); pos != allAnds.end(); ++pos) {
		std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "------ VCCS -------" <<std::endl;
	for(pos = allVccs.begin(); pos != allVccs.end(); ++pos) {
		std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}
        std::cout << std::endl;
	std::cout << "------ GNDS -------" <<std::endl;
	for(pos = allGnds.begin(); pos != allGnds.end(); ++pos) {
		std::cout << "Element: " << pos->first  << std::endl; //<< "    ";
		//std::cout << "Value:" << pos->second << std::endl;
	}


}    

     
		






/// \brief Unit test for the SiteTypes class.
BOOST_AUTO_TEST_CASE(SiteTypesUnitTest) {

        // open and initialize a database
        DDB ddb("xc5vlx30");
        // look up the logic sites in the device
        const Sites& sites = ddb.getSites();
        // look up the primitive def types
        typedef const Array<const PrimitiveDef> PrimitiveDefArray;
        PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();
        PrimitiveDefArray::const_iterator p = primitiveDefs.begin();
        PrimitiveDefArray::const_iterator e = primitiveDefs.end();
        int i = 0;
        // iterate over the primitive defs
	while(p < e) {
		// look up the current primitive def
		const PrimitiveDef& primitiveDef = *p++;
		std::cout << i << ": " << primitiveDef.getName() << std::endl;
		typedef const Array<const PrimitivePin> PrimitivePinArray;
		// look up the primitive def pins
		const PrimitivePinArray& primitivePins = primitiveDef.getPins();
		PrimitivePinArray::const_iterator sp = primitivePins.begin();
		PrimitivePinArray::const_iterator se = primitivePins.end();
		// iterate over the primitive def pins
		if(false && primitivePins.getSize() > 0) {
			std::cout << "    ";
			// output each of the pin names along with the flags (2-INPUT, 4-OUTPUT)
			while(sp < se) {
				const PrimitivePin& primitivePin = *sp++;
				std::cout << primitivePin.getName() << " ";
				std::cout << primitivePin.getFlags() << " ";
			}
			std::cout << std::endl;
		}
		// look up the primitive def elements
		
		const PrimitiveElementArray& elements = primitiveDef.getElements();
		PrimitiveElementArray::const_iterator ep = elements.begin();
		PrimitiveElementArray::const_iterator ee = elements.end();
		// iterate over the primitive def elements
		if(true && elements.getSize() > 0) {
			// output each of the element names and if the pin is a basic element
			while(ep < ee) {
				const PrimitiveElement& element = *ep++;
				std::cout << "ELEMENT NAME:" << element.getName() << " "<< std::endl;
				//std::cout << "    ";
				//std::cout<< "ELEMENT IS Basic ELement:" << element.isBel() << " " << std::endl;
				
				const PrimitiveElementPinArray& elementPins = element.getPins();
				PrimitiveElementPinArray::const_iterator pp = elementPins.begin();
				PrimitiveElementPinArray::const_iterator pe = elementPins.end();
				// iterate over the element pins
				if(elementPins.getSize()> 0) {
					
					// output each of the element pin names along with the element flags (2-Input, 4-Output)
					while(pp < pe) {
					//	std::cout << "    ";
						const PrimitiveElementPin& elementPin = *pp++;
						std::cout << elementPin.getElementPtr()->getName() << "." << elementPin.getName() << " ";
						//std::cout << ((elementPin.getElement()->getName() == element.getName()) 
						                     //? "" : (elementPin.getElement()->getName() + " "));
						//std::cout<< "ELEMENT PIN NAME:" << elementPin.getName() << " " << std::endl;
						//std::cout << "    ";
						//std::cout<< "ELEMENT FLAG NAME:" << elementPin.getFlags() << " " << std::endl;
						// typedef std::set<std::string> StringSet;
						// const StringSet elementCfgs = element.getCfgs();
						// StringSet::const_iterator cp = elementCfgs.begin();
						// StringSet::const_iterator ce = elementCfgs.end();
						// // iterate over the configs
						// if(elementCfgs.size() != 0){
						// 	// output each of the configs
						// 	while(cp != ce){
						// 	        std::cout << "    ";
                                                //                 std::cout << "CONFIG NAME: " << *cp << " " << std::endl;
						// 		*cp++;
						// 	}
						// }
					}
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;
		}
		const PrimitiveConnSharedPtrArray& connections = primitiveDef.getConnections();
		PrimitiveConnSharedPtrArray::const_iterator ccp = connections.begin();
		PrimitiveConnSharedPtrArray::const_iterator cce = connections.end();
		// iterate over the primitive element connections
		while(true && ccp < cce) {
			const PrimitiveConn& connection = *(*ccp++);
			const PrimitiveElementPin& source = *connection.getSourcePtr();
			const PrimitiveElementPinPtrVector& sinks = connection.getSinks();
			std::cout << source.getElementPtr()->getName() << "." << source.getName() << " ==> ";
			PrimitiveElementPinPtrVector::const_iterator epp = sinks.begin();
			PrimitiveElementPinPtrVector::const_iterator epe = sinks.end();
			while(epp < epe) {
				const PrimitiveElementPin& sink = **epp++;
				std::cout << sink.getElementPtr()->getName() << "." << sink.getName() << " ";
			}
			std::cout << std::endl;
		}
		i++;
	}
	BOOST_REQUIRE(true);
	
}
	
BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc

