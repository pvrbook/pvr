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

/*! \file Renderer.h
  Contains the Renderer class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RENDERER_H__
#define __INCLUDED_PVR_RENDERER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <OpenEXR/ImathRandom.h>

// Project headers

#include "pvr/Camera.h"
#include "pvr/Image.h"
#include "pvr/Exception.h"
#include "pvr/Scene.h"
#include "pvr/DeepImage.h"
#include "pvr/Types.h"

#include "pvr/Raymarchers/Raymarcher.h"
#include "pvr/Lights/Light.h"
#include "pvr/Volumes/Volume.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Renderer
//----------------------------------------------------------------------------//

/*! \class Renderer
  \brief The Renderer class is the core of PVR's volume rendering pipeline. 

  See chapter 'PVR's rendering pipeline'.

 */

//----------------------------------------------------------------------------//

class Renderer
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Renderer);

  // Enums ---------------------------------------------------------------------

  // Exceptions ----------------------------------------------------------------  

  DECLARE_PVR_RT_EXC(MissingCameraException, "No camera found.");
  DECLARE_PVR_RT_EXC(MissingRaymarcherException, "No raymarcher found.");
  DECLARE_PVR_RT_EXC(MissingSceneException, "No scene created.");
  DECLARE_PVR_RT_EXC(MissingVolumeException, "No volume in scene.");

  // Constructor, destructor, factory ------------------------------------------

  //! Default constructor
  Renderer();
  //! Factory method. Use this for all objects that require lifetime management
  static Ptr create();
  //! Clones the Renderer. Will clear all the non-const data members but keeps
  //! pointers to const data members (volumes, lights, etc)
  Ptr clone() const;

  // Setup ---------------------------------------------------------------------

  //! Sets the camera to use for rendering
  void setCamera    (Camera::CPtr camera);
  //! Sets the raymarcher to use for rendering
  void setRaymarcher(Raymarcher::CPtr raymarcher);
  //! Adds a Volume object to the collection of volumes to be rendered
  void addVolume    (Volume::CPtr volume);
  //! Adds a Light to the scene
  void addLight     (Light::CPtr light);

  // Access --------------------------------------------------------------------

  //! Returns a pointer to the raymarcher
  Raymarcher::CPtr raymarcher() const;
  //! Returns a pointer to the Scene instance
  Scene::Ptr       scene() const;  
  //! Returns the number of pixel samples to use
  size_t           numPixelSamples() const;
  
  // Options -------------------------------------------------------------------

  //! Whether to enable primary output. This is true for beauty renders
  //! and false when rendering a secondary pass, such as the transmittance map.
  void setPrimaryEnabled         (const bool enabled);
  //! Sets whether to enable computation of the deep transmittance map.
  void setTransmittanceMapEnabled(const bool enabled);
  //! Sets whether to enable computation of the deep luminance map.
  void setLuminanceMapEnabled    (const bool enabled);
  //! Sets whether to randomize pixel samples
  void setDoRandomizePixelSamples(const bool enabled);
  //! Sets the number of pixel samples to use
  //! \note The number of rays fired will be the square of this number
  void setNumPixelSamples        (const size_t numSamples);
  //! Sets the number of samples to use for deep images (transmittance and
  //! luminance)
  void setNumDeepSamples         (const size_t numSamples);

  // Execution -----------------------------------------------------------------

  //! Prints the scene contents
  void printSceneInfo() const;
  //! Executes the render
  void execute();

  // Ray server ----------------------------------------------------------------

  //! Traces a single arbitrary ray.
  //! \note Does not change current camera.
  IntegrationResult trace(const RayState &state) const;

  // Results -------------------------------------------------------------------

  //! Returns a pointer to the transmittance map
  DeepImage::Ptr transmittanceMap() const;
  //! Returns a pointer to the luminance map
  DeepImage::Ptr luminanceMap() const;
  //! Saves the rendered image to the given filename
  void           saveImage(const std::string &filename) const;

private:

  // Private methods -----------------------------------------------------------

  //! Integrates a single ray and returns the result
  IntegrationResult integrateRay(const float x, const float y, 
                                 const PTime time) const;
  //! Configures the next pixel sample
  void setupSample(const float xCenter, const float yCenter,
                   const size_t xSubpixel, const size_t ySubpixel, 
                   float &xSample, float &ySample, PTime &pTime) const;

  // Structs -------------------------------------------------------------------

  struct Params {
    Params();
    bool doPrimary;
    bool doLuminanceMap;
    bool doTransmittanceMap;
    bool doRandomizePixelSamples;
    size_t numPixelSamples;
  };

  // Private data members ------------------------------------------------------

  //! Random number generator
  mutable Imath::Rand48 m_rng;
  //! Renderer parameters
  Params m_params;
  //! Pointer to scene
  Scene::Ptr m_scene;
  //! Pointer to primary camera
  Camera::CPtr m_camera;
  //! Pointer to raymarcher instance
  Raymarcher::CPtr m_raymarcher;
  //! Primary image output. 
  Image::Ptr m_primary;
  //! Pointer to deep transmittance map
  DeepImage::Ptr m_deepTransmittance;
  //! Pointer to deep luminance map
  DeepImage::Ptr m_deepLuminance;
};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

//! Creates a Ray for a given pixel and time.
//! \param time Between shutter open and shutter close. Always in [0,1]
Ray setupRay(Camera::CPtr camera, const float x, const float y, 
             const PTime time);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
