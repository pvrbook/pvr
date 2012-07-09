//-*-c++-*--------------------------------------------------------------------//

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

/*! \file RenderGlobals.h
  Contains the RenderGlobals class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RENDERGLOBALS_H__
#define __INCLUDED_PVR_RENDERGLOBALS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

#include "pvr/Exception.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Forward declarations
//----------------------------------------------------------------------------//

  namespace Render {
    class Scene;
    class Camera;
  }

//----------------------------------------------------------------------------//
// RenderGlobals
//----------------------------------------------------------------------------//

class RenderGlobals 
{
public:
  
  // Typedefs ------------------------------------------------------------------

  typedef boost::shared_ptr<const pvr::Render::Scene> SceneCPtr;
  typedef boost::shared_ptr<const pvr::Render::Camera> CameraCPtr;

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(BadFpsException, "Bad frames per second value:");
  DECLARE_PVR_RT_EXC(BadShutterException, "Bad shutter value:");

  // Main methods --------------------------------------------------------------

  //! Sets the motion blur settings
  //! \param fps Frames per second. Should be 24 for film
  //! \param shutter Fraction of frame length that shutter is open. A value 
  //! of 0.5 corresponsonds to a 180 degree shutter angle.
  static void       setupMotionBlur(const float fps, const float shutter);
  //! Sets the current Scene instance
  static void       setScene(SceneCPtr scene);
  //! Sets the current Camera
  static void       setCamera(CameraCPtr camera);

  // Accessors -----------------------------------------------------------------

  //! Returns the number of frames per second
  static float      fps();
  //! Returns the shutter length
  static float      shutter();
  //! Returns length (in seconds) of open shutter time.
  static float      dt();
  //! Returns a pointer to the current scene
  static SceneCPtr  scene();
  //! Returns a pointer to the current camera
  static CameraCPtr camera();

private:

  // Data members --------------------------------------------------------------

  static float      ms_fps;
  static float      ms_shutter;
  static float      ms_dt;
  static SceneCPtr  ms_scene;
  static CameraCPtr ms_camera;

};

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
