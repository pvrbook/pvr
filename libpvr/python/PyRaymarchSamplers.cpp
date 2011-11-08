//----------------------------------------------------------------------------//

/*! \file PyRaymarchSamplers.cpp
  Contains the interface definition for the RaymarchSamplers subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

#include <pvr/RaymarchSamplers/RaymarchSampler.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportRaymarchSamplers()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;

  using pvr::Render::RaymarchSampler;

  // RaymarchSampler ---

  class_<RaymarchSampler, RaymarchSampler::Ptr, boost::noncopyable>
    ("RaymarchSampler", no_init)
    .def("typeName", &RaymarchSampler::typeName)
    ;
  
  implicitly_convertible<RaymarchSampler::Ptr, RaymarchSampler::CPtr>();

}

//----------------------------------------------------------------------------//

