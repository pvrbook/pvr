//----------------------------------------------------------------------------//

/*! \file PyClassPool.cpp
  Contains the interface definition for the ClassFactory class
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

#include <pvr/ClassFactory.h>
#include <pvr/PluginLoader.h>
#include <pvr/Primitives/RasterizationPrim.h>
#include <pvr/Primitives/InstantiationPrim.h>
#include <pvr/Raymarchers/Raymarcher.h>
#include <pvr/RaymarchSamplers/RaymarchSampler.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

template <typename Class_T>
typename Class_T::Ptr
classFactoryHelper(const std::string &typeName)
{
  typedef Sys::ClassFactory<Class_T> Factory;
  return Factory::singleton()->create(typeName);
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportClassFactory()
{
  using namespace boost::python;
  using namespace pvr::Model::Prim;
  using namespace pvr::Render;

  registerStandardTypes();

  def("createRasterizationPrim", &classFactoryHelper<Rast::RasterizationPrim>);
  def("createInstantiationPrim", &classFactoryHelper<Inst::InstantiationPrim>);
  def("createVolume",            &classFactoryHelper<Volume>);
  def("createRaymarcher",        &classFactoryHelper<Raymarcher>);
  def("createRaymarchSampler",   &classFactoryHelper<RaymarchSampler>);
}

//----------------------------------------------------------------------------//
