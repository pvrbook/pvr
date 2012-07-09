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

