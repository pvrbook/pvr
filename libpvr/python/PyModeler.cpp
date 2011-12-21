//----------------------------------------------------------------------------//

/*! \file PyModeler.cpp
  Contains the interface definition for the Modeler class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/shared_ptr.hpp>

#include <boost/python.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Library includes

#include <pvr/Modeler.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using pvr::Model::Modeler;
using pvr::Model::ModelerInput;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

void (Modeler::*setRes1)(size_t)                 = &Modeler::setResolution;
void (Modeler::*setRes3)(size_t, size_t, size_t) = &Modeler::setResolution;

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportModeler()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;

  // Modeler ---

  class_<Modeler, Modeler::Ptr>("Modeler", no_init)
    .def("__init__",           make_constructor(Modeler::create))
    .def("clone",              &Modeler::clone)
    .def("setResolution",      setRes1)
    .def("setResolution",      setRes3)
    .def("setVoxelSize",       &Modeler::setVoxelSize)
    .def("setMapping",         &Modeler::setMapping)
    .def("setDataStructure",   &Modeler::setDataStructure)
    .def("setSparseBlockSize", &Modeler::setSparseBlockSize)
    .def("setCamera",          &Modeler::setCamera)
    .def("addInput",           &Modeler::addInput)
    .def("updateBounds",       &Modeler::updateBounds)
    .def("execute",            &Modeler::execute)
    .def("saveBuffer",         &Modeler::saveBuffer)
    .def("buffer",             &Modeler::buffer)
    ;

  enum_<Modeler::Mapping>("Mapping")
    .value("UniformMappingType",  Modeler::UniformMappingType)
    .value("FrustumMappingType", Modeler::FrustumMappingType)
    ;

  enum_<Modeler::DataStructure>("DataStructure")
    .value("DenseBufferType",  Modeler::DenseBufferType)
    .value("SparseBufferType", Modeler::SparseBufferType)
    ;

  enum_<Modeler::SparseBlockSize>("SparseBlockSize")
    .value("Size8",  Modeler::SparseBlockSize8)
    .value("Size16", Modeler::SparseBlockSize16)
    .value("Size32", Modeler::SparseBlockSize32)
    ;

}

//----------------------------------------------------------------------------//

void exportModelerInput()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;

  // Modeler ---

  class_<ModelerInput, ModelerInput::Ptr>("ModelerInput", no_init)
    .def("__init__",           make_constructor(ModelerInput::create))
    .def("setGeometry",        &ModelerInput::setGeometry)
    .def("setVolumePrimitive", &ModelerInput::setVolumePrimitive)
    .def("addFilter",          &ModelerInput::addFilter)
    ;

}

//----------------------------------------------------------------------------//
