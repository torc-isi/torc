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

#ifndef TORC_GENERIC_OM_SYMTAB_HPP
#define TORC_GENERIC_OM_SYMTAB_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <algorithm>
#include <map>
#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/util/Error.hpp"

namespace torc {

namespace generic {

/**
 * @brief A symbol table
 *
 * This class acts as a symbol table to store key-value pairs
 */

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting = false>
class SymTab {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif

    struct Data
    {
#ifdef GENOM_SERIALIZATION
        friend class boost::serialization::access;
#endif
        size_t mIndex;
        _ValueType mValue;

        bool
        operator < ( const Data &inRhs ) const {
            return mIndex < inRhs.mIndex;
        }

#ifdef GENOM_SERIALIZATION
        template<class Archive> void
        serialize( Archive &ar, unsigned int ) {
            ar & mIndex;
            ar & mValue;
        }
#endif //GENOM_SERIALIZATION

    };
  public:
    typedef _KeyType KeyType;
    typedef _ValueType ValueType;
    typedef std::map<KeyType,Data> Map;
    typedef std::map<KeyType,ValueType> UserMap;

    SymTab();

    ~SymTab() throw();

    /**
     * Get a value for a key
     * @param[in] inKey key to look for
     * @param[out] outValue resultant value
     * @return true if object was found
     */
    inline bool
    get( const KeyType &inKey,
        ValueType &outValue ) const throw();

    /**
     * Set a value for a key. If cOverWriteExisting is true, Existing value will be replaced.
     * @param[in] inKey key to look for
     * @param[in] inValue value to be stored
     * @return True if value was inserted
     */
    inline bool
    set( const KeyType &inKey,
         const ValueType &inValue ) throw();

    /**
     * Remove a value for a key
     * @param[in] inKey key to look for
     * @return true if object was removed
     */
    inline bool
    remove( const KeyType &inKey ) throw();

    inline void
    getValues(std::vector<ValueType> &outValues) const throw();

    inline void
    getValueMap( UserMap &outMap ) const throw();

    inline size_t
    getSize() const throw();

    inline void
    clear() throw();

    template<typename _Action>
    void
    applyOnAll( const _Action &action ) throw(Error);

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Map mValues;
    //For ordering
    size_t mNextValue;
};

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
SymTab<_KeyType,_ValueType,cOverWriteExisting>::SymTab()
    :mValues(),
    mNextValue(0) {
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
SymTab<_KeyType,_ValueType,cOverWriteExisting>::~SymTab() throw() {
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
inline bool
SymTab<_KeyType,_ValueType,cOverWriteExisting>::get(
        const KeyType &inKey, ValueType &outValue ) const throw() {
    typename Map::const_iterator it = mValues.find( inKey );
    if( it == mValues.end() )
    {
        return false;
    }
    outValue = (*it).second.mValue;
    return true;
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
inline bool
SymTab<_KeyType,_ValueType,cOverWriteExisting>::set(
    const KeyType &inKey, const ValueType &inValue ) throw() {
    Data data;
    data.mIndex = mNextValue;
    data.mValue = inValue;
    mNextValue++;
    typename Map::value_type value
            = std::make_pair( inKey, data );

    std::pair< typename Map::iterator, bool > res
            = mValues.insert( value );
    if( false == res.second )
    {
        if( cOverWriteExisting )
        {
            mValues.erase( res.first );
            mValues.insert( value );
        }
        else
        {
            mNextValue--;
            return false;
        }
    }
    return true;
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
inline bool
SymTab<_KeyType,_ValueType,cOverWriteExisting>::remove(
            const KeyType &inKey ) throw()
{
    return mValues.erase( inKey ) > 0;
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
inline void
SymTab<_KeyType,_ValueType,cOverWriteExisting>::getValues(
        std::vector<ValueType> &outValues) const throw() {
    std::vector<Data> values;
    for( typename Map::const_iterator it = mValues.begin();
            it != mValues.end(); ++it )
    {
        values.push_back( (*it).second );
    }
    sort( values.begin(), values.end(), std::less<Data>());
    for( typename std::vector<Data>::iterator it = values.begin();
                it != values.end(); ++it )
    {
        outValues.push_back( (*it).mValue );
    }
    return;
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
inline void
SymTab<_KeyType,_ValueType,cOverWriteExisting>::getValueMap(
    typename SymTab<_KeyType,_ValueType,cOverWriteExisting>::UserMap &outMap    ) const throw()
{
    
    for( typename Map::const_iterator it = mValues.begin();
            it != mValues.end(); ++it )
    {
        typename UserMap::value_type value
            = std::make_pair( (*it).first, (*it).second.mValue );
        outMap.insert( value );
    }
    return;
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
inline size_t
SymTab<_KeyType,_ValueType,cOverWriteExisting>::getSize() const throw()
{
    return mValues.size();
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
inline void
SymTab<_KeyType,_ValueType,cOverWriteExisting>::clear() throw()
{
    mValues.clear();
}

template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
template<typename _Action>
inline void
SymTab<_KeyType,_ValueType,cOverWriteExisting>::applyOnAll( const _Action &action ) throw(Error)
{
    std::vector<Data> values;
    for( typename Map::const_iterator it = mValues.begin();
            it != mValues.end(); ++it )
    {
        values.push_back( (*it).second );
    }
    sort( values.begin(), values.end(), std::less<Data>());
    for( typename std::vector<Data>::iterator value = values.begin();
                value != values.end(); ++value ) {
        try
        {
            action( (*value).mValue );
        }
        catch(Error &e)
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

#ifdef GENOM_SERIALIZATION
template<typename _KeyType,
        typename _ValueType,
        bool cOverWriteExisting>
template<class Archive> void
SymTab<_KeyType,_ValueType,cOverWriteExisting>::serialize( Archive &ar, unsigned int ) {
    ar & mValues;
}
#endif //GENOM_SERIALIZATION

} // namespace generic

} //namespace torc

#endif // TORC_GENERIC_OM_SYMTAB_HPP
