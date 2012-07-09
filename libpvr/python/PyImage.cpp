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
