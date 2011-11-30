//----------------------------------------------------------------------------//

/*! \file PyRenderer.cpp
  Contains the interface definition for the Renderer class
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

#include <pvr/Renderer.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportRenderer()
{
  using namespace boost::python;

  using pvr::Render::Renderer;

  // Renderer ---

  class_<Renderer, Renderer::Ptr>("Renderer", no_init)
    .def("__init__",                   make_constructor(Renderer::create))
    .def("clone",                      &Renderer::clone)
    .def("setCamera",                  &Renderer::setCamera)
    .def("setRaymarcher",              &Renderer::setRaymarcher)
    .def("addVolume",                  &Renderer::addVolume)
    .def("addLight",                   &Renderer::addLight)
    .def("setPrimaryEnabled",          &Renderer::setPrimaryEnabled)
    .def("setTransmittanceMapEnabled", &Renderer::setTransmittanceMapEnabled)
    .def("setLuminanceMapEnabled",     &Renderer::setLuminanceMapEnabled)
    .def("setDoRandomizePixelSamples", &Renderer::setDoRandomizePixelSamples)
    .def("execute",                    &Renderer::execute)
    .def("raymarcher",                 &Renderer::raymarcher)
    .def("transmittanceMap",           &Renderer::transmittanceMap)
    .def("luminanceMap",               &Renderer::luminanceMap)
    .def("saveImage",                  &Renderer::saveImage)
    ;

}

//----------------------------------------------------------------------------//
