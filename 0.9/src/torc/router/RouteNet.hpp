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
/// \brief Header for the Net class.

#ifndef TORC_ROUTER_ROUTNET_HPP
#define TORC_ROUTER_ROUTNET_HPP

#include "torc/architecture/Tilewire.hpp"
#include "torc/architecture/Arc.hpp"
#include "torc/router/RouteNode.hpp"
#include <string>
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>

namespace torc {
namespace router {

	enum ERoutePropertyType {
		eRouteTime = 0,
		eRoutePropertyTypeCount
	};

	/// \brief Router net.
	/// \details The RouteNet class contains architecture specific sources and sinks.
	class RouteNet {
	// types
		/// Imported type names
		typedef std::string string;
		typedef architecture::Tilewire Tilewire;
		typedef architecture::TilewireVector TilewireVector;
		typedef architecture::Arc Arc;
		typedef architecture::ArcVector ArcVector;
	public:
		typedef ArcVector::const_iterator ArcConstIterator;
		typedef ArcVector::iterator ArcIterator;
		typedef TilewireVector::const_iterator TilewireConstIterator;
		typedef TilewireVector::iterator TilewireIterator;
		typedef RouteNodePtrVector::const_iterator RouteNodePtrConstIterator;
		typedef RouteNodePtrVector::iterator RouteNodePtrIterator;
		
	protected:
	// members
		/// \brief Name of the net.
		string mName;
		/// \brief Vector of net source Tilewires.
		TilewireVector mSources;
		/// \brief Vector of net sink Tilewires.
		TilewireVector mSinks;
		/// \brief Vector of arcs representing net connectivity.
		ArcVector mArcs;
		/// \brief Vector of RouteNodes representing net connectivity.
		RouteNodePtrVector mRouteNodes;
		/// \brief Net annotation structure.
	public:
		boost::unordered_map<boost::uint32_t, boost::any> mProperties;

