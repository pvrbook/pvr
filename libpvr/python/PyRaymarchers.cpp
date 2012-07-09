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

/*! \file PyRaymarchers.cpp
  Contains the interface definition for the Raymarchers subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

#include <pvr/Raymarchers/Raymarcher.h>
#include <pvr/Raymarchers/UniformRaymarcher.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

void setParamsHelper(pvr::Util::ParamBase &self, boost::python::dict d)
{
  self.setParams(pvr::dictToParamMap(d));
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportRaymarchers()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;
  
  using namespace pvr::Render;

  // ParamBase ---

  class_<Util::ParamBase, Util::ParamBase::Ptr, boost::noncopyable>
    ("ParamBase", no_init)
    .def("setParams", &setParamsHelper)
    ;

  // Raymarcher ---

  class_<Raymarcher, bases<Util::ParamBase>, 
         Raymarcher::Ptr, boost::noncopyable>
    ("Raymarcher", no_init)
    .def("typeName",           &Raymarcher::typeName)
    .def("setRaymarchSampler", &Raymarcher::setRaymarchSampler)
    ;
  
  implicitly_convertible<Raymarcher::Ptr, Raymarcher::CPtr>();

  // UniformRaymarcher ---

  class_<UniformRaymarcher, bases<Raymarcher>, UniformRaymarcher::Ptr>
    ("UniformRaymarcher", no_init)
    .def("__init__", make_constructor(UniformRaymarcher::create))
    ;
  
  implicitly_convertible<UniformRaymarcher::Ptr, UniformRaymarcher::CPtr>();

}

//----------------------------------------------------------------------------//

