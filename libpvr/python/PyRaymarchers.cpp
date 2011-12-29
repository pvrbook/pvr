//----------------------------------------------------------------------------//

/*! \file PyRaymarchers.cpp
  Contains the interface definition for the Raymarchers subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

#include <pvr/Raymarchers/Raymarcher.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

void setParamsHelper(pvr::Util::ParamBase &self, boost::python::dict d)
{
  self.setParams(pvr::dictToParamMap(d));
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportRaymarchers()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;
  
  using pvr::Render::Raymarcher;

  // ParamBase ---

  class_<Util::ParamBase, Util::ParamBase::Ptr, boost::noncopyable>
    ("ParamBase", no_init)
    .def("setParams", &setParamsHelper)
    ;

  // Raymarcher ---

  class_<Raymarcher, bases<Util::ParamBase>, 
         Raymarcher::Ptr, boost::noncopyable>
    ("Raymarcher", no_init)
    .def("typeName",           &Raymarcher::typeName)
    .def("setRaymarchSampler", &Raymarcher::setRaymarchSampler)
    ;
  
  implicitly_convertible<Raymarcher::Ptr, Raymarcher::CPtr>();

}

//----------------------------------------------------------------------------//

