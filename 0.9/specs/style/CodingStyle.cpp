// TORC - Copyright 2010 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

#include "CodingStyle.hpp"

namespace torc {

	/// \brief String representation of pip directions.
	const char* CodingStyle::sPipDirectionStrings[ePipDirectionCount] = {
		"==", // sPipBidirectionalUnbufferedString = 0,
		"=>", // sPipBidirectionalUnidirectionallyBufferedString,
		"=-", // sPipBidirectionalBidirectionallyBufferedString,
		"->"  // sPipUnidirectionalBufferedString
	};

	const char* CodingStyle::getPipDirectionString(void) const {
		return sPipDirectionStrings[mPipDirection];
	}

	/// \details Braces follow the K&R convention, with exceptions permitted when appropriate.
	void CodingStyle::sampleFunction(void) {

		// descriptive names are preferred in general
		uint32_t someDescriptiveName = 0;

		// loop counters of limited scope can follow traditional C naming conventions (i, j, k, ...)
		for(uint16_t i = 0; i < 1024; i++) {
			// do something
			someDescriptiveName++;
		}

		// booleans are very convenient in C++
		bool flag = true;
		while(flag) {
			// compact state machines can be coded with cases on single lines when that improves 
			// readability
			switch(mState) {
			case eStateNull: if(someDescriptiveName-- != 0) { mState = eStateOne; } break;
			case eStateOne: /* do something; */ mState = mPrivate ? eStateTwo : eStateThree; break;
			case eStateTwo: /* do something; */ mState = eStateThree; break;
			case eStateThree: /* do something; */ mState = eStateFour; break;
			case eStateSpecial:
				someDescriptiveName++;
				mPrivate = !mPrivate;
				// explicit fallthrough
			case eStateFour: /* do something; */ mState = eStateOne; break;
			default: mState = eStateNull; break;
			}
		}

		// complex if-else structures can be written like this (blank lines not mandatory)
		if(mPrivate) {
			// do a whole bunch of stuff

		// description of next condition
		} else if(someDescriptiveName < 10) {
			// do a bunch more stuff

		// description of next condition
		} else if(someDescriptiveName > 1024) {
			// do yet more stuff

		// description of default
		} else {
			// perform some default behavior

		}

	}

} // namespace torc
