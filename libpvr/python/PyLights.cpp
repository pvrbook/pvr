//----------------------------------------------------------------------------//

/*! \file PyLights.cpp
  Contains the interface definition for the Lights subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

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
    .def("typeName",     &Light::typeName)
    .def("setIntensity", &Light::setIntensity)
    .def("setOccluder",  &Light::setOccluder)
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
    .def("__init__",    make_constructor(SpotLight::create))
    .def("setCamera",   &SpotLight::setCamera)
    .def("camera",      &SpotLight::camera)
    ;
  
  implicitly_convertible<SpotLight::Ptr, SpotLight::CPtr>();

}

//----------------------------------------------------------------------------//

