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

/*! \file Modeler.cpp
  Contains implementations of Modeler class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Modeler.h"

// System includes

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {

//----------------------------------------------------------------------------//
// ModelerInput
//----------------------------------------------------------------------------//

void ModelerInput::setGeometry(Geo::Geometry::CPtr geometry)
{
  m_geometry = geometry;
}
  
//----------------------------------------------------------------------------//

Geo::Geometry::CPtr ModelerInput::geometry() const
{
  return m_geometry;
}

//----------------------------------------------------------------------------//

void ModelerInput::setVolumePrimitive(Prim::Primitive::CPtr primitive)
{
  m_primitive = primitive;
}

//----------------------------------------------------------------------------//

Prim::Primitive::CPtr ModelerInput::volumePrimitive() const
{
  return m_primitive;
}
  
//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
