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

#ifndef TORC_GENERIC_UTIL_MESSAGE_HPP
#define TORC_GENERIC_UTIL_MESSAGE_HPP

#include <string>
#include "torc/generic/util/MessageSeverity.hpp"

namespace torc {

namespace generic {

/**
 * The actual message object. It consists of the actual message string and the currently set severity of this message.
 */
class Message
{
  public:
    inline const std::string
    getMessage() const throw();

    void
    setMessage(const std::string & inSource) throw();

    inline const MessageSeverity
    getSeverity() const throw();

    void
    setSeverity(const MessageSeverity & inSource) throw();

    Message();

    Message( const std::string &inMessage,
            MessageSeverity inSeverity );

    ~Message() throw();

    Message(const Message &inSource);

    Message &
    operator=(const Message &inSource) throw();

  private:
    std::string mMessage;
    MessageSeverity mSeverity;

};
inline const std::string
Message::getMessage() const throw() {
  return mMessage;
}

inline const MessageSeverity
Message::getSeverity() const throw() {
  return mSeverity;
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_UTIL_MESSAGE_HPP
