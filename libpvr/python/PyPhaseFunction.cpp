//----------------------------------------------------------------------------//

/*! \file PyPhaseFunction.cpp
  Contains the interface definition for the PhaseFunction base and subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>

// Library includes

#include <pvr/PhaseFunction.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportPhaseFunction()
{
  using namespace boost::python;
  using namespace pvr::Render::Phase;

  // PhaseFunction ---

  class_<PhaseFunction, PhaseFunction::Ptr, boost::noncopyable>
    ("PhaseFunction", no_init)
    .def("typeName",    &PhaseFunction::typeName)
    .def("probability", &PhaseFunction::probability)
    ;
  
  implicitly_convertible<PhaseFunction::Ptr, PhaseFunction::CPtr>();

  // Isotropic ---

  class_<Isotropic, bases<PhaseFunction>, Isotropic::Ptr>
    ("Isotropic")
    .def("__init__", make_constructor(Isotropic::create))
    ;

  implicitly_convertible<Isotropic::Ptr, Isotropic::CPtr>();

  // HenyeyGreenstein ---

  class_<HenyeyGreenstein, bases<PhaseFunction>, HenyeyGreenstein::Ptr>
    ("HenyeyGreenstein", no_init)
    .def("__init__", make_constructor(HenyeyGreenstein::create))
    ;

  implicitly_convertible<HenyeyGreenstein::Ptr, HenyeyGreenstein::CPtr>();

  // DoubleHenyeyGreenstein ---

  class_<DoubleHenyeyGreenstein, bases<PhaseFunction>, 
    DoubleHenyeyGreenstein::Ptr>
    ("DoubleHenyeyGreenstein", no_init)
    .def("__init__", make_constructor(DoubleHenyeyGreenstein::create))
    ;

  implicitly_convertible<DoubleHenyeyGreenstein::Ptr, 
                         DoubleHenyeyGreenstein::CPtr>();


}

//----------------------------------------------------------------------------//

