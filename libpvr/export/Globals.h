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

/*! \file Globals.h
  Contains the Globals class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_GLOBALS_H__
#define __INCLUDED_PVR_GLOBALS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

#include "pvr/Exception.h"

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {
namespace Sys {

//----------------------------------------------------------------------------//
// Globals
//----------------------------------------------------------------------------//

class Globals 
{
public:
  
  // Main methods --------------------------------------------------------------

  //! Initializes the globals. Safe to call multiple times
  static void init();

  // Accessors -----------------------------------------------------------------

  static bool debugLogEnabled()
  { return ms_debugLogEnabled; }

private:

  // Data members --------------------------------------------------------------

  static bool ms_debugLogEnabled;

};

//----------------------------------------------------------------------------//

} // namespace Sys
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
