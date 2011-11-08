//----------------------------------------------------------------------------//

/*! \file PyOccluders.cpp
  Contains the interface definition for the Occluders subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>

// Library includes

#include <pvr/Occluders/Occluder.h>
#include <pvr/Occluders/RaymarchOccluder.h>
#include <pvr/Occluders/TransmittanceMapOccluder.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportOccluders()
{
  using namespace boost::python;

  using namespace pvr::Render;

  // Occluder ---

  class_<Occluder, Occluder::Ptr, boost::noncopyable>("Occluder", no_init)
    .def("typeName", &Occluder::typeName)
    ;
  
  implicitly_convertible<Occluder::Ptr, Occluder::CPtr>();

  // RaymarchOccluder ---

  class_<RaymarchOccluder, bases<Occluder>, RaymarchOccluder::Ptr>
    ("RaymarchOccluder", no_init)
    .def("__init__", make_constructor(RaymarchOccluder::create))
    .def("setRaymarcher", &RaymarchOccluder::setRaymarcher)
    ;
  
  implicitly_convertible<RaymarchOccluder::Ptr, RaymarchOccluder::CPtr>();

  // TransmittanceMapOccluder ---

  class_<TransmittanceMapOccluder, bases<Occluder>, 
         TransmittanceMapOccluder::Ptr>
    ("TransmittanceMapOccluder", no_init)
    .def("__init__", make_constructor(TransmittanceMapOccluder::create))
    .def("setTransmittanceMap", &TransmittanceMapOccluder::setTransmittanceMap)
    .def("setCamera",           &TransmittanceMapOccluder::setCamera)
    ;
  
  implicitly_convertible<TransmittanceMapOccluder::Ptr, 
                         TransmittanceMapOccluder::CPtr>();

}

//----------------------------------------------------------------------------//

