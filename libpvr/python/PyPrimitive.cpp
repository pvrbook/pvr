//----------------------------------------------------------------------------//

/*! \file PyPrimitive.cpp
  Contains the interface definition for the Primitive class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

#include <boost/shared_ptr.hpp>

#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <pvr/Primitives/Primitive.h>
#include <pvr/Primitives/InstantiationPrim.h>
#include <pvr/Primitives/RasterizationPrim.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

void setParamsHelper(pvr::Model::Prim::Primitive &self, boost::python::dict d)
{
  self.setParams(pvr::dictToParamMap(d));
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportPrimitive()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;

  using namespace pvr::Model::Prim;
  using namespace pvr::Model::Prim::Inst;
  using namespace pvr::Model::Prim::Rast;

  // Primitive ---

  class_<Primitive, Primitive::Ptr, boost::noncopyable>
    ("Primitive", no_init)
    .def("typeName", &Primitive::typeName)
    .def("setParams", &setParamsHelper)
    ;
  implicitly_convertible<Primitive::Ptr, Primitive::CPtr>();

  // InstantiationPrim ---

  class_<InstantiationPrim, bases<Primitive>, 
         InstantiationPrim::Ptr, boost::noncopyable>
    ("InstantiationPrim", no_init)
    ;
  implicitly_convertible<InstantiationPrim::Ptr, InstantiationPrim::CPtr>();

  // RasterizationPrim ---

  class_<RasterizationPrim, bases<Primitive>, 
         RasterizationPrim::Ptr, boost::noncopyable>
    ("RasterizationPrim", no_init)
    ;
  implicitly_convertible<RasterizationPrim::Ptr, RasterizationPrim::CPtr>();

}

//----------------------------------------------------------------------------//

