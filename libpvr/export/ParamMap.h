//----------------------------------------------------------------------------//

/*
    This file is part of PVR. Copyright (C) 2012 Magnus Wrenninge

    PVR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PVR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
