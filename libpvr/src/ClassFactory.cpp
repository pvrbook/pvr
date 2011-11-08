//----------------------------------------------------------------------------//

/*! \file ClassFactory.cpp
  Contains implementations of ClassFactory class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/ClassFactory.h"

// System includes

// Project headers

#include "pvr/Log.h"

#include "pvr/Lights/Light.h"
#include "pvr/Occluders/Occluder.h"
#include "pvr/Primitives/InstantiationPrim.h"
#include "pvr/Primitives/RasterizationPrim.h"
#include "pvr/Raymarchers/Raymarcher.h"
#include "pvr/RaymarchSamplers/RaymarchSampler.h"
#include "pvr/Volumes/Volume.h"
#include "pvr/Globals.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace pvr;

  //--------------------------------------------------------------------------//

  template <typename Class_T>
  struct ClassTraits
  {
    static std::string className();
  };

  //--------------------------------------------------------------------------//

  template <>
  std::string ClassTraits<Render::Light>::className()
  { 
    return "Light";
  }

  //--------------------------------------------------------------------------//

  template <>
  std::string ClassTraits<Render::Occluder>::className()
  { 
    return "Occluder";
  }

  //--------------------------------------------------------------------------//

  template <>
  std::string ClassTraits<Render::Raymarcher>::className()
  { 
    return "Raymarcher";
  }

  //--------------------------------------------------------------------------//

  template <>
  std::string ClassTraits<Render::RaymarchSampler>::className()
  { 
    return "RaymarchSampler";
  }

  //--------------------------------------------------------------------------//

  template <>
  std::string ClassTraits<Model::Prim::Rast::RasterizationPrim>::className()
  { 
    return "RasterizationPrimitive";
  }

  //--------------------------------------------------------------------------//

  template <>
  std::string ClassTraits<Model::Prim::Inst::InstantiationPrim>::className()
  { 
    return "InstantiationPrimitive";
  }

  //--------------------------------------------------------------------------//

  template <>
  std::string ClassTraits<Render::Volume>::className()
  { 
    return "Volume";
  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Sys {

//----------------------------------------------------------------------------//
// ClassFactory template instantiations
//----------------------------------------------------------------------------//

template <typename Class_T>  
void ClassFactory<Class_T>::registerClass(CreateFnPtr createFn)
{
  ClassPtr instance = createFn();
  if (!instance) {
    Log::warning("Couldn't register class");
    return;
  }
  std::string typeName = instance->typeName();
  if (m_classes.find(typeName) != m_classes.end()) {
    Log::warning("Already registered class: " + typeName);
    return;
  }
  std::string baseClassName = ClassTraits<Class_T>::className();
  if (Sys::Globals::debugLogEnabled()) {
    Log::print("Registered " + baseClassName + " class: " + typeName);
  }
  m_classes[typeName] = createFn;
}

//----------------------------------------------------------------------------//

template <typename Class_T>
typename ClassFactory<Class_T>::ClassPtr 
ClassFactory<Class_T>::create(const std::string &typeName) const
{
  typename ClassMap::const_iterator i = m_classes.find(typeName);
  if (i != m_classes.end()) {
    return i->second();
  } else {
    Log::warning("Couldn't create object of type: " + typeName);
    return ClassPtr();
  }
}

//----------------------------------------------------------------------------//

template <typename Class_T>
typename ClassFactory<Class_T>::Ptr 
ClassFactory<Class_T>::singleton()
{
  if (!ms_instance) {
    ms_instance = Ptr(new ClassFactory);
  }
  return ms_instance;
}

//----------------------------------------------------------------------------//
// Template instantiations
//----------------------------------------------------------------------------//
  
#define PVR_CLASSFACTORY_INSTANTIATION(name)                    \
  template <>                                                   \
  ClassFactory<name>::Ptr                                       \
  ClassFactory<name>::ms_instance = ClassFactory<name>::Ptr();  \
  template class ClassFactory<name>;                            \
  
//----------------------------------------------------------------------------//

PVR_CLASSFACTORY_INSTANTIATION(Model::Prim::Inst::InstantiationPrim);
PVR_CLASSFACTORY_INSTANTIATION(Model::Prim::Rast::RasterizationPrim);
PVR_CLASSFACTORY_INSTANTIATION(Render::Light);
PVR_CLASSFACTORY_INSTANTIATION(Render::Occluder);
PVR_CLASSFACTORY_INSTANTIATION(Render::Volume);
PVR_CLASSFACTORY_INSTANTIATION(Render::RaymarchSampler);
PVR_CLASSFACTORY_INSTANTIATION(Render::Raymarcher);

//----------------------------------------------------------------------------//

} // namespace Sys
} // namespace pvr

//----------------------------------------------------------------------------//
