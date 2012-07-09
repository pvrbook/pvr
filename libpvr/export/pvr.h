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

/*! \file pvr.h
  Contains the Geometry class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_H__
#define __INCLUDED_PVR_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/lexical_cast.hpp>

// Project headers

#include "AttrTable.h"
#include "Exception.h"
#include "Types.h"

//----------------------------------------------------------------------------//
// pvr namespace documentation
//----------------------------------------------------------------------------//

/*! \namespace pvr
  \brief This is the overall namespace of the pvr library
 */

/*! \namespace pvr::Accel
  \brief Acceleration data structures
 */

/*! \namespace pvr::Geo
  \brief Geometry classes and geometry-related functions
 */

/*! \namespace pvr::Math
  \brief Everything Math-related
 */

/*! \namespace pvr::Model
  \brief Everything Modeling-related
 */

/*! \namespace pvr::Model::Prim
  \brief Modeling primitives
 */

/*! \namespace pvr::Model::Prim::Rast
  \brief Rasterization-based primitives
 */

/*! \namespace pvr::Model::Prim::Inst
  \brief Instantiation-based primitives
 */

/*! \namespace pvr::Noise
  \brief Everything Noise-related
 */

/*! \namespace pvr::Render
  \brief Everything Rendering-related
 */

/*! \namespace pvr::Util
  \brief Various utility classes and functions
 */

/*! \namespace pvr::Sys
  \brief Render system classes and functions
 */

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
