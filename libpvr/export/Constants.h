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

/*! \file Constants.h
  Contains the Constants class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_CONSTANTS_H__
#define __INCLUDED_PVR_CONSTANTS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Common PVR constants
//----------------------------------------------------------------------------//

namespace Colors {
  inline Color one()
  { return Color(1.0); }
  inline Color zero()
  { return Color(0.0); }
  inline Color red()
  { return Color(1.0, 0.0, 0.0); }
}

//----------------------------------------------------------------------------//

namespace Vectors {
  inline Vector zero() {
    return Vector(0.0);
  }
}

//----------------------------------------------------------------------------//

namespace Bounds {
  inline BBox zeroOne() {
    return BBox(Vector(0.0), Vector(1.0));
  }
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
