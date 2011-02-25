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

#ifndef TORC_PHYSICAL_NET_HPP
#define TORC_PHYSICAL_NET_HPP

#include "torc/physical/Named.hpp"
#include "torc/physical/Progeny.hpp"
#include "torc/physical/Progenitor.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/InstancePin.hpp"
#include "torc/physical/Pip.hpp"
#include "torc/physical/Routethrough.hpp"
#include <string>

namespace torc {
namespace physical {

	/// \brief Physical design net.
	/// \details The Net class owns its instance pins and its pips, and is responsible for deleting 
	///		them.
	/// \todo Consider checking for duplicates whenever a source, sink, or pip is added.  On the 
	///		other hand, we provide the functions to check for existing resources, so perhaps we can 
	///		just leave it to the user.
	class Net : public Named, public Progeny<class Circuit>, public ConfigMap, 
		protected Progenitor<Net> {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The net power type.  See ENetType.
		ENetType mNetType;
		/// \brief Vector of instance pin shared pointer sources for the net.
		InstancePinSharedPtrVector mSources;
		/// \brief Vector of instance pin shared pointer sinks for the net.
		InstancePinSharedPtrVector mSinks;
		/// \brief Vector of pips for the net.
		PipVector mPips;
	// constructors
		/// \brief Protected constructor.
		/// \param inName The net name.
		/// \param inNetType The net power type.
		Net(const string& inName, ENetType inNetType) : Named(inName), ConfigMap(), 
			mNetType(inNetType) {}
	public:
	// types
		/// \brief Constant iterator to InstancePin shared pointer objects.
		typedef InstancePinSharedPtrVector::const_iterator InstancePinSharedPtrConstIterator;
		/// \brief Non-constant iterator to InstancePin shared pointer objects.
		typedef InstancePinSharedPtrVector::iterator InstancePinSharedPtrIterator;
		/// \brief Constant iterator to Pip objects.
		typedef PipVector::const_iterator PipConstIterator;
		/// \brief Non-constant iterator to Pip objects.
		typedef PipVector::iterator PipIterator;
	// functions
		/// \brief Determines whether the given instance pin is a source of this net.
		/// \returns true if the given instance pin is a source of this net, or false otherwise.
		bool containsSource(InstancePinSharedPtr& inInstancePinPtr) const {
			return std::find(sourcesBegin(), sourcesEnd(), inInstancePinPtr) != sourcesEnd();
		}
		/// \brief Determines whether the given instance pin is a sink of this net.
		/// \returns true if the given instance pin is a sink of this net, or false otherwise.
		bool containsSink(InstancePinSharedPtr& inInstancePinPtr) const {
			return std::find(sinksBegin(), sinksEnd(), inInstancePinPtr) != sinksEnd();
		}
		/// \brief Determines whether the net contains the given pip.
		/// \returns true if the net contains the given pip, or false otherwise.
		bool containsPip(const Pip& inPip) const {
			return std::find(pipsBegin(), pipsEnd(), inPip) != pipsEnd();
		}
		/// \brief Adds the given instance pin as a source for this net.
		/// \details Duplicate sources are not currently discarded.
		void addSource(InstancePinSharedPtr& inInstancePinPtr) {
			/// \todo Acquire mutex.
			mSources.push_back(inInstancePinPtr);
			inInstancePinPtr->setParentWeakPtr(mSelfWeakPtr);
			inInstancePinPtr->addToInstance();
//			inInstancePinPtr->getInstancePtr().lock()->addPin(inInstancePinPtr);
			/// \todo Release mutex.
		}
		/// \brief Adds the given instance pin as a sink for this net.
		/// \details Duplicate sinks are not currently discarded.
		void addSink(InstancePinSharedPtr& inInstancePinPtr) {
			/// \todo Acquire mutex.
			mSinks.push_back(inInstancePinPtr);
			inInstancePinPtr->setParentWeakPtr(mSelfWeakPtr);
			inInstancePinPtr->addToInstance();
//			inInstancePinPtr->getInstancePtr().lock()->addPin(inInstancePinPtr);
			/// \todo Release mutex.
		}
		/// \brief Removes the given instance pin from the sources of this net.
		/// \returns true if the instance pin was removed from the source list, or false if it was 
		///		not found.
		bool removeSource(InstancePinSharedPtr& inInstancePinPtr) {
			/// \todo Acquire mutex.
			InstancePinSharedPtrIterator e = sourcesEnd();
			InstancePinSharedPtrIterator result = std::find(sourcesBegin(), e, inInstancePinPtr);
			if(result == e) return false;
			inInstancePinPtr->removeFromInstance();
			mSources.erase(result);
			/// \todo Release mutex.
			return true;
		}
		/// \brief Removes the given instance pin from the sinks of this net.
		/// \returns true if the instance pin was removed from the sink list, or false if it was 
		///		not found.
		bool removeSink(InstancePinSharedPtr& inInstancePinPtr) {
			/// \todo Acquire mutex.
			InstancePinSharedPtrIterator e = sinksEnd();
			InstancePinSharedPtrIterator result = std::find(sinksBegin(), e, inInstancePinPtr);
			if(result == e) return false;
			inInstancePinPtr->removeFromInstance();
			mSinks.erase(result);
			/// \todo Release mutex.
			return true;
		}
		/// \brief Adds the given pip to this net.
		/// \details Duplicate pips are not currently discarded.
		void addPip(const Pip& inPip) {
			/// \todo Acquire mutex.
			mPips.push_back(inPip);
			const_cast<Pip&>(inPip).setParentWeakPtr(mSelfWeakPtr);
			/// \todo Release mutex.
		}
		/// \brief Removes the given pip from this net.
		/// \returns true if the pip was removed from this net, or false if it was not found.
		bool removePip(const Pip& inPip) {
			/// \todo Acquire mutex.
			// look up the pip
			PipIterator e = pipsEnd();
			PipIterator result = std::find(pipsBegin(), e, inPip);
			// do nothing if the pip doesn't exist
			if(result == e) return false;
			// erase the pip
			mPips.erase(result);
			/// \todo Release mutex.
			return true;
		}
		/// \brief Unroute the net.
		/// \details The net is unrouted by clearing its pip list.
		void unroute(void) {
			mPips.clear();
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
		/// \brief Returns true if the net has any pips.
		bool hasAnyPips(void) const { return !mPips.empty(); }
		/// \brief Returns the number of pips on the net.
		size_t getPipCount(void) const { return mPips.size(); }
		/// \brief Returns true if the net has any pips.
		bool isRouted(void) const { return hasAnyPips(); }
		/// \brief Returns true if the net has no pips.
		bool isUnrouted(void) const { return !hasAnyPips(); }
	// iterators
		/// \brief Returns the begin constant iterator for source instance pins.
		InstancePinSharedPtrConstIterator sourcesBegin(void) const { return mSources.begin(); }
		/// \brief Returns the end constant iterator for source instance pins.
		InstancePinSharedPtrConstIterator sourcesEnd(void) const { return mSources.end(); }
		/// \brief Returns the begin non-constant iterator for source instance pins.
		InstancePinSharedPtrIterator sourcesBegin(void) { return mSources.begin(); }
		/// \brief Returns the end non-constant iterator for source instance pins.
		InstancePinSharedPtrIterator sourcesEnd(void) { return mSources.end(); }
		/// \brief Returns the begin constant iterator for sink instance pins.
		InstancePinSharedPtrConstIterator sinksBegin(void) const { return mSinks.begin(); }
		/// \brief Returns the end constant iterator for sink instance pins.
		InstancePinSharedPtrConstIterator sinksEnd(void) const { return mSinks.end(); }
		/// \brief Returns the begin non-constant iterator for sink instance pins.
		InstancePinSharedPtrIterator sinksBegin(void) { return mSinks.begin(); }
		/// \brief Returns the end non-constant iterator for sink instance pins.
		InstancePinSharedPtrIterator sinksEnd(void) { return mSinks.end(); }
		/// \brief Returns the begin constant iterator for pips.
		PipConstIterator pipsBegin(void) const { return mPips.begin(); }
		/// \brief Returns the end constant iterator for pips.
		PipConstIterator pipsEnd(void) const { return mPips.end(); }
		/// \brief Returns the begin non-constant iterator for pips.
		PipIterator pipsBegin(void) { return mPips.begin(); }
		/// \brief Returns the end non-constant iterator for pips.
		PipIterator pipsEnd(void) { return mPips.end(); }
	// accessors
		/// \brief Returns the net power type.  See ENetPowerType.
		ENetType getNetType(void) const { return mNetType; }
		/// \brief Sets the net power type.  See ENetPowerType.
		void setNetType(ENetType inNetType) { mNetType = inNetType; }
	// operators
		/// \brief Equality operator.
		/// \details This function deems nets equal if their names are identical.
		/// \param rhs The net to compare against.
		/// \returns true if both net names are identical, or false otherwise.
		bool operator ==(const Net& rhs) const { return mName == rhs.mName; }
	};

	/// \brief Shared pointer encapsulation of a Net.
	typedef boost::shared_ptr<Net> NetSharedPtr;

	/// \brief Weak pointer encapsulation of a Net.
	typedef boost::weak_ptr<Net> NetWeakPtr;

	/// \brief Vector of Net shared pointers.
	typedef std::vector<NetSharedPtr> NetSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_NET_HPP
