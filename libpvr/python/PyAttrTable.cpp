//----------------------------------------------------------------------------//

/*! \file PyAttrTable.cpp
  Contains the interface definition for the AttrTable class
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

#include <pvr/AttrTable.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace pvr;
using namespace pvr::Geo;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

AttrTable::IntVec& intAttrElemsHelper(AttrTable &self, const AttrRef &ref)
{
  return self.intAttrElems(ref);
}

//----------------------------------------------------------------------------//

AttrTable::FloatVec& floatAttrElemsHelper(AttrTable &self, const AttrRef &ref)
{
  return self.floatAttrElems(ref);
}

//----------------------------------------------------------------------------//

AttrTable::VectorVec& vectorAttrElemsHelper(AttrTable &self, const AttrRef &ref)
{
  return self.vectorAttrElems(ref);
}

//----------------------------------------------------------------------------//

AttrTable::StringIdxVec& stringIdxAttrElemsHelper(AttrTable &self, 
                                                  const AttrRef &ref)
{
  return self.stringIdxAttrElems(ref);
}

//----------------------------------------------------------------------------//

AttrRef addIntAttr(AttrTable &self, const std::string &attrName, 
                   const size_t size, boost::python::list l)
{
  std::vector<int> defaults = pyListValues<int>(l);
  return self.addIntAttr(attrName, size, defaults);
}

//----------------------------------------------------------------------------//

AttrRef addFloatAttr(AttrTable &self, const std::string &attrName, 
                     const size_t size, boost::python::list l)
{
  std::vector<float> defaults = pyListValues<float>(l);
  return self.addFloatAttr(attrName, size, defaults);
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportAttrTable()
{
  using namespace boost::python;

  using namespace std;

  // AttrRef ---

  class_<AttrRef>("AttrRef")
    .def("isValid",          &AttrRef::isValid)
    .def("idx",              &AttrRef::idx)
    .def("arraySize",        &AttrRef::arraySize)
    ;

  // AttrTable ---

  class_<AttrTable, AttrTable::Ptr>("AttrTable", no_init)
    .def("__init__",           make_constructor(AttrTable::create))
    .def("resize",             &AttrTable::resize)
    .def("size",               &AttrTable::size)
    .def("addElems",           &AttrTable::addElems)
    .def("intAttrNames",       &AttrTable::intAttrNames)
    .def("floatAttrNames",     &AttrTable::floatAttrNames)
    .def("vectorAttrNames",    &AttrTable::vectorAttrNames)
    .def("stringAttrNames",    &AttrTable::stringAttrNames)
    .def("addIntAttr",         &addIntAttr)
    .def("addFloatAttr",       &addFloatAttr)
    .def("addVectorAttr",      &AttrTable::addVectorAttr)
    .def("addStringAttr",      &AttrTable::addStringAttr)
    .def("intAttrRef",         &AttrTable::intAttrRef)
    .def("floatAttrRef",       &AttrTable::floatAttrRef)
    .def("vectorAttrRef",      &AttrTable::vectorAttrRef)
    .def("stringAttrRef",      &AttrTable::stringAttrRef)
    .def("intAttr",            &AttrTable::intAttr)
    .def("floatAttr",          &AttrTable::floatAttr)
    .def("vectorAttr",         &AttrTable::vectorAttr)
    .def("stringIdxAttr",      &AttrTable::stringIdxAttr)
    .def("intAttrElems",       &intAttrElemsHelper,
         return_internal_reference<>())
    .def("floatAttrElems",     &floatAttrElemsHelper,
         return_internal_reference<>())
    .def("vectorAttrElems",    &vectorAttrElemsHelper,
         return_internal_reference<>())
    .def("stringIdxAttrElems", &stringIdxAttrElemsHelper,
         return_internal_reference<>())
    .def("setIntAttr",         &AttrTable::setIntAttr)
    .def("setFloatAttr",       &AttrTable::setFloatAttr)
    .def("setVectorAttr",      &AttrTable::setVectorAttr)
    .def("setStringIdxAttr",   &AttrTable::setStringIdxAttr)
    .def("stringAttr",         &AttrTable::stringAttr,
         return_value_policy<copy_const_reference>())
    .def("setStringAttr",      &AttrTable::setStringAttr)
    .def("stringFromTable",    &AttrTable::stringFromTable,
         return_value_policy<copy_const_reference>())
    .def("addStringToTable",   &AttrTable::addStringToTable)
    ;

}

//----------------------------------------------------------------------------//
