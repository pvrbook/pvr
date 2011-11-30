//-*-c++-*--------------------------------------------------------------------//

/*! \file Common.h
  Contains common utility functions for the Python PVR setup.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PY_COMMON_H__
#define __INCLUDED_PVR_PY_COMMON_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <boost/shared_ptr.hpp>

#include <Python.h>

#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Project headers

#include <pvr/Log.h>
#include <pvr/ParamMap.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

template <typename T>
boost::python::list vecToList(const std::vector<T> &vec)
{
  boost::python::list values;
  for (typename std::vector<T>::const_iterator i = vec.begin(), end = vec.end();
       i != end; ++i) {
    values.append(*i);
  }
  return values;
}

//----------------------------------------------------------------------------//
template <typename T>
std::vector<T> pyListValues(boost::python::list l)
{
  using namespace boost::python;
  using namespace std;

  vector<T> hits;

  for (boost::python::ssize_t i = 0, end = len(l); i < end; ++i) {
    object o = l.pop();
    extract<T> s(o);
    if (s.check()) {
      hits.push_back(s());
    } 
  }

  return hits;
}

//----------------------------------------------------------------------------//

template <typename Key_T, typename Value_T>
std::map<Key_T, Value_T> pyDictValues(boost::python::dict d)
{
  using namespace boost::python;
  using namespace std;
  using namespace pvr::Util;

  vector<Key_T> keys(pyListValues<Key_T>(d.keys()));
  map<Key_T, Value_T> result;

  for (typename vector<Key_T>::const_iterator i = keys.begin(), end = keys.end();
       i != end; ++i) {
    try {
      object valueObject = d.get(*i);
      extract<Value_T> value(valueObject);
      if (value.check()) {
        result[*i] = value();
      }
    }
    catch (...) { 
      Log::warning("Caught unexpected exception in pyDictValues"); 
    }
  }

  return result;
}

//----------------------------------------------------------------------------//

Util::ParamMap dictToParamMap(boost::python::dict d);

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
