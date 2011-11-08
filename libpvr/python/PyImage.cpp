//----------------------------------------------------------------------------//

/*! \file PyImage.cpp
  Contains the interface definition for the Image class
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

#include <pvr/Image.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportImage()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;
  using pvr::Render::Image;

  // Image ---

  class_<Image, Image::Ptr>("Image", no_init)
    .def("__init__",      make_constructor(Image::create))
    .def("setSize",       &Image::setSize)
    .def("setPixel",      &Image::setPixel)
    .def("setPixelAlpha", &Image::setPixelAlpha)
    .def("pixel",         &Image::pixel)
    .def("pixelAlpha",    &Image::pixelAlpha)
    .def("write",         &Image::write)
    ;

  enum_<Image::Channels>("Channels")
    .value("RGB", Image::RGB)
    .value("RGBA", Image::RGBA)
    ;

}

//----------------------------------------------------------------------------//
