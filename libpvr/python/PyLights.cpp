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

/*! \file PyLights.cpp
  Contains the interface definition for the Lights subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Needs to be first, to avoid macro "tolower" passed 2 arguments, on OSX, python 2.7
#include <Python.h>

// System includes

#include <boost/python.hpp>

// Library includes

#include <pvr/Lights/Light.h>
#include <pvr/Lights/PointLight.h>
#include <pvr/Lights/SpotLight.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportLights() 
{
  using namespace boost::python;

  using namespace pvr::Render;

  // Light ---

  class_<Light, Light::Ptr, boost::noncopyable>("Light", no_init)
    .def("typeName",          &Light::typeName)
    .def("setIntensity",      &Light::setIntensity)
    .def("setFalloffEnabled", &Light::setFalloffEnabled)
    .def("falloffEnabled",    &Light::falloffEnabled)
    .def("setOccluder",       &Light::setOccluder)
    .def("occluder",          &Light::occluder)
    ;
  
  implicitly_convertible<Light::Ptr, Light::CPtr>();

  // PointLight ---

  class_<PointLight, bases<Light>, PointLight::Ptr>("PointLight", no_init)
    .def("__init__",    make_constructor(PointLight::create))
    .def("setPosition", &PointLight::setPosition)
    .def("position",    &PointLight::position)
    ;
  
  implicitly_convertible<PointLight::Ptr, PointLight::CPtr>();

  // SpotLight ---

  class_<SpotLight, bases<Light>, SpotLight::Ptr>("SpotLight", no_init)
    .def("__init__",      make_constructor(SpotLight::create))
    .def("setCamera",     &SpotLight::setCamera)
    .def("camera",        &SpotLight::camera)
    .def("setConeAngles", &SpotLight::setConeAngles)
    ;
  
  implicitly_convertible<SpotLight::Ptr, SpotLight::CPtr>();

}

//----------------------------------------------------------------------------//