	public:
	// constructors
		/// \brief Name only constructor.
		RouteNet(string& inName) : mName(inName) {}
		/// \brief Populated constructor.
		/// \details Copies the contents of the input vectors.
		RouteNet(string& inName, const TilewireVector& inSources, const TilewireVector& inSinks) :
			mName(inName), mSources(inSources), mSinks(inSinks) {}
	// functions
		/// \brief Determines whether the given Tilewire is a source of this net.
		/// \returns true if the given Tilewire is a source of this net, or false otherwise.
		bool containsSource(Tilewire inTilewire) const {
			return std::find(sourcesBegin(), sourcesEnd(), inTilewire) != sourcesEnd();
		}
		/// \brief Determines whether the given Tilewire is a sink of this net.
		/// \returns true if the given Tilewire is a sink of this net, or false otherwise.
		bool containsSink(Tilewire inTilewire) const {
			return std::find(sinksBegin(), sinksEnd(), inTilewire) != sinksEnd();
		}
		/// \brief Determines whether the net contains the given arc.
		/// \returns true if the net contains the given arc, or false otherwise.
		bool containsArc(const Arc& inArc) const {
			return std::find(arcsBegin(), arcsEnd(), inArc) != arcsEnd();
		}
		/// \brief Adds the given Tilewire as a source for this net.
		/// \details Duplicate sources are not currently discarded.
		void addSource(Tilewire inTilewire) {
			mSources.push_back(inTilewire);
		}
		/// \brief Adds the given Tilewire as a sink for this net.
		/// \details Duplicate sinks are not currently discarded.
		void addSink(Tilewire inTilewire) {
			mSinks.push_back(inTilewire);
		}
		/// \brief Removes the given Tilewire from the sources of this net.
		/// \returns true if the Tilewire was removed from the source list, or false if it was 
		///		not found.
		bool removeSource(Tilewire inTilewire) {
			TilewireIterator e = sourcesEnd();
			TilewireIterator result = std::find(sourcesBegin(), e, inTilewire);
			if(result == e) return false;
			mSources.erase(result);
			return true;
		}
		/// \brief Removes the given instance pin from the sinks of this net.
		/// \returns true if the instance pin was removed from the sink list, or false if it was 
		///		not found.
		bool removeSink(Tilewire inTilewire) {
			TilewireIterator e = sinksEnd();
			TilewireIterator result = std::find(sinksBegin(), e, inTilewire);
			if(result == e) return false;
			mSinks.erase(result);
			return true;
		}
		/// \brief Adds the given arc to this net.
		/// \details Duplicate arcs are not currently discarded.
		void addArc(const Arc& inArc) {
			mArcs.push_back(inArc);
		}
		/// \brief Removes the given arc from this net.
		/// \returns true if the arc was removed from this net, or false if it was not found.
		bool removeArc(const Arc& inArc) {
			ArcIterator e = arcsEnd();
			ArcIterator result = std::find(arcsBegin(), e, inArc);
			// do nothing if the arc doesn't exist
			if(result == e) return false;
			// erase the arc
			mArcs.erase(result);
			return true;
		}
		/// \brief Unroute the net.
		/// \details The net is unrouted by clearing its arc list.
		void unroute(void) {
			mArcs.clear();
		}
	// tests
		/// \brief Returns true if the net has any sources.
		bool hasAnySources(void) const { return !mSources.empty(); }
		/// \brief Returns true if the net has exactly one source.
		bool hasOneSource(void) const { return mSources.size() == 1; }
		/// \brief Returns true if the net has more than one source.
		bool hasMultipleSources(void) const { return mSources.size() > 1; }
		/// \brief Returns the number of sources on the net.
		size_t getSourceCount(void) const { return mSources.size(); }
		/// \brief Returns true if the net has any sinks.
		bool hasAnySinks(void) const { return !mSinks.empty(); }
		/// \brief Returns true if the net has exactly one sink.
		bool hasOneSink(void) const { return mSinks.size() == 1; }
		/// \brief Returns true if the next has more than one sink.
		bool hasMultipleSinks(void) const { return mSinks.size() > 1; }
		/// \brief Returns the number of sinks on the net.
		size_t getSinkCount(void) const { return mSinks.size(); }
		/// \brief Returns true if the net has any arcs.
		bool hasAnyArcs(void) const { return !mArcs.empty(); }
		/// \brief Returns the number of arcs on the net.
		size_t getArcCount(void) const { return mArcs.size(); }
		/// \brief Returns true if the net has any arcs.
		bool isRouted(void) const { return hasAnyArcs(); }
		/// \brief Returns true if the net has no arcs.
		bool isUnrouted(void) const { return !hasAnyArcs(); }
	// iterators
		/// \brief Returns the begin constant iterator for source Tilewires.
		TilewireConstIterator sourcesBegin(void) const { return mSources.begin(); }
		/// \brief Returns the end constant iterator for source Tilewires.
		TilewireConstIterator sourcesEnd(void) const { return mSources.end(); }
		/// \brief Returns the begin non-constant iterator for source Tilewires.
		TilewireIterator sourcesBegin(void) { return mSources.begin(); }
		/// \brief Returns the end non-constant iterator for source Tilewires.
		TilewireIterator sourcesEnd(void) { return mSources.end(); }
		/// \brief Returns the begin constant iterator for sink Tilewires.
		TilewireConstIterator sinksBegin(void) const { return mSinks.begin(); }
		/// \brief Returns the end constant iterator for sink Tilewires.
		TilewireConstIterator sinksEnd(void) const { return mSinks.end(); }
		/// \brief Returns the begin non-constant iterator for sink Tilewires.
		TilewireIterator sinksBegin(void) { return mSinks.begin(); }
		/// \brief Returns the end non-constant iterator for sink Tilewires.
		TilewireIterator sinksEnd(void) { return mSinks.end(); }
		/// \brief Returns the begin constant iterator for arcs.
		ArcConstIterator arcsBegin(void) const { return mArcs.begin(); }
		/// \brief Returns the end constant iterator for arcs.
		ArcConstIterator arcsEnd(void) const { return mArcs.end(); }
		/// \brief Returns the begin non-constant iterator for arcs.
		ArcIterator arcsBegin(void) { return mArcs.begin(); }
		/// \brief Returns the end non-constant iterator for arcs.
		ArcIterator arcsEnd(void) { return mArcs.end(); }
	// accessors
		/// \brief Returns the name of the net.
		const string& getName() const { return mName; }
		/// \brief Returns a reference to the RouteNodePtrVector.
		RouteNodePtrVector& routeNodes() { return mRouteNodes; }
	};

	/// \brief Vector of RouteNet objects.
	typedef std::vector<RouteNet> RouteNetVector;

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_ROUTENET_HPP
