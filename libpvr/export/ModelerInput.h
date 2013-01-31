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

/*! \file ModelerInput.h
  Contains the ModelerInput class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_MODELERINPUT_H__
#define __INCLUDED_PVR_MODELERINPUT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <boost/foreach.hpp>

// Project headers

#include "pvr/export.h"
#include "pvr/Geometry.h"
#include "pvr/ParamBase.h"
#include "pvr/Types.h"
#include "pvr/Primitives/Primitive.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {

//----------------------------------------------------------------------------//
// ModelerInput
//----------------------------------------------------------------------------//

/*! \class ModelerInput
  \brief Holds the data structures that define a single input to the Modeler
  class.
 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC ModelerInput
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(ModelerInput);

  // Constructor, destructor, factory ------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC(ModelerInput);

  // Main methods --------------------------------------------------------------

  //! Sets the geometry data that the input should use
  void                  setGeometry(Geo::Geometry::CPtr geometry);
  //! Returns a pointer to the geometry contained by the ModelerInput.
  Geo::Geometry::CPtr   geometry() const;
  //! Sets the volumetric primitive to use for generating data
  void                  setVolumePrimitive(Prim::Primitive::CPtr primitive);
  //! Returns the volumetric primitive currently assigned.
  Prim::Primitive::CPtr volumePrimitive() const;

protected:
  
  // Protected data members ----------------------------------------------------

  Geo::Geometry::CPtr   m_geometry;
  Prim::Primitive::CPtr m_primitive;

};

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
