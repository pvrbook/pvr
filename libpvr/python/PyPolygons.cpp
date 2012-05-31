//----------------------------------------------------------------------------//

/*! \file PyPolygons.cpp
  Contains the interface definition for the Polygons class
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

#include <pvr/Polygons.h>
#include <pvr/Meshes.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;

using namespace pvr::Geo;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

AttrTable& polyAttrsHelper(Polygons &self)
{
  return self.polyAttrs();
}

//----------------------------------------------------------------------------//

AttrTable& pointAttrsHelper(Polygons &self)
{
  return self.pointAttrs();
}

//----------------------------------------------------------------------------//

AttrTable& meshAttrsHelper(Meshes &self)
{
  return self.meshAttrs();
}

//----------------------------------------------------------------------------//

AttrTable& meshPointAttrsHelper(Meshes &self)
{
  return self.pointAttrs();
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportPolygons()
{
  using namespace boost::python;

  class_<Polygons, Polygons::Ptr>("Polygons", no_init)
    .def("__init__",       make_constructor(Polygons::create))
    .def("size",           &Polygons::size)
    .def("addPoint",       &Polygons::addPoint)
    .def("addPolygon",     &Polygons::addPolygon)
    .def("numVertices",    &Polygons::numVertices)
    .def("setVertex",      &Polygons::setVertex)
    .def("pointForVertex", &Polygons::pointForVertex)
    .def("isClosed",       &Polygons::isClosed)
    .def("setIsClosed",    &Polygons::setIsClosed)
    .def("polyAttrs",      &polyAttrsHelper, 
         return_value_policy<reference_existing_object>())
    .def("pointAttrs",     &pointAttrsHelper, 
         return_value_policy<reference_existing_object>())
    .def("debugPrint",     &Polygons::print)
    ;

  implicitly_convertible<Polygons::Ptr, Polygons::CPtr>();

  class_<Meshes, Meshes::Ptr>("Meshes", no_init)
    .def("__init__",           make_constructor(Meshes::create))
    .def("size",               &Meshes::size)
    .def("addMesh",            &Meshes::addMesh)
    .def("numRows",            &Meshes::numRows)
    .def("numCols",            &Meshes::numCols)
    .def("startPoint",         &Meshes::startPoint)
    .def("pointForVertex",     &Meshes::pointForVertex)
    .def("meshAttrs",          &meshAttrsHelper, 
         return_value_policy<reference_existing_object>())
    .def("pointAttrs",         &meshPointAttrsHelper, 
         return_value_policy<reference_existing_object>())
    .def("computeDerivatives", &Meshes::computeDerivatives)
    .def("debugPrint",         &Meshes::print)
    ;

  implicitly_convertible<Meshes::Ptr, Meshes::CPtr>();

}

//----------------------------------------------------------------------------//
