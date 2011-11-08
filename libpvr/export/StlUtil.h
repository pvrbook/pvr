//-*-c++-*--------------------------------------------------------------------//

/*! \file StlUtil.h
  Contains the StlUtil class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_STLUTIL_H__
#define __INCLUDED_PVR_STLUTIL_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

template <typename Array_T>
void swapClear(Array_T &array)
{
  Array_T tmp;
  array.swap(tmp);
}

//----------------------------------------------------------------------------//

template <typename Map_T>
bool hasKey(const Map_T &map, const typename Map_T::key_type &key)
{
  return map.find(key) != map.end();
}

//----------------------------------------------------------------------------//

//! See also 
//! http://stackoverflow.com/questions/152643/idiomatic-c-for-reading-from-a-const-map
template <typename Map_T>
typename Map_T::mapped_type value(const Map_T &map, 
                                  const typename Map_T::key_type &key,
                                  const typename Map_T::mapped_type &def)
{
  typename Map_T::const_iterator i(map.find(key));
  return i != map.end() ? i->second : def;
}

//----------------------------------------------------------------------------//

//! See also 
//! http://stackoverflow.com/questions/152643/idiomatic-c-for-reading-from-a-const-map
template <typename Map_T, typename ValueConvertible_T>
void getValue(const Map_T &map, const typename Map_T::key_type &key,
              ValueConvertible_T &out)
{
  typename Map_T::const_iterator i(map.find(key));
  if (i != map.end()) {
    out = i->second;
  }
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
