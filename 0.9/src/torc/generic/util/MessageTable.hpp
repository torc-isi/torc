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

#ifndef TORC_GENERIC_MESSAGE_TABLE_HPP
#define TORC_GENERIC_MESSAGE_TABLE_HPP


#include "torc/generic/util/MessageId.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/util/MessageSeverity.hpp"

namespace torc { namespace generic { class Message; }  }

namespace torc {

namespace generic {

/**
 * Table of messages for this library. This is used for retreiving a message for a given MessageId.
 * @note getMessage method is a read only method, and as such it is thread-safe. However, the changeMessageSeverity() and changeMessageSeverity() are not thread-safe. Typically they are not meant to be used in a multi-threaded context. If required however, for some reason, they need to be protected by proper locking contexts
 */
class MessageTable
{
  public:
    typedef std::map<MessageId,Message> MessageContainer;

  public:
    /**
     * Get the message string corresponding to the message id. This is constructed using the actual message string and the currently set severity of this message.
     *
     * @param[in] inId MessageId object to denote the message.
     * @return Formatted message string. Empty string is returned if there was some error.
     */
    std::string
    getMessage(MessageId inId) const throw();

    /**
     * Change the message string for a given Id.
     *
     * @param[in] inId MessageId that needs to be changed
     * @param[in] inMessage New message that needs to be set.
     */
    void
    changeMessageString(MessageId inId,
            const Message & inMessage) throw(Error);

    /**
     * Change the message severity for a given Id.
     *
     * @param[in] inId MessageId that needs to be changed
     * @param[in] inSeverity New severity that needs to be set.
     */
    void changeMessageSeverity(MessageId inId,
                    MessageSeverity inSeverity) throw();

    /**
     * Get a pointer to the singleton MessageTable object.
     */
    static MessageTable *
    instance();


  private:
    explicit MessageTable();


  public:
    ~MessageTable() throw();


  private:
    MessageTable(const MessageTable & source);

    MessageTable & operator=(
                const MessageTable & source) throw();

    void
    operator delete( void *); //block

  private:
    MessageContainer mMessages;
};

} // namespace torc::generic

} // namespace torc
#endif
