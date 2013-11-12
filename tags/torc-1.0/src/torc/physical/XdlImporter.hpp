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

/// \file
/// \brief Header for the XdlImporter class.

#ifndef TORC_PHYSICAL_XDLIMPORTER_HPP
#define TORC_PHYSICAL_XDLIMPORTER_HPP

// The foundation of this code comes from Timo Bingmann's Flex Bison C++ Template/Example, 
// available at http://idlebox.net/2007/flex-bison-cpp-example.

#include "torc/physical/Design.hpp"
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

// forward declarations of friends outside our namespace.
namespace torc {
	class XdlParser;
	class XdlScanner;
	class location;
} // torc

namespace torc {
namespace physical {

namespace physical { class XdlImporterUnitTest; }

	/// \brief Importer from XDL format into a physical design.
	/// \details The XdlImporter creates the XdlParser and XdlScanner classes and connects them.  
	///		The input stream is then fed into the scanner object, which delivers a sequence of 
	///		tokens to the parser.  The importer is also available as a parameter to the grammar 
	///		rules.
	class XdlImporter {

	// friends
		/// \brief The unit test class has access to our internals.
		friend class torc::physical::physical::XdlImporterUnitTest;

	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Imported type name.
		typedef std::istream istream;

	public:
	// constructors
		/// \brief Construct the parser importer context.
		XdlImporter(void);
		/// \brief Virtual destructor.
		virtual ~XdlImporter(void) {}

	// members
		/// \brief Enable debug output in the flex scanner.
		bool mTraceScanning;
		/// \brief Enable debug output in the bison parser.
		bool mTraceParsing;
		/// \brief Name of file or input stream for error messages.
		string mStreamName;
		/// \brief Pointer to the current lexer instance.
		/// \details This serves to connect the parser to the scanner, and is used by the yylex 
		///		macro.
		class torc::XdlScanner* lexer;

	// macros
		/// \cond OMIT_FROM_DOXYGEN
		// Doxygen gets confused by the explicit "__attribute__ ((deprecated))" so we used this
		#define DEPRECATED __attribute__ ((deprecated))
		/// \endcond

	// operators
		/// \brief Import XDL from an input stream.
		/// \param in Input stream.
		/// \param name Stream name to use for error messages.
		/// \returns true if successfully parsed.
		/// \deprecated Please use operator()(...) instead of import(...).  i.e. importer(stream, 
		///		name);
		DEPRECATED bool import(istream& in, const string& name = "stream input") 
			{ return (*this)(in, name); }
		/// \brief Import XDL from an input stream.
		/// \param in Input stream.
		/// \param name Stream name to use for error messages.
		/// \returns true if successfully parsed.
		bool operator()(istream& in, const string& name = "stream input");

		/// \brief Import XDL from a string.
		/// \param input Input stream.
		/// \param name Stream name to use for error messages.
		/// \returns true if successfully parsed.
		/// \deprecated Please use operator()(...) instead of import(...).  i.e. importer(input, 
		///		name);
		DEPRECATED bool import(const string& input, const string& name = "string stream") 
			{ return (*this)(input, name); }
		/// \brief Import XDL from a string.
		/// \param input Input stream.
		/// \param name Stream name to use for error messages.
		/// \returns true if successfully parsed.
		bool operator()(const string& input, const string& name = "string stream");

		/// \brief Import XDL from a file.
		/// \param filename Input file name.
		/// \returns true if successfully parsed.
		/// \deprecated Please use operator()(...) instead of import(...).  i.e. importer(filename);
		DEPRECATED bool import(const boost::filesystem::path& filename) 
			{ return (*this)(filename); }
		/// \brief Import XDL from a file.
		/// \param filename Input file name.
		/// \returns true if successfully parsed.
		bool operator()(const boost::filesystem::path& filename);

	// functions
		// To demonstrate pure handling of parse errors, instead of
		// simply dumping them on the standard error output, we will pass
		// them to the importer using the following two member functions.
		/// \brief Error handling with associated line number.
		/// \details This can be modified to output the error in another manner, for example to a 
		///		dialog box.
		void error(const location& l, const string& m);
		/// \brief General error handling.
		/// \details This can be modified to output the error in another manner, for example to a 
		///		dialog box.
		void error(const string& m);
		/// \brief Signals a parsing failure by deasserting the success flag.
		void failure(void) { mSuccess = false; }

	// accessors
		/// \brief Returns a shared pointer for the design.
		/// \details This design is created and populated during XDL import process.
		DesignSharedPtr getDesignPtr(void) { return mDesignPtr; }

