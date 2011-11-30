//----------------------------------------------------------------------------//

/*! \file PyTransmittanceMap.cpp
  Contains the interface definition for the Occluders subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>

// Library includes

#include <pvr/TransmittanceMap.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportTransmittanceMap()
{
  using namespace boost::python;

  using namespace pvr::Render;

  // TransmittanceMap ---

  class_<TransmittanceMap, TransmittanceMap::Ptr>
    ("TransmittanceMap", no_init)
    .def("__init__",      make_constructor(TransmittanceMap::create))
    .def("pixelFunction", &TransmittanceMap::pixelFunction)
    .def("printStats",    &TransmittanceMap::printStats)
    ;
  
  implicitly_convertible<TransmittanceMap::Ptr, 
                         TransmittanceMap::CPtr>();

}

//----------------------------------------------------------------------------//

