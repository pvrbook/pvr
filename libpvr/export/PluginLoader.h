//-*-c++-*--------------------------------------------------------------------//

/*! \file PluginLoader.h
  Contains the PluginLoader class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PLUGINLOADER_H__
#define __INCLUDED_PVR_PLUGINLOADER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

// Project includes

#include "pvr/ClassFactory.h"

#include "pvr/FrustumMapping.h"

#include "pvr/Lights/PointLight.h"
#include "pvr/Occluders/RaymarchOccluder.h"
#include "pvr/Occluders/TransmittanceMapOccluder.h"
#include "pvr/Primitives/Rasterization/Point.h"
#include "pvr/Primitives/Rasterization/PyroclasticPoint.h"
#include "pvr/Primitives/Rasterization/Line.h"
#include "pvr/Primitives/Rasterization/PyroclasticLine.h"
#include "pvr/Primitives/Instantiation/Line.h"
#include "pvr/Primitives/Instantiation/Sphere.h"
#include "pvr/Primitives/Instantiation/Surface.h"
#include "pvr/Raymarchers/NaiveRaymarcher.h"
#include "pvr/RaymarchSamplers/DensitySampler.h"
#include "pvr/RaymarchSamplers/ScatteringSampler.h"
#include "pvr/Volumes/VoxelVolume.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

template <typename Base_T, typename SubClass_T>
typename Base_T::Ptr
factoryFunc()
{
  return SubClass_T::create();
}

//----------------------------------------------------------------------------//
// PluginLoader
//----------------------------------------------------------------------------//

void registerStandardTypes()
{
  using namespace Sys;
  using namespace Model;
  using namespace Render;
  using namespace Model::Prim;

  // RasterizationPrim classes
  {
    ClassFactory<Rast::RasterizationPrim>::Ptr factory = 
      ClassFactory<Rast::RasterizationPrim>::singleton();
    
    factory->registerClass(factoryFunc<Rast::RasterizationPrim, 
                           Rast::Point>);
    factory->registerClass(factoryFunc<Rast::RasterizationPrim, 
                           Rast::PyroclasticPoint>);
    factory->registerClass(factoryFunc<Rast::RasterizationPrim, 
                           Rast::Line>);
    factory->registerClass(factoryFunc<Rast::RasterizationPrim, 
                           Rast::PyroclasticLine>);
  }

  // InstantiationPrim classes
  {
    ClassFactory<Inst::InstantiationPrim>::Ptr factory = 
      ClassFactory<Inst::InstantiationPrim>::singleton();
    
    factory->registerClass(factoryFunc<Inst::InstantiationPrim, 
                                       Inst::Sphere>);
    factory->registerClass(factoryFunc<Inst::InstantiationPrim, 
                                       Inst::Line>);
    factory->registerClass(factoryFunc<Inst::InstantiationPrim, 
                                       Inst::Surface>);
  }

  // Light classes
  {
    ClassFactory<Light>::Ptr factory = 
      ClassFactory<Light>::singleton();
    
    factory->registerClass(factoryFunc<Light, PointLight>);
  }

  // Occluder classes
  {
    ClassFactory<Occluder>::Ptr factory = 
      ClassFactory<Occluder>::singleton();
    
    factory->registerClass(factoryFunc<Occluder, NullOccluder>);
    factory->registerClass(factoryFunc<Occluder, RaymarchOccluder>);
    factory->registerClass(factoryFunc<Occluder, TransmittanceMapOccluder>);
  }

  // Raymarcher classes
  {
    ClassFactory<Raymarcher>::Ptr factory = 
      ClassFactory<Raymarcher>::singleton();
    
    factory->registerClass(factoryFunc<Raymarcher, NaiveRaymarcher>);
  }

  // RaymarchSampler classes
  {
    ClassFactory<RaymarchSampler>::Ptr factory = 
      ClassFactory<RaymarchSampler>::singleton();
    
    factory->registerClass(factoryFunc<RaymarchSampler, DensitySampler>);
    factory->registerClass(factoryFunc<RaymarchSampler, ScatteringSampler>);
  }

  // Volume classes
  {
    ClassFactory<Volume>::Ptr factory = 
      ClassFactory<Volume>::singleton();
    
    factory->registerClass(factoryFunc<Volume, VoxelVolume>);
  }
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
