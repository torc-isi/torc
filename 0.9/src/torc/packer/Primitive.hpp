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
/// \brief Header for the Primitive class.

#ifndef TORC_PHYSICAL_PRIMITIVE_HPP
#define TORC_PHYSICAL_PRIMITIVE_HPP

#include "torc/packer/Component.hpp"
#include "torc/packer/Element.hpp"


#include <string>

namespace torc {
namespace physical {

/// \brief primitive.

class Primitive : public Component {

// friends
/// \brief The Factory class has direct access to our internals.
friend class RcFactory;

protected:
// types
/// \brief Imported type name.
typedef std::string string;
// members
/// \brief Vector of element shared pointers.
ElementSharedPtrVector mElements;

// constructors
Primitive(const string& inName) : Component(inName){}   
public:
	
// types
/// \brief Constant iterator for Element shared pointers.
typedef ElementSharedPtrVector::const_iterator ElementSharedPtrConstIterator;
/// \brief Non-constant iterator for Element shared pointers.
typedef ElementSharedPtrVector::iterator ElementSharedPtrIterator;
// functions
/// \brief Find a primitive element by name.
/// \param inName The element name to look for.
/// \returns an iterator for the specified element, or elementsEnd() if the name was not 
///     found.
ElementSharedPtrIterator findElement(const string& inName) {
NameComparator predicate(inName);
return std::find_if(elementsBegin(), elementsEnd(), predicate);
}
/// \brief Add a element to the primitive.
/// \param inElementPtr The element to add.
/// \returns true if the element was added, or false if a element with the same name already 
///     exists in the primitive.
bool addElement(ElementSharedPtr& inElementPtr) {
/// \todo Acquire mutex.
ElementSharedPtrVector::iterator e = mElements.end();
ElementSharedPtrVector::iterator result = findElement(inElementPtr->getName());
if(result == e) mElements.push_back(inElementPtr);
return result == e; // return true if we added the element
/// \todo Release mutex.
}
/// \brief Remove a element from the primitive.
/// \param inElementPtr The element to remove.
/// \returns true if the element was remove, or false if the element did not exist.
bool removeElement(ElementSharedPtr& inElementPtr) {
/// \todo Acquire mutex.
ElementSharedPtrVector::iterator e = mElements.end();
ElementSharedPtrVector::iterator result = std::find(mElements.begin(), e, inElementPtr);
if(result == e) return false;

//adjust connections

			ConnectionPinVector sinkPins;
			ConnectionPinVector sourcePins;
			ConnectionSharedPtrVector sourceConnections;
			ConnectionSharedPtrVector sinkConnections;
			ElementSharedPtrVector sourceElements;
			ElementSharedPtrVector sinkElements;
			
			Connection::ConnectionPinSharedPtrIterator pinIt, pinIt2;
			Element::ConnectionSharedPtrIterator cnIt, cnIt2; 
			
			ConnectionPin pin;
			
			ConnectionSharedPtr cnPtr;
			ElementSharedPtr elemPtr;
			for( cnIt = inElementPtr->connectionsBegin(); cnIt != inElementPtr->connectionsEnd(); ++cnIt){
				if( (*cnIt)->getSource()->getElementName() == inElementPtr->getName() ){
					pin = *(*cnIt)->getSink();
					sinkPins.push_back(pin);
					elemPtr = *findElement(pin.getElementName());
					sinkElements.push_back(elemPtr);
					for(cnIt2 = elemPtr->connectionsBegin(); cnIt2 != elemPtr->connectionsEnd(); ++cnIt2){
						if( (*cnIt2)->getSink()->getPinName() == pin.getPinName()){
							sinkConnections.push_back(*cnIt2);
							elemPtr->removeConnection(cnIt2);
							break;
						}
					}	
				}
				else{
					pin = *(*cnIt)->getSource();
					sourcePins.push_back(pin);
					elemPtr = *findElement(pin.getElementName());
					sourceElements.push_back(elemPtr);
					for(cnIt2 = elemPtr->connectionsBegin(); cnIt2 != elemPtr->connectionsEnd(); ++cnIt2){
						if( (*cnIt2)->getSource()->getPinName() == pin.getPinName()){
							sourceConnections.push_back(*cnIt2);
							elemPtr->removeConnection(cnIt2);
							break;
						}
					}	
				}
			}
			for(size_t i=0; i<sourcePins.size(); ++i){
				for(size_t j=0; j<sinkPins.size(); ++j){
					ConnectionSharedPtr connectionPtr(new Connection(sourcePins[i].getPinName()));
					ConnectionSharedPtr connectionPtr1(new Connection(sourcePins[i].getPinName()));
					connectionPtr->addConnectionPin(sourcePins[i]);
					connectionPtr->addConnectionPin(sinkPins[j]);
					sinkElements[j]->addConnection(connectionPtr);
					
					connectionPtr1->addConnectionPin(sourcePins[i]);
					connectionPtr1->addConnectionPin(sinkPins[j]);
					sourceElements[i]->addConnection(connectionPtr1);
		
				}
			}
			mElements.erase(result);
return true;
}

// operators
	         /// \brief Equality operator.
	         /// \details This function deems elements equal if their names are identical.
	         /// \param rhs The element to compare against.
	         /// \returns true if both element names are identical, or false otherwise.
	         bool operator ==(const Primitive& rhs) const { return mName == rhs.mName; }
		
		
// accessors

size_t getElementCount(void) const { return mElements.size(); }
// iterators
/// \brief Returns the begin constant iterator for elements.
ElementSharedPtrConstIterator elementsBegin(void) const { return mElements.begin(); }
/// \brief Returns the end constant iterator for elements.
ElementSharedPtrConstIterator elementsEnd(void) const { return mElements.end(); }
/// \brief Returns the begin non-constant iterator for elements.
ElementSharedPtrIterator elementsBegin(void) { return mElements.begin(); }
/// \brief Returns the end non-constant iterator for elements.
ElementSharedPtrIterator elementsEnd(void) { return mElements.end(); }
};

/// \brief Shared pointer encapsulation of a Primitive.
typedef boost::shared_ptr<Primitive> PrimitiveSharedPtr;

/// \brief Vector of Primitive shared pointers.
typedef std::vector<PrimitiveSharedPtr> PrimitiveSharedPtrVector;
	


} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_PRIMITIVE_HPP
