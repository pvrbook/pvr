//----------------------------------------------------------------------------//

/*! \file PyPartices.cpp
  Contains the interface definition for the Particles class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Library includes

#include <pvr/Particles.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;

using namespace pvr::Geo;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

AttrTable& pointAttrsHelper(Particles &self)
{
  return self.pointAttrs();
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportParticles()
{
  using namespace boost::python;

  class_<Particles, Particles::Ptr>("Particles", no_init)
    .def("__init__",    make_constructor(Particles::create))
    .def("resize",      &Particles::resize)
    .def("add",         &Particles::add)
    .def("setPosition", &Particles::setPosition)
    .def("pointAttrs",  &pointAttrsHelper,
         return_value_policy<reference_existing_object>())
    ;

  implicitly_convertible<Particles::Ptr, Particles::CPtr>();

}

//----------------------------------------------------------------------------//
