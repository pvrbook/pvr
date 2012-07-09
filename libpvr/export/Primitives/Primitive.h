//-*-c++-*--------------------------------------------------------------------//

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

/*! \file Primitive.h
  Contains the Primitive class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PRIMITIVE_H__
#define __INCLUDED_PVR_PRIMITIVE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Geometry.h"
#include "pvr/ParamMap.h"
#include "pvr/ParamBase.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Defines
//----------------------------------------------------------------------------//

#define PVR_PRIM_SANITY_CHECK(prim)                                     \
  if (!points.vectorAttrRef("P").isValid()) {                           \
    Log::warning(prim " - no P attribute in input");                    \
    return;                                                             \
  }                                                                     \

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {

//----------------------------------------------------------------------------//
// Primitive
//----------------------------------------------------------------------------//

/*! \class Primitive
  \brief Base class for all volume modeling primitives.

  See chapter 9.3 for details.
 */

//----------------------------------------------------------------------------//

class Primitive : public Util::ParamBase
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Primitive);

  // Main methods --------------------------------------------------------------

  //! Returns a world-space bounding box for the primitive
  //! \note See chapter 8.2.1 (Bounding primitives)
  virtual BBox wsBounds(Geo::Geometry::CPtr geometry) const = 0;

  //! Sets the primitive's global parameters
  //! \note See chapter 5.4 for an overview of attributes and parameters in PVR.
  void setParams(const Util::ParamMap &params)
  { m_params = params; }

protected:

  // Data members --------------------------------------------------------------

  //! The primitive's global parameters. 
  Util::ParamMap m_params;

};

//----------------------------------------------------------------------------//

} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
