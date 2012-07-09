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

#include <pvr/Camera.h>
#include <pvr/Globals.h>
#include <pvr/RenderGlobals.h>
#include <pvr/Scene.h>

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
    .def("scene", &RenderGlobals::scene).staticmethod("scene")
    .def("camera", &RenderGlobals::camera).staticmethod("camera")
    ;

}

//----------------------------------------------------------------------------//
