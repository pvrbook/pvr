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
#include <pvr/Occluders/OtfTransmittanceMapOccluder.h>
#include <pvr/Occluders/VoxelOccluder.h>
#include <pvr/Occluders/OtfVoxelOccluder.h>

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

  // NullOccluder ---

  class_<NullOccluder, bases<Occluder>, NullOccluder::Ptr>
    ("NullOccluder", no_init)
    .def("__init__", make_constructor(NullOccluder::create))
    ;
  
  implicitly_convertible<NullOccluder::Ptr, NullOccluder::CPtr>();

  // RaymarchOccluder ---

  class_<RaymarchOccluder, bases<Occluder>, RaymarchOccluder::Ptr>
    ("RaymarchOccluder", no_init)
    .def("__init__", make_constructor(RaymarchOccluder::create))
    ;
  
  implicitly_convertible<RaymarchOccluder::Ptr, RaymarchOccluder::CPtr>();

  // TransmittanceMapOccluder ---

  class_<TransmittanceMapOccluder, bases<Occluder>, 
         TransmittanceMapOccluder::Ptr>
    ("TransmittanceMapOccluder", no_init)
    .def("__init__", make_constructor(TransmittanceMapOccluder::create))
    ;
  
  implicitly_convertible<TransmittanceMapOccluder::Ptr, 
                         TransmittanceMapOccluder::CPtr>();

  // OtfTransmittanceMapOccluder ---

  class_<OtfTransmittanceMapOccluder, bases<Occluder>, 
         OtfTransmittanceMapOccluder::Ptr>
    ("OtfTransmittanceMapOccluder", no_init)
    .def("__init__", make_constructor(OtfTransmittanceMapOccluder::create))
    ;
  
  implicitly_convertible<OtfTransmittanceMapOccluder::Ptr, 
                         OtfTransmittanceMapOccluder::CPtr>();

  // VoxelOccluder ---

  class_<VoxelOccluder, bases<Occluder>, 
         VoxelOccluder::Ptr>
    ("VoxelOccluder", no_init)
    .def("__init__", make_constructor(VoxelOccluder::create))
    ;
  
  implicitly_convertible<VoxelOccluder::Ptr, 
                         VoxelOccluder::CPtr>();

  // OtfVoxelOccluder ---

  class_<OtfVoxelOccluder, bases<Occluder>, 
         OtfVoxelOccluder::Ptr>
    ("OtfVoxelOccluder", no_init)
    .def("__init__", make_constructor(OtfVoxelOccluder::create))
    ;
  
  implicitly_convertible<OtfVoxelOccluder::Ptr, 
                         OtfVoxelOccluder::CPtr>();

}

//----------------------------------------------------------------------------//

