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

/*! \file InstantiationPrimitive.h
  Contains the InstantiationPrimitive class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INSTANTIATIONPRIMITIVE_H__
#define __INCLUDED_PVR_INSTANTIATIONPRIMITIVE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers



// Project headers

#include "pvr/ModelerInput.h"
#include "pvr/Primitives/Primitive.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Inst {

//----------------------------------------------------------------------------//
// InstantiationPrim
//----------------------------------------------------------------------------//

/*! \class InstantiationPrim
  \brief Base class for instantiation-based primitives.

  Instantiation-based primitives are covered in chapter XX.
 */

//----------------------------------------------------------------------------//

class InstantiationPrim : public Primitive
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(InstantiationPrim);

  // Main methods --------------------------------------------------------------

  //! Executes the instantiation primitive
  virtual ModelerInput::Ptr execute(const Geo::Geometry::CPtr geo) const = 0;

};

//----------------------------------------------------------------------------//

} // namespace Inst
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
