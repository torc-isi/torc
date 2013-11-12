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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#include <cstdio>

#include "torc/generic/MessageTable.hpp"
#include "torc/generic/Message.hpp"

namespace torc {
namespace generic {

/**
 * Get the message string corresponding to the message inId. This is constructed using the actual
 * message string and the currently set severity of this message.
 *
 * @param[in] inId MessageId object to denote the message.
 */
std::string MessageTable::getMessage(MessageId inId) const {
	MessageContainer::const_iterator msg = mMessages.find(inId);
	if(msg == mMessages.end()) {
		return std::string();
	}
	const Message message((*msg).second);
	char msgString[BUFSIZ];
	int nbytes = 0;
	switch(message.getSeverity()) {
	case eMessageSeveritySuppressed: {
		nbytes = snprintf(msgString, BUFSIZ, "SUPPRESSED ");
		break;
	}
	case eMessageSeverityInfo: {
		nbytes = snprintf(msgString, BUFSIZ, "INFO ");
		break;
	}
	case eMessageSeverityError: {
		nbytes = snprintf(msgString, BUFSIZ, "ERROR ");
		break;
	}
	case eMessageSeverityWarning: {
		nbytes = snprintf(msgString, BUFSIZ, "WARNING ");
		break;
	}
	}
	const std::string& actualMsg = message.getMessage();
	nbytes = snprintf(msgString + nbytes, BUFSIZ - nbytes, "%d : %s", inId, actualMsg.c_str());
	if(nbytes >= BUFSIZ) {
		return std::string();
	}
	return msgString;
}

/**
 * Change the message string for a given Id.
 *
 * @param[in] inId MessageId that needs to be changed
 * @param[in] inMessage New message that needs to be set.
 */
void MessageTable::changeMessageString(MessageId inId, const Message& inMessage) throw (Error) {
	mMessages[inId] = inMessage;
}

/**
 * Change the message severity for a given Id.
 *
 * @param[in] inId MessageId that needs to be changed
 * @param[in] inSeverity New severity that needs to be set.
 */
void MessageTable::changeMessageSeverity(MessageId inId, MessageSeverity inSeverity) {
	MessageContainer::const_iterator msg = mMessages.find(inId);
	if(msg != mMessages.end()) {
		mMessages[inId].setSeverity(inSeverity);
	}
}

/**
 * Get a pointer to the singleton MessageTable object.
 */
MessageTable* MessageTable::instance() {
	static MessageTable singletonObject;
	return &singletonObject;
}

MessageTable::MessageTable() : mMessages() {
	//Populate message table
	mMessages[eMessageIdErrorArrayIndexSizeMismatch] = Message(
		"Array index size does not match array dimensions", eMessageSeverityError);

	mMessages[eMessageIdErrorArrayIndexOutOfBounds] = Message("Array index out of bounds",
		eMessageSeverityError);

	mMessages[eMessageIdErrorEmptyArray] = Message("Empty array", eMessageSeverityError);

	mMessages[eMessageIdErrorNullChildfactory] = Message("Null child factory",
		eMessageSeverityError);

	mMessages[eMessageIdErrorEmptyItemName] = Message("Empty item name", eMessageSeverityError);

	mMessages[eMessageIdErrorItemAlreadyExists] = Message("Item already exists",
		eMessageSeverityError);

	mMessages[eMessageIdErrorItemNotFound] = Message("Item not found", eMessageSeverityError);

	mMessages[eMessageIdErrorPointerToItemDoesNotExist] = Message("Pointer to item does not exist",
		eMessageSeverityError);

	mMessages[eMessageIdErrorItemSizeMismatch] = Message("Item size does not match",
		eMessageSeverityError);

	mMessages[eMessageIdErrorConnectionInvalid] = Message("Provided connection is invalid",
		eMessageSeverityError);

	mMessages[eMessageIdErrorTypeCast] = Message("Type cast not possible", eMessageSeverityError);

	mMessages[eMessageIdErrorValueNotSet] = Message("Value not set", eMessageSeverityError);

	mMessages[eMessageIdErrorCompositionTypeMismatch] = Message("Composition type mismatch",
		eMessageSeverityError);
	mMessages[eMessageIdParserError] = Message("Error from parser", eMessageSeverityError);

	mMessages[eMessageIdParserWarning] = Message("Warning from parser", eMessageSeverityWarning);
	mMessages[eMessageIdErrorNullPointer] = Message("Null Pointer detected", eMessageSeverityError);
	mMessages[eMessageIdErrorUnsupoortedOperation] = Message("Operation is unsupported on class",
		eMessageSeverityError);
}

MessageTable::~MessageTable() throw () {}

} // namespace generic
} // namespace torc
