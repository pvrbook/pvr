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

/*! \file PyParticles.cpp
  Contains the interface definition for the Particles class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Needs to be first, to avoid macro "tolower" passed 2 arguments, on OSX, python 2.7
#include <Python.h>

// System includes

#include <boost/python.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Library includes

#include <pvr/Particles.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;

using namespace pvr::Geo;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

AttrTable& pointAttrsHelper(Particles &self)
{
  return self.pointAttrs();
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportParticles()
{
  using namespace boost::python;

  class_<Particles, Particles::Ptr>("Particles", no_init)
    .def("__init__",    make_constructor(Particles::create))
    .def("resize",      &Particles::resize)
    .def("add",         &Particles::add)
    .def("setPosition", &Particles::setPosition)
    .def("pointAttrs",  &pointAttrsHelper,
         return_value_policy<reference_existing_object>())
    ;

  implicitly_convertible<Particles::Ptr, Particles::CPtr>();

}

//----------------------------------------------------------------------------//
