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

/*! \file Interpolation.h
  Contains various interpolation functions
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INTERPOLATION_H__
#define __INCLUDED_PVR_INTERPOLATION_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {
namespace Util {

//----------------------------------------------------------------------------//
// Interpolation functions
//----------------------------------------------------------------------------//

//! Linear interpolation in two dimensions
template <typename T>
T lerp2D(const float tX, const float tY, const T &x0y0, const T &x1y0,
         const T &x0y1, const T &x1y1);

//----------------------------------------------------------------------------//
// Template implementations
//----------------------------------------------------------------------------//

template <typename T>
T lerp2D(const float tX, const float tY, const T &x0y0, const T &x1y0,
         const T &x0y1, const T &x1y1)
{
  return ((1.0f - tX) * ((1.0f - tY) * x0y0 + tY * x0y1) + 
                  tX  * ((1.0f - tY) * x1y0 + tY * x1y1));
}

//----------------------------------------------------------------------------//

} // namespace Util
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
