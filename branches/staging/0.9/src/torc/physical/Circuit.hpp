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
/// \brief Header for the Circuit class.

#ifndef TORC_PHYSICAL_CIRCUIT_HPP
#define TORC_PHYSICAL_CIRCUIT_HPP

#include "torc/physical/Named.hpp"
#include "torc/physical/Progeny.hpp"
#include "torc/physical/Progenitor.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/Net.hpp"
#include "torc/common/Annotated.hpp"
#include <map>

namespace torc {
namespace physical {

	/// \brief Circuit composed of instances and nets.
	/// \details This class serves as a base class for Design and Module classes, and cannot be 
	///		instantiated.  A circuit is essentially a container for instances and their connecting 
	///		nets.
	/// \warning Do not attempt to sort or otherwise manipulate the order of the instances and 
	///		nets.  An internal container maps instance or net names to corresponding vector indexes 
	///		for fast lookup, and those indexes updated whenever and instance or net is removed.  
	///		Sorting or changing the order of the instances and nets will result in incorrect 
	///		results from findInstance() and findNet(), along with data corruption if 
	///		removeInstance() or removeNet() are subsequently called.
	/// \todo May want to replace the instance and net vector and map combinations with a Boost 
	///		multi-index.
	class Circuit : public Named, public Progeny<class Circuit>, public ConfigMap, 
		public common::Annotated, protected Progenitor<class Circuit> {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Imported type name
		typedef boost::int64_t int64_t;
		/// \brief Map from an element name to a vector index.
		typedef std::map<std::string, int64_t> NameToIndexMap;
	// members
		/// \brief Vector of Instance shared pointers for the circuit.
		InstanceSharedPtrVector mInstances;
		/// \brief Instance name to index mapping for this circuit.
		NameToIndexMap mInstanceMap;
		/// \brief Vector of Net shared pointers for the circuit.
		NetSharedPtrVector mNets;
		/// \brief Net name to index mapping for this circuit.
		NameToIndexMap mNetMap;
	// constructors
		/// \brief Protected constructor.  Circuit objects cannot be instantiated.
		/// \param inName The circuit name.
		Circuit(const string& inName) : Named(inName), ConfigMap(), Progenitor<class Circuit>() {}
	public:
	// types
		/// \brief Constant iterator to Instance shared pointers.
		typedef InstanceSharedPtrVector::const_iterator InstanceSharedPtrConstIterator;
		/// \brief Non-constant iterator to Instance shared pointers.
		typedef InstanceSharedPtrVector::iterator InstanceSharedPtrIterator;
		/// \brief Constant iterator to Net shared pointers.
		typedef NetSharedPtrVector::const_iterator NetSharedPtrConstIterator;
		/// \brief Non-constant iterator to Net shared pointers.
		typedef NetSharedPtrVector::iterator NetSharedPtrIterator;
	// functions
		/// \brief Find a circuit instance by name.
		/// \param inName The instance name to look for.
		/// \returns an iterator for the specified instance, or instancesEnd() if the name was not 
		///		found.
		InstanceSharedPtrIterator findInstance(const string& inName) {
			NameToIndexMap::iterator result = mInstanceMap.find(inName);
			NameToIndexMap::iterator e = mInstanceMap.end();
			if(result == e) return instancesEnd();
			return instancesBegin() + result->second;
		}
		/// \brief Add an instance to the circuit.
		/// \param inInstancePtr The instance to add.
		/// \returns true if the instance was added, or false if an instance with the same name 
		///		already exists in the circuit.
		bool addInstance(InstanceSharedPtr& inInstancePtr) {
			/// \todo Acquire mutex.
			const std::string& name = inInstancePtr->getName();
			InstanceSharedPtrIterator e = mInstances.end();
			InstanceSharedPtrIterator result = findInstance(name);
			if(result != e) return false;
			inInstancePtr->setParentWeakPtr(mParentWeakPtr);
			mInstanceMap[name] = mInstances.size();
			mInstances.push_back(inInstancePtr);
			return true;
			/// \todo Release mutex.
		}
		/// \brief Remove an instance from the circuit.
		/// \param inInstancePtr The instance to remove.
		/// \returns true if the instance was removed, or false if the instance did not exist.
		bool removeInstance(InstanceSharedPtr& inInstancePtr) {
			/// \todo Acquire mutex.
			const std::string& name = inInstancePtr->getName();
			NameToIndexMap::iterator result = mInstanceMap.find(name);
			NameToIndexMap::iterator e = mInstanceMap.end();
			if(result == e) return false;
			inInstancePtr->resetParentWeakPtr();
			// fix up the map indexes
			NameToIndexMap::mapped_type target = result->second;
			NameToIndexMap::iterator mp = mInstanceMap.begin();
			NameToIndexMap::iterator me = mInstanceMap.end();
			while(mp != me) {
				NameToIndexMap::mapped_type& index = mp->second;
				if(index > target) index--;
				mp++;
			}
			mInstances.erase(mInstances.begin() + target);
			mInstanceMap.erase(result);
			/// \todo Release mutex.
			return true;
		}
		/// \brief Unplace the circuit by discarding placement information for each instance.
		void unplace(void) {
			/// \todo Acquire mutex.
			InstanceSharedPtrIterator p = instancesBegin();
			InstanceSharedPtrIterator e = instancesEnd();
			while(p < e) (*p++)->unplace();
			/// \todo Release mutex.
		}
		/// \brief Find a circuit net by name.
		/// \param inName The net name to look for.
		/// \returns an iterator to the specified net, or netsEnd() if the name was not found.
		NetSharedPtrIterator findNet(const string& inName) {
			NameToIndexMap::iterator result = mNetMap.find(inName);
			NameToIndexMap::iterator e = mNetMap.end();
			if(result == e) return netsEnd();
			return netsBegin() + result->second;
		}
		/// \brief Add a net to the circuit.
		/// \param inNetPtr The net to add.
		/// \returns true if the net was added, or false if a net with the same name already exists 
		///		in the circuit.
		bool addNet(NetSharedPtr& inNetPtr) {
			/// \todo Acquire mutex.
			const std::string& name = inNetPtr->getName();
			NetSharedPtrIterator e = mNets.end();
			NetSharedPtrIterator result = findNet(name);
			if(result != e) return false;
			inNetPtr->setParentWeakPtr(mParentWeakPtr);
			mNetMap[name] = mNets.size();
			mNets.push_back(inNetPtr);
			return true;
			/// \todo Release mutex.
		}
		/// \brief Remove a net from the circuit.
		/// \param inNetPtr The net to remove.
		/// \returns true if the net was removed, or false if the net did not exist.
		bool removeNet(NetSharedPtr& inNetPtr) {
			/// \todo Acquire mutex.
			const std::string& name = inNetPtr->getName();
			NameToIndexMap::iterator result = mNetMap.find(name);
			NameToIndexMap::iterator e = mNetMap.end();
			if(result == e) return false;
			inNetPtr->resetParentWeakPtr();
			// fix up the map indexes
			NameToIndexMap::mapped_type target = result->second;
			NameToIndexMap::iterator mp = mNetMap.begin();
			NameToIndexMap::iterator me = mNetMap.end();
			while(mp != me) {
				NameToIndexMap::mapped_type& index = mp->second;
				if(index > target) index--;
				mp++;
			}
			mNets.erase(mNets.begin() + target);
			mNetMap.erase(result);
			/// \todo Release mutex.
			return true;
		}
		/// \brief Unroute the circuit by discarding routing information for each net.
		void unroute(void) {
			/// \todo Acquire mutex.
			NetSharedPtrIterator p = netsBegin();
			NetSharedPtrIterator e = netsEnd();
			while(p < e) (*p++)->unroute();
			/// \todo Release mutex.
		}
	// iterators
		/// \brief Returns the begin constant iterator for instances.
		InstanceSharedPtrConstIterator instancesBegin(void) const { return mInstances.begin(); }
		/// \brief Returns the end constant iterator for instances.
		InstanceSharedPtrConstIterator instancesEnd(void) const { return mInstances.end(); }
		/// \brief Returns the begin non-constant iterator for instances.
		InstanceSharedPtrIterator instancesBegin(void) { return mInstances.begin(); }
		/// \brief Returns the end non-constant iterator for instances.
		InstanceSharedPtrIterator instancesEnd(void) { return mInstances.end(); }
		/// \brief Returns the number of instances in the circuit.
		size_t getInstanceCount(void) const { return mInstances.size(); }
		/// \brief Returns the begin constant iterator for nets.
		NetSharedPtrConstIterator netsBegin(void) const { return mNets.begin(); }
		/// \brief Returns the end constant iterator for nets.
		NetSharedPtrConstIterator netsEnd(void) const { return mNets.end(); }
		/// \brief Returns the begin non-constant iterator for nets.
		NetSharedPtrIterator netsBegin(void) { return mNets.begin(); }
		/// \brief Returns the end non-constant iterator for nets.
		NetSharedPtrIterator netsEnd(void) { return mNets.end(); }
		/// \brief Returns the number of nets in the circuit.
		size_t getNetCount(void) const { return mNets.size(); }
	};

	/// \brief Shared pointer encapsulation of a Circuit.
	typedef boost::shared_ptr<Circuit> CircuitSharedPtr;

	/// \brief Weak pointer encapsulation of a Circuit.
	typedef boost::weak_ptr<Circuit> CircuitWeakPtr;

	/// \brief Vector of Circuit shared pointers.
	typedef std::vector<CircuitSharedPtr> CircuitSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_CIRCUIT_HPP
