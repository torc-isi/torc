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

/// \file
/// \brief Source for the XdlImporter class.

#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/xdl/XdlScanner.hpp"
#include <fstream>
#include <sstream>

namespace torc {
namespace physical {

	using namespace std;
	using namespace torc;

	XdlImporter::XdlImporter(void) : mTraceScanning(false), mTraceParsing(false), 
		mDesignName(), mDesignPart(), mDesignDevice(), mDesignPackage(), mDesignSpeedGrade(), 
		mDesignXdlVersion(), mDesignPtr(), mCircuitPtr(), mModuleCount(0), mModuleName(""), 
		mModuleAnchor(""), mModuleInstance(""), mModulePtr(), mReferenceInstantiation(""), 
		mReferenceModule(""), mReferenceInstance(""), mReferenceModulePtr(), 
		mReferenceInstancePtr(), mInstanceReferencePtr(), mPortName(""), mPortInstance(""), 
		mPortPin(""), mInstanceCount(0), mInstanceName(), mInstanceType(""), mInstanceSite(""), 
		mInstanceTile(""), mInstanceBonding(eInstanceBondingUnknown), mNetCount(0), 
		mNetType(eNetTypeNormal), mNetName(), mPinInstance(""), mPinName(""), 
		mPinDirection(ePinDirectionUnknown), mPipTile(""), mPipSource(""), mPipSink(""), 
		mPipDirection(static_cast<EPipDirection>(0)), mRoutethroughConfigSetting(), 
		mRoutethroughConfigName(), mRoutethroughConfigValue(), mRoutethroughInstance(),  
		mRoutethroughSource(""), mRoutethroughSink(""), mConfigCount(0), mConfigSetting(), 
		mConfigName(), mConfigValue() {
		// assume success unless we encounter an error
		mSuccess = true;
	}

	bool XdlImporter::operator()(istream& in, const string& name) {
		mStreamName = name;

		XdlScanner scanner(&in);
		scanner.set_debug(mTraceScanning);
		this->lexer = &scanner;

		XdlParser parser(*this);
		parser.set_debug_level(mTraceParsing);
		bool result = parser.parse() == 0;
		return mSuccess && result;
	}

	bool XdlImporter::operator()(const boost::filesystem::path& path) {
		string pathname = path.string();
		ifstream in(pathname.c_str());
		if (!in.good()) return false;
		return (*this)(in, pathname);
	}

	bool XdlImporter::operator()(const string &input, const string& name) {
		istringstream iss(input);
		return (*this)(iss, name);
	}

	void XdlImporter::error(const location& l, const string& m) {
		failure();
		cerr << l << ": " << m << std::endl;
	}

	void XdlImporter::error(const string& m) {
		failure();
		cerr << m << std::endl;
	}

} // namespace physical
} // namespace torc

//namespace {
//	/// \brief Suppress GCC warnings about unused boost global variables.
//	void suppress_boost_gcc_warnings(void);
//	void suppress_boost_gcc_warnings(void) {
//		(void) boost::system::system_category;
//		(void) boost::system::generic_category;
//		(void) boost::system::posix_category;
//		(void) boost::system::errno_ecat;
//		(void) boost::system::native_ecat;
//	}
//}
