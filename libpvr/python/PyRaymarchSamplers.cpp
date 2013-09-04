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

/*! \file PyRaymarchSamplers.cpp
  Contains the interface definition for the RaymarchSamplers subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Needs to be first, to avoid macro "tolower" passed 2 arguments, on OSX, python 2.7
#include <Python.h>

// System includes

#include <map>

// Library includes

#include <pvr/RaymarchSamplers/RaymarchSampler.h>
#include <pvr/RaymarchSamplers/DensitySampler.h>
#include <pvr/RaymarchSamplers/PhysicalSampler.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportRaymarchSamplers()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;

  using namespace pvr::Render;

  // RaymarchSampler ---

  class_<RaymarchSampler, RaymarchSampler::Ptr, boost::noncopyable>
    ("RaymarchSampler", no_init)
    .def("typeName", &RaymarchSampler::typeName)
    ;
  
  implicitly_convertible<RaymarchSampler::Ptr, RaymarchSampler::CPtr>();

  // DensitySampler ---

  class_<DensitySampler, bases<RaymarchSampler>, DensitySampler::Ptr>
    ("DensitySampler", no_init)
    .def("__init__", make_constructor(DensitySampler::create))
    ;
  
  implicitly_convertible<DensitySampler::Ptr, DensitySampler::CPtr>();

  // PhysicalSampler ---

  class_<PhysicalSampler, bases<RaymarchSampler>, PhysicalSampler::Ptr>
    ("PhysicalSampler", no_init)
    .def("__init__", make_constructor(PhysicalSampler::create))
    ;
  
  implicitly_convertible<PhysicalSampler::Ptr, PhysicalSampler::CPtr>();

}

//----------------------------------------------------------------------------//

