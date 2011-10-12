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

#ifdef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif //HAVE_CONFIG_H

#ifdef LOGGING
#include <cstdio>
#include <cstdarg>
#endif

#include "torc/generic/util/Log.hpp"

namespace {

#ifdef LOGGING

class Logger {
  public:
    void
    write( const char *fmt, va_list ap );

    void
    openLogFile( const std::string &inFileName );

    Logger();
    ~Logger();

    static Logger *
    instance();

  private:
    FILE *mStream;  
};


void
Logger::write( const char *fmt, va_list ap )
{
    vfprintf( mStream, fmt, ap );
    return;
}

void
Logger::openLogFile( const std::string &inFileName ) {
    if( !inFileName.empty() )
    {
        FILE *fp = fopen( inFileName.c_str(), "w" );
        if( fp )
        {
            if( mStream && mStream != stdout )
            {
                fclose( mStream );
            }
            mStream = fp;
        }
    }
}

Logger::Logger()
    :mStream(stdout) {
}

Logger::~Logger() {
    if( mStream && mStream != stdout )
    {
        fclose( mStream );
		mStream = NULL;
    }
}

Logger *
Logger::instance() {
    static Logger obj;
    return &obj;
}

#endif //LOGGING

}

namespace torc {
namespace generic {
void
openLogFile( const std::string &logFileName )
{
#ifdef LOGGING
    Logger::instance()->openLogFile( logFileName );
#endif
    return;
}

void
log( const char *fmt, ... )
{
#ifdef LOGGING
    va_list args;
    va_start( args, fmt );
    Logger::instance()->write( fmt, args );
    va_end( args );
#endif
    return;
}

} //namespace torc
} //namespace generic

