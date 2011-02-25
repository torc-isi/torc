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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#include "torc/generic/util/Message.hpp"

namespace torc {

namespace generic {

void
Message::setMessage(const std::string &inSource) throw() {
  mMessage = inSource;
}

void
Message::setSeverity(
    const MessageSeverity & inSource) throw() {
  mSeverity = inSource;
}

Message::Message()
    :mMessage(),
    mSeverity( eMessageSeveritySuppressed ) {
}

Message::Message( const std::string &inMessage,
            MessageSeverity inSeverity )
    :mMessage( inMessage ),
    mSeverity( inSeverity ) {
}

Message::~Message() throw() {
}

Message::Message(const Message & source)
    :mMessage( source.mMessage ),
    mSeverity( source.mSeverity ) {
}

Message &
Message::operator=(const Message & source) throw() {
    if( this != &source )
    {
        mMessage = source.mMessage;
        mSeverity = source.mSeverity;
    }
    return *this;
}


} // namespace torc::generic

} // namespace torc
