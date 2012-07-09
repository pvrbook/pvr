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

/*! \file PyGeometry.cpp
  Contains the interface definition for the Geometry class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Library includes

#include <pvr/Geometry.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;
using namespace pvr::Geo;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

Particles::Ptr particlesHelper(Geometry &self)
{
  return self.particles();
}

//----------------------------------------------------------------------------//

Polygons::Ptr polygonsHelper(Geometry &self)
{
  return self.polygons();
}

//----------------------------------------------------------------------------//

Meshes::Ptr meshesHelper(Geometry &self)
{
  return self.meshes();
}

//----------------------------------------------------------------------------//

AttrTable& globalAttrHelper(Geometry &self)
{
  return self.globalAttrs();
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportGeometry()
{
  using namespace boost::python;

  class_<Geometry, Geometry::Ptr>("Geometry", no_init)
    .def("__init__",     make_constructor(Geometry::create))
    .def("read",         &Geometry::read).staticmethod("read")
    .def("setParticles", &Geometry::setParticles)
    .def("setPolygons",  &Geometry::setPolygons)
    .def("setMeshes",    &Geometry::setMeshes)
    .def("particles",    &particlesHelper)
    .def("polygons",     &polygonsHelper)
    .def("meshes",       &meshesHelper)
    .def("globalAttrs",  &globalAttrHelper,
         return_value_policy<reference_existing_object>())
    ;
  
  implicitly_convertible<Geometry::Ptr, Geometry::CPtr>();
}

//----------------------------------------------------------------------------//
