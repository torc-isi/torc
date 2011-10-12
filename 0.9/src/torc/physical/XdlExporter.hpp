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
/// \brief Header for the XdlExport class.

#ifndef TORC_PHYSICAL_XDL_EXPORTER_HPP
#define TORC_PHYSICAL_XDL_EXPORTER_HPP

#include "torc/physical/Design.hpp"
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <string>

namespace torc {
namespace physical {

	/// \brief Physical design exporter for XDL.
	class XdlExporter {
	// members
		/// \brief The output stream.
		std::ostream& mStream;
		/// \brief The indent count.
		int mIndentCount;
		/// \brief The indent string.
		std::string mIndentString;
	// functions
		/// \brief Indent the current line.
		void indent(void) const 
			{ int count = mIndentCount; while(count--) mStream << mIndentString; }
	public:
	// constructors
		/// \brief Public constructor.
		/// \param inStream The output stream.
		/// \param inIndentString The indent string.  The default parameter uses a single tab.
		XdlExporter(std::ostream& inStream, const std::string& inIndentString = "\t") 
			: mStream(inStream), mIndentCount(0), mIndentString(inIndentString) {}
	// operators
		/// \brief Top level design exporter operator.
		void operator() (const DesignSharedPtr& inDesignPtr);
	// functions
		/// \brief Writes the given Circuit.
		void write(const Circuit& circuit);
		/// \brief Writes the given Design.
		void write(const Design& design);
		/// \brief Writes the given Module.
		void write(const Module& module);
		/// \brief Writes the given Port.
		void write(const Port& port);
		/// \brief Writes the given Instance.
		void write(const Instance& instance);
		/// \brief Writes the given Net.
		void write(const Net& net);
		/// \brief Writes the given InstancePin, with an optional trailing comma.
		void write(const InstancePin& instancePin, EPinDirection pinDirection, bool comma);
		/// \brief Writes the given pip, with an options trailing comma.
		void write(const Pip& pip, bool comma);
		/// \brief Writes the given Routethrough.
		void write(const Routethrough& routethrough);
		/// \brief Writes the given ConfigMap.
		void write(const ConfigMap& configMap);
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_XDL_EXPORTER_HPP
