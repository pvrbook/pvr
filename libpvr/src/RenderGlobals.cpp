//----------------------------------------------------------------------------//

/*
    This file is part of PVR. Copyright (C) 2012 Magnus Wrenninge

    PVR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PVR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
