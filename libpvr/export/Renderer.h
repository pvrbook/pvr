//-*-c++-*--------------------------------------------------------------------//

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
#include "pvr/TransmittanceMap.h"
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

  DECLARE_SMART_PTRS(Renderer);

  // Enums ---------------------------------------------------------------------

  // Exceptions ----------------------------------------------------------------  

  DECLARE_PVR_RT_EXC(MissingCameraException, "No camera found.");
  DECLARE_PVR_RT_EXC(MissingRaymarcherException, "No raymarcher found.");
  DECLARE_PVR_RT_EXC(MissingSceneException, "No scene created.");
  DECLARE_PVR_RT_EXC(MissingVolumeException, "No volume in scene.");

  // Constructor, destructor, factory ------------------------------------------

  //! Factory method. Use this for all objects that require lifetime management
  static Ptr create();
  //! Clones the Renderer. Will clear all the non-const data members but keeps
  //! pointers to const data members (volumes, lights, etc)
  Ptr clone();

  // Setup ---------------------------------------------------------------------

  //! Sets the camera to use for rendering
  void setCamera(Camera::CPtr camera);
  //! Sets the raymarcher to use for rendering
  void setRaymarcher(Raymarcher::CPtr raymarcher);
  //! Returns a pointer to the raymarcher
  Raymarcher::CPtr raymarcher() const;
  //! Adds a Volume object to the collection of volumes to be rendered
  void addVolume(Volume::CPtr volume);
  //! Adds a Light to the scene
  void addLight(Light::CPtr light);
  //! Prints the scene contents
  void printSceneInfo() const;

  // Options -------------------------------------------------------------------

  //! Whether to enable primary output. This is true for beauty renders
  //! and false when rendering a seconary pass, such as the transmittance map.
  void setPrimaryEnabled(const bool enabled);
  //! Sets whether to enable computation of the deep transmittance map.
  void setTransmittanceMapEnabled(const bool enabled);
  //! Sets whether to enable computation of the deep luminance map.
  void setLuminanceMapEnabled(const bool enabled);
  //! Sets whether to randomize pixel samples
  void setDoRandomizePixelSamples(const bool enabled);

  // Execution -----------------------------------------------------------------

  //! Executes the render
  void execute();

  // Results -------------------------------------------------------------------

  //! Returns a pointer to the transmittance map
  TransmittanceMap::Ptr transmittanceMap() const;
  //! Returns a pointer to the luminance map
  TransmittanceMap::Ptr luminanceMap() const;
  //! Saves the rendered image to the given filename
  void saveImage(const std::string &filename) const;

private:

  // Private methods -----------------------------------------------------------

  //! Creates a Ray for a given pixel and time.
  //! \param time Between shutter open and shutter close. Always in [0,1]
  Ray setupRay(const float x, const float y, const PTime time) const;
  //! Integrates a single ray and returns the result
  IntegrationResult integrateRay(const float x, const float y, 
                                 const PTime time) const;

  // Structs -------------------------------------------------------------------

  struct Params {
    Params();
    bool doPrimary;
    bool doLuminanceMap;
    bool doTransmittanceMap;
    bool doRandomizePixelSamples;
  };

  // Private data members ------------------------------------------------------

  //! Random number generator
  Imath::Rand48 m_rng;
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
  TransmittanceMap::Ptr m_transmittanceMap;
  //! Pointer to deep luminance map
  TransmittanceMap::Ptr m_luminanceMap;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