	protected:
	// friends
		/// \brief The XdlParse has access to our members.
		friend class torc::XdlParser;

	// enums
		/// \brief The pip type, either regular or routethrough.
		enum EPipType { ePipTypeRegular = 0, ePipTypeRoutethrough };

	// members
		// error flags
		bool mSuccess; ///< Flag signaling parsing success.

		// design variables
		string mDesignName; ///< Name of the design.
		string mDesignPart; ///< Device, package, and speed grade specified for the design.
		string mDesignDevice; ///< Device specified for the design.
		string mDesignPackage; ///< Package specified for the design.
		string mDesignSpeedGrade; ///< Speed grade specified for the design.
		string mDesignXdlVersion; ///< XDL version read in by the design.
		DesignSharedPtr mDesignPtr; ///< Shared pointer to the design.
		CircuitWeakPtr mCircuitPtr; ///< Weak pointer to the circuit.

		// module variables
		boost::uint32_t mModuleCount; ///< Number of modules read for the design.
		string mModuleName; ///< Name of the module.
		string mModuleAnchor; ///< Module anchor instance name.
		string mModuleInstance; ///< Name of instantiating instance for module reference.
		ModuleSharedPtr mModulePtr; ///< Shared pointer to the current module.

		// instance reference variables
		string mReferenceInstantiation; ///< Name under which the module was instantiated.
		string mReferenceModule; ///< Name of the referenced module.
		string mReferenceInstance; ///< Name of the referenced instance.
		ModuleSharedPtr mReferenceModulePtr; ///< Shared pointer to the referenced module.
		InstanceSharedPtr mReferenceInstancePtr; ///< Shared pointer to the referenced instance.
		InstanceReferenceSharedPtr mInstanceReferencePtr; ///< Shared pointer to instance reference.

		// port variables
		string mPortName; ///< Name of the port.
		string mPortInstance; ///< Current port instance name.
		string mPortPin; ///< Current port pin name.
		PortTempVector mPortTempVector; ///< Vector of ports not yet mapped to instances.

		// instance variables
		boost::uint32_t mInstanceCount; ///< Number of instances read for the design.
		string mInstanceName; ///< Current instance name.
		TileTypeName mInstanceType; ///< Current instance type.
		SiteName mInstanceSite; ///< Current instance site name.
		TileName mInstanceTile; ///< Current instance tile name.
		torc::physical::EInstanceBonding mInstanceBonding; ///< Current instance bonding.
		InstanceSharedPtr mInstancePtr; ///< Shared pointer to the current instance.

		// net variables
		boost::uint32_t mNetCount; ///< Number of nets read for the design.
		torc::physical::ENetType mNetType; ///< Current net type.
		string mNetName; ///< Current net name.
		NetSharedPtr mNetPtr; ///< Shared pointer to the current net.

		// pin variables
		InstanceName mPinInstance; ///< Current pin instance name.
		PinName mPinName; ///< Current pin name.
		torc::physical::EPinDirection mPinDirection; ///< Current pin direction.

		// pip variables
		TileName mPipTile; ///< Current pip tile name.
		WireName mPipSource; ///< Current pip source wire name.
		WireName mPipSink; ///< Current pip sink wire name.
		torc::physical::EPipDirection mPipDirection; ///< Current pip directionality.

		// routethrough variables
		string mRoutethroughConfigSetting; ///< Current routethrough config setting.
		string mRoutethroughConfigName; ///< Current routethrough config name.
		string mRoutethroughConfigValue; ///< Current routethrough config value.
		string mRoutethroughInstance; ///< Current routethrough Instance name.
		WireName mRoutethroughSource; ///< Current routethrough source wire name.
		WireName mRoutethroughSink; ///< Current routethrough sink wire name.
		
		// config variables
		boost::uint32_t mConfigCount; ///< Number of configurations read for the design.
		ConfigMap mConfigMap; ///< Current config map.
		string mConfigSetting; ///< Current config setting.
		string mConfigName; ///< Current config name.
		string mConfigValue; ///< Current config value.

	// functions
		/// \brief Initialize the database if applicable.
		/// \details Only the torc::architecture::XdlImporter subclass actually initializes the 
		///		database.
		virtual void initializeDatabase(void) {}
		/// \brief Bind the given instance pin to its database Tilewire if applicable.
		virtual void bind(torc::physical::InstancePinSharedPtr&) {}
		/// \brief Bind the given pip to the database arc and wire usage if applicable.
		virtual void bind(torc::physical::Pip&, EPipType) {}
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_XDLIMPORTER_HPP
