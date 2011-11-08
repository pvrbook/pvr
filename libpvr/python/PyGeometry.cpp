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

AttrTable& objectAttrHelper(Geometry &self)
{
  return self.objectAttrs();
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
    .def("objectAttrs",  &objectAttrHelper,
         return_value_policy<reference_existing_object>())
    ;
}

//----------------------------------------------------------------------------//
