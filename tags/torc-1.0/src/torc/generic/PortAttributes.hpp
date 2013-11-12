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

#ifndef TORC_GENERIC_PORTATTRIBUTES_HPP
#define TORC_GENERIC_PORTATTRIBUTES_HPP

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/PortDelay.hpp"
#include "torc/generic/Value.hpp"

namespace torc {
namespace generic {

/**
 * @brief Represents attributes of a Port or PortReference object
 * 
 * The PortAttributes class stores some optional attributes of a Port or overridden values in a
 * PortReference object. The values can be set from an EDIF file using the (port ...),
 * (portBackAnnotate ...) or the (portInstance ...) constructs. The main logic of bringing this out
 * of the Port/PortReference objects is to save space.
 */
class PortAttributes {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
public:
	inline const Value getUnused() const;

	void setUnused(const Value& inSource);

	inline const Value getDesignator() const;

	void setDesignator(const Value& inSource);

	inline const Value getAcLoad() const;

	void setAcLoad(const Value& inSource);

	inline const PortDelay getPortDelay() const;

	inline const bool getIsPortDelaySet() const;

	void setPortDelay(const PortDelay& inSource);

	inline const Value getDcFaninLoad() const;

	void setDcFaninLoad(const Value& inSource);

	inline const Value getDcFanoutLoad() const;

	void setDcFanoutLoad(const Value& inSource);

	inline const Value getDcMaxFanin() const;

	void setDcMaxFanin(const Value& inSource);

	inline const Value getDcMaxFanout() const;

	void setDcMaxFanout(const Value& inSource);

	PortAttributes();

	~PortAttributes() throw ();

	PortAttributes(const PortAttributes& inSource);

	PortAttributes& operator=(const PortAttributes& inSource);

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	Value mUnused;
	Value mDesignator;
	Value mDcFaninLoad;
	Value mDcFanoutLoad;
	Value mDcMaxFanin;
	Value mDcMaxFanout;
	Value mAcLoad;
	PortDelay mPortDelay;
	bool mPortDelaySet;
};

inline const Value PortAttributes::getUnused() const {
	return mUnused;
}

inline const Value PortAttributes::getDesignator() const {
	return mDesignator;
}

inline const Value PortAttributes::getAcLoad() const {
	return mAcLoad;
}

inline const PortDelay PortAttributes::getPortDelay() const {
	return mPortDelay;
}

inline const bool PortAttributes::getIsPortDelaySet() const {
	return mPortDelaySet;
}

inline const Value PortAttributes::getDcFaninLoad() const {
	return mDcFaninLoad;
}

inline const Value PortAttributes::getDcFanoutLoad() const {
	return mDcFanoutLoad;
}

inline const Value PortAttributes::getDcMaxFanin() const {
	return mDcMaxFanin;
}

inline const Value PortAttributes::getDcMaxFanout() const {
	return mDcMaxFanout;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_PORTATTRIBUTES_HPP
