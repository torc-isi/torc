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

#ifndef TORC_GENERIC_PORTELEMENT_HPP
#define TORC_GENERIC_PORTELEMENT_HPP

#include "torc/generic/PointerTypes.hpp"

namespace torc { namespace generic { class Port; } }
namespace torc { namespace generic { class PortReference; } }

namespace torc {
namespace generic {

/**
 * @brief Represents port element like port or port reference.
 *
 */

class PortElement {
public:
	/**
	 * @enum ElementType
	 * Port element type
	 */
	enum ElementType {
		eElementTypePort, eElementTypePortReference
	};

	/**
	 * Get the port element type
	 *
	 * @return port element type
	 */
	inline const ElementType getType() const;

	/**
	 * Set the port element type
	 *
	 * @param[in] Pointer to port element type
	 */
	void setType(const ElementType& inSource);

	/**
	 * Get the port from the composition
	 *
	 * @return Pointer to port from the composition
	 */
	inline const PortSharedPtr getPort() const;

	/**
	 * Get the port reference from the composition
	 *
	 * @return Pointer to port reference from the composition
	 */
	inline const PortReferenceSharedPtr getPortReference() const;

	PortElement();
	PortElement(const PortSharedPtr& inPort);
	PortElement(const PortReferenceSharedPtr& inPortRef);

	PortElement(const PortElement& inRhs);

	PortElement& operator =(const PortElement& inRhs);

	~PortElement() throw ();

private:
	ElementType mType;
	PortSharedPtr mPort;
	PortReferenceSharedPtr mPortReference;
};

inline const PortElement::ElementType PortElement::getType() const {
	return mType;
}

inline const PortSharedPtr PortElement::getPort() const {
	return mPort;
}

inline const PortReferenceSharedPtr PortElement::getPortReference() const {
	return mPortReference;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_PORTELEMENT_HPP
