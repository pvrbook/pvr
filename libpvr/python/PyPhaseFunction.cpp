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
    .def("typeName", &PhaseFunction::typeName)
    ;
  
  implicitly_convertible<PhaseFunction::Ptr, PhaseFunction::CPtr>();

#if 0

  // Isotropic ---

  class_<Isotropic, bases<PhaseFunction>, Isotropic::Ptr>
    ("Isotropic")
    .def("__init__", make_constructor(Isotropic::create))
    ;

  implicitly_convertible<Isotropic::Ptr, Isotropic::CPtr>();

  // HenyeyGreenstein ---

  class_<HenyeyGreenstein, bases<PhaseFunction>, HenyeyGreenstein::Ptr>
    ("HenyeyGreenstein")
    .def("__init__", make_constructor(HenyeyGreenstein::create))
    ;

  implicitly_convertible<HenyeyGreenstein::Ptr, HenyeyGreenstein::CPtr>();

  // Schlick ---

  class_<Schlick, bases<PhaseFunction>, Schlick::Ptr>
    ("Schlick")
    .def("__init__", make_constructor(Schlick::create))
    ;

  implicitly_convertible<Schlick::Ptr, Schlick::CPtr>();

  // Rayleigh ---

  class_<Rayleigh, bases<PhaseFunction>, Rayleigh::Ptr>
    ("Rayleigh")
    .def("__init__", make_constructor(Rayleigh::create))
    ;

  implicitly_convertible<Rayleigh::Ptr, Rayleigh::CPtr>();

  // Mie ---

  class_<Mie, bases<PhaseFunction>, Mie::Ptr>
    ("Mie")
    .def("__init__", make_constructor(Mie::create))
    ;

  implicitly_convertible<Mie::Ptr, Mie::CPtr>();

#endif

}

//----------------------------------------------------------------------------//

