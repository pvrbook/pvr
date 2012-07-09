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

/*! \file Globals.cpp
  Contains implementations of Globals class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Globals.h"

// System includes

#include <stdlib.h>

// Project includes

#include "pvr/Log.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Sys {

//----------------------------------------------------------------------------//
// Globals static member instantiation
//----------------------------------------------------------------------------//

bool Globals::ms_debugLogEnabled = false;

//----------------------------------------------------------------------------//
// Globals implementations
//----------------------------------------------------------------------------//

void Globals::init()
{
  const char *debugLog = getenv("PVR_DEBUG");
  ms_debugLogEnabled = debugLog != NULL;
  if (debugLog) {
    Util::Log::print("Debug printouts enabled.");
  }
}

//----------------------------------------------------------------------------//

} // namespace Sys
} // namespace pvr

//----------------------------------------------------------------------------//
