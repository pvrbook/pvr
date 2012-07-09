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

/*! \file Strings.h
  Contains commonly used strings.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_STRINGS_H__
#define __INCLUDED_PVR_STRINGS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <string>

// Library includes

#include <boost/lexical_cast.hpp>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Common PVR strings
//----------------------------------------------------------------------------//

//! Struct containing commonly used strings.
struct Str {

static const std::string pvr;
static const std::string pvrLogStamp;

static const std::string density;
static const std::string scattering;
static const std::string absorption;
static const std::string emission;

};

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
