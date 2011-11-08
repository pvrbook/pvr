//----------------------------------------------------------------------------//

/*! \file ParamMap.h
  Contains the ParamMap class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PARAMMAP_H__
#define __INCLUDED_PVR_PARAMMAP_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Geometry.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Util {

//----------------------------------------------------------------------------//
// ParamMap
//----------------------------------------------------------------------------//

/*! \class ParamMap
  \brief Holds string-index key-value pairs for integer, float, vector and
  string attributes. Most commonly used to set parameters on Primitives.
 */

//----------------------------------------------------------------------------//

struct ParamMap
{

  // Typedefs ------------------------------------------------------------------

  typedef std::map<std::string, int>         IntMap;
  typedef std::map<std::string, float>       FloatMap;
  typedef std::map<std::string, Imath::V3f>  VectorMap;
  typedef std::map<std::string, std::string> StringMap;

  // Data members --------------------------------------------------------------

  IntMap    intMap;
  FloatMap  floatMap;
  VectorMap vectorMap;
  StringMap stringMap;

};

//----------------------------------------------------------------------------//

} // namespace Util
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
