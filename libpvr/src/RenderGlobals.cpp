//----------------------------------------------------------------------------//

/*! \file RenderGlobals.cpp
  Contains implementations of RenderGlobals class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/RenderGlobals.h"

// System includes

#include <stdlib.h>

// Project includes

#include "pvr/Log.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// RenderGlobals static member instantiation
//----------------------------------------------------------------------------//

float RenderGlobals::ms_fps = 24.0f;
float RenderGlobals::ms_dt = 0.0f;
float RenderGlobals::ms_shutter = 0.0f;

RenderGlobals::SceneCPtr RenderGlobals::ms_scene;
RenderGlobals::CameraCPtr RenderGlobals::ms_camera;

//----------------------------------------------------------------------------//
// RenderGlobals implementations
//----------------------------------------------------------------------------//

void RenderGlobals::setupMotionBlur(const float fps, const float shutter)
{
  // Error handling and sanity checks
  if (fps <= 0.0f) {
    throw BadFpsException("Zero or negative value.");
  } 
  if (shutter < 0.0f) {
    throw BadShutterException("Negative value.");
  }
  if (shutter > 1.0f) {
    Log::warning("Shutter time greater than 1.0: " + str(shutter));
  }

  ms_fps = fps;
  ms_shutter = shutter;
  ms_dt = 1.0 / fps * shutter;
}

//----------------------------------------------------------------------------//

void RenderGlobals::setScene(SceneCPtr scene)
{
  ms_scene = scene;
}

//----------------------------------------------------------------------------//

void RenderGlobals::setCamera(CameraCPtr camera)
{
  ms_camera = camera;
}

//----------------------------------------------------------------------------//

float RenderGlobals::fps()
{ 
  return ms_fps; 
}

//----------------------------------------------------------------------------//

float RenderGlobals::shutter() 
{ 
  return ms_shutter; 
}

//----------------------------------------------------------------------------//

float RenderGlobals::dt()
{ 
  return ms_dt; 
}

//----------------------------------------------------------------------------//

RenderGlobals::SceneCPtr RenderGlobals::scene()
{ 
  return ms_scene; 
}

//----------------------------------------------------------------------------//

RenderGlobals::CameraCPtr RenderGlobals::camera()
{ 
  return ms_camera; 
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//
