//----------------------------------------------------------------------------//

/*! \file PyDeepImage.cpp
  Contains the interface definition for the Occluders subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>

// Library includes

#include <pvr/DeepImage.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportDeepImage()
{
  using namespace boost::python;

  using namespace pvr::Render;

  // DeepImage ---

  class_<DeepImage, DeepImage::Ptr>
    ("DeepImage", no_init)
    .def("__init__",      make_constructor(DeepImage::create))
    .def("setNumSamples", &DeepImage::setNumSamples)
    .def("pixelFunction", &DeepImage::pixelFunction)
    .def("printStats",    &DeepImage::printStats)
    ;
  
  implicitly_convertible<DeepImage::Ptr, DeepImage::CPtr>();

}

//----------------------------------------------------------------------------//

