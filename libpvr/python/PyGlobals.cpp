//----------------------------------------------------------------------------//

/*! \file PyGlobals.cpp
  Contains the interface definition for the Globals class
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

// Project includes

#include <pvr/Globals.h>
#include <pvr/RenderGlobals.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportGlobals()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;

  // Globals ---

  class_<Sys::Globals>("Globals")
    ;

  // RenderGlobals ---

  class_<RenderGlobals>("RenderGlobals")
    .def("setupMotionBlur", &RenderGlobals::setupMotionBlur)
    .staticmethod("setupMotionBlur")
    .def("dt",  &RenderGlobals::dt).staticmethod("dt")
    .def("shutter",  &RenderGlobals::shutter).staticmethod("shutter")
    .def("fps", &RenderGlobals::fps).staticmethod("fps")
    ;

}

//----------------------------------------------------------------------------//
