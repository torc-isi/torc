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

#ifndef TORC_GENERIC_MESSAGEID_HPP
#define TORC_GENERIC_MESSAGEID_HPP

namespace torc {
namespace generic {

/**
 * @enum MessageId
 * 
 * Each element of this enumeration denote a message in the system.
 */
enum MessageId {
	eMessageIdErrorArrayIndexSizeMismatch, eMessageIdErrorArrayIndexOutOfBounds,
	eMessageIdErrorEmptyArray, eMessageIdErrorNullChildfactory, eMessageIdErrorEmptyItemName,
	eMessageIdErrorItemAlreadyExists, eMessageIdErrorItemNotFound,
	eMessageIdErrorPointerToItemDoesNotExist, eMessageIdErrorItemSizeMismatch,
	eMessageIdErrorConnectionInvalid, eMessageIdErrorTypeCast, eMessageIdErrorValueNotSet,
	eMessageIdErrorCompositionTypeMismatch, eMessageIdParserError, eMessageIdParserWarning,
	eMessageIdErrorNullPointer, eMessageIdErrorUnsupoortedOperation
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_MESSAGEID_HPP
