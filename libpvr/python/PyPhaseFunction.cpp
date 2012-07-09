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

/*! \file PyPhaseFunction.cpp
  Contains the interface definition for the PhaseFunction base and subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>

// Library includes

#include <pvr/PhaseFunction.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportPhaseFunction()
{
  using namespace boost::python;
  using namespace pvr::Render::Phase;

  // PhaseFunction ---

  class_<PhaseFunction, PhaseFunction::Ptr, boost::noncopyable>
    ("PhaseFunction", no_init)
    .def("typeName",    &PhaseFunction::typeName)
    .def("probability", &PhaseFunction::probability)
    ;
  
  implicitly_convertible<PhaseFunction::Ptr, PhaseFunction::CPtr>();

  // Isotropic ---

  class_<Isotropic, bases<PhaseFunction>, Isotropic::Ptr>
    ("Isotropic")
    .def("__init__", make_constructor(Isotropic::create))
    ;

  implicitly_convertible<Isotropic::Ptr, Isotropic::CPtr>();

  // HenyeyGreenstein ---

  class_<HenyeyGreenstein, bases<PhaseFunction>, HenyeyGreenstein::Ptr>
    ("HenyeyGreenstein", no_init)
    .def("__init__", make_constructor(HenyeyGreenstein::create))
    ;

  implicitly_convertible<HenyeyGreenstein::Ptr, HenyeyGreenstein::CPtr>();

  // DoubleHenyeyGreenstein ---

  class_<DoubleHenyeyGreenstein, bases<PhaseFunction>, 
    DoubleHenyeyGreenstein::Ptr>
    ("DoubleHenyeyGreenstein", no_init)
    .def("__init__", make_constructor(DoubleHenyeyGreenstein::create))
    ;

  implicitly_convertible<DoubleHenyeyGreenstein::Ptr, 
                         DoubleHenyeyGreenstein::CPtr>();


}

//----------------------------------------------------------------------------//

