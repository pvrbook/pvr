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

/*! \file Common.cpp
  Contains implementations of functions from Common.h
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "Common.h"

// System includes

// Library includes

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

Util::ParamMap dictToParamMap(boost::python::dict d)
{
  using namespace std;
  using namespace pvr;

  pvr::Util::ParamMap params;
  params.intMap = pyDictValues<string, int>(d);
  params.floatMap = pyDictValues<string, float>(d);
  params.vectorMap = pyDictValues<string, Imath::V3f>(d);
  params.stringMap = pyDictValues<string, string>(d);

  return params;
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//
