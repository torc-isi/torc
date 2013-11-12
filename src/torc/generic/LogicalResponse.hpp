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

#ifndef TORC_GENERIC_LOGICALRESPONSE_HPP
#define TORC_GENERIC_LOGICALRESPONSE_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/FactoryType.hpp"

namespace torc { namespace generic { class Port; } }
namespace torc { namespace generic { class PortList; } }
namespace torc { namespace generic { class PortListAlias; } }
namespace torc { namespace generic { class LogicElement; } }

namespace torc {
namespace generic {

/**
 * @brief This class is used to model logicInput/logicOutput construct.
 * This class holds information of logical response to be expected from
 * a ports during simulation.
 */
class LogicalResponse : public Visitable, public SelfReferencing<LogicalResponse> {
	friend class FactoryType<LogicalResponse> ;

public:
	/**
	 * @enum ResponseType
	 * Logical response types (logicInput/logicOutput)
	 */
	enum ResponseType {
		eResponseTypeInput = 0, eResponseTypeOutput
	};

	/**
	 * Get the Logical response type
	 *
	 * @return Logical response type
	 */
	inline const ResponseType getResponseType() const;

	/**
	 * Set the Logical response type
	 *
	 * @param[in] inSource Logical response type
	 */
	void setResponseType(const ResponseType& inSource);

	/**
	 * Convenience class to visit LogicalResponse.
	 */
	typedef VisitorType<LogicalResponse> Visitor;

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * Convenience class to create LogicalResponse.
	 */
	class Factory : public FactoryType<LogicalResponse> {
	public:
		using FactoryType<LogicalResponse>::create;
		/**
		 * Create a logical response.
		 *
		 * @param[in] inResponseType Response type.
		 * @param[in] inPort Connected Port to this logical response.
		 * @param[in] inLogicWaveForm Logic wave form for this logical response.
		 * @param[in] inPortListAlias Connected Port list alias to this logical response [optional].
		 *
		 * @return Pointer to created logical response.
		 */
		virtual LogicalResponseSharedPtr newLogicalResponsePtr(const ResponseType& inResponseType,
			const PortSharedPtr& inPort, const LogicElementSharedPtr& inLogicWaveForm,
			const PortListAliasSharedPtr& inPortListAlias = PortListAliasSharedPtr()) throw (Error);
	};

	/**
	 * Get the connected port shared pointer.
	 *
	 * @return Connected port shared pointer.
	 */
	inline const PortSharedPtr getConnectedPort() const;

	/**
	 * Set the connected port shared pointer.
	 *
	 * @param[in] inPort Connected port shared pointer.
	 */
	void setConnectedPort(const PortSharedPtr& inPort);

	/**
	 * Get the connected port list shared pointer.
	 *
	 * @return Connected port list shared pointer.
	 */
	inline const PortListSharedPtr getConnectedPortList() const;

	/**
	 * Set the connected port list shared pointer.
	 *
	 * @param[in] inPortList Connected port list shared pointer.
	 */
	void setConnectedPortList(const PortListSharedPtr& inPortList);

	/**
	 * Get the connected port list alias shared pointer.
	 *
	 * @return Connected port list alias shared pointer.
	 */
	inline const PortListAliasSharedPtr getConnectedPortListAlias() const;

	/**
	 * Set the connected port list alias shared pointer.
	 *
	 * @param[in] inPortListAlias Connected port list alias shared pointer.
	 */
	void setConnectedPortListAlias(const PortListAliasSharedPtr& inPortListAlias);

	/**
	 * Get the logic wave form for this response.
	 *
	 * @return LogicElementSharedPtr Pointer to logic element object.
	 */
	inline const LogicElementSharedPtr getLogicWaveForm() const;

	/**
	 * Set the logic wave form for this response.
	 *
	 * @param[in] inSource Pointer to logic element object.
	 */
	void setLogicWaveForm(const LogicElementSharedPtr& inSource);

	~LogicalResponse() throw ();

protected:
	LogicalResponse();

private:
	LogicalResponse(const LogicalResponse& source);

	LogicalResponse& operator=(const LogicalResponse& source);

	ResponseType mResponseType;
	PortSharedPtr mConnectedPort;
	PortListSharedPtr mConnectedPortList;
	PortListAliasSharedPtr mConnectedPortListAlias;
	LogicElementSharedPtr mLogicWaveForm;

};

/**
 * Get the Logical response type
 *
 * @return Logical response type
 */
inline const LogicalResponse::ResponseType LogicalResponse::getResponseType() const {
	return mResponseType;
}

/**
 * Get the connected port shared pointer.
 *
 * @return Connected port shared pointer.
 */
inline const PortSharedPtr LogicalResponse::getConnectedPort() const {
	return mConnectedPort;
}

/**
 * Get the connected port list shared pointer.
 *
 * @return Connected port list shared pointer.
 */
inline const PortListSharedPtr LogicalResponse::getConnectedPortList() const {
	return mConnectedPortList;
}

/**
 * Get the connected port list alias shared pointer.
 *
 * @return Connected port list alias shared pointer.
 */
inline const PortListAliasSharedPtr LogicalResponse::getConnectedPortListAlias() const {
	return mConnectedPortListAlias;
}

/**
 * Get the logic wave form for this response.
 *
 * @return LogicElementSharedPtr Pointer to logic element object.
 */
inline const LogicElementSharedPtr LogicalResponse::getLogicWaveForm() const {
	return mLogicWaveForm;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_LOGICALRESPONSE_HPP
