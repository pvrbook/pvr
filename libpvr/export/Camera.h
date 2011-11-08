//-*-c++-*--------------------------------------------------------------------//

/*! \file Camera.h
  Contains the Camera class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_CAMERA_H__
#define __INCLUDED_PVR_CAMERA_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <cmath>
#include <vector>

// Library headers

#include <boost/lexical_cast.hpp>

#include <OpenEXR/ImathQuat.h>
#include <OpenEXR/ImathVec.h>

// Project headers

#include "pvr/Curve.h"
#include "pvr/Exception.h"
#include "pvr/Time.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Defines
//----------------------------------------------------------------------------//

#define PVR_DEFINE_STATIC_CLONE_FUNC(className)                 \
  static className::Ptr staticClone(className::CPtr object)     \
  { return className::Ptr(object->rawClone()); }                \

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

/*! \class Camera
  \brief Base class for cameras. 

  Defines the common operations for all camera types, i.e. position, 
  orientation, resolution. Also defines the base worldToCamera/cameraToWorld
  transforms and defines the common interface that subclasses must implement.

  A note on the definition of time. The start time of the current frame being
  rendered is always 0.0. The end of the shutter open/close interval is
  always Globals::dt(). 

  The time in the curves that describe the camera's motion are all defined
  in PTime.
 */

//----------------------------------------------------------------------------//

class Camera
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Camera);

  // Exceptions ----------------------------------------------------------------

  //! Thrown if zero number of time samples given
  DECLARE_PVR_RT_EXC(TimeSamplesException, "Invalid number of time samples:");

  // Constructor, destructor, factory ------------------------------------------

  //! Constructs an empty Camera object. We don't pass configuration params
  //! to the constructor because we need to call virtual functions to recompute
  //! the transforms after setting the parameters. Calling virtual functions
  //! in constructors is bad.
  Camera();

  //! Virtual desctructor
  virtual ~Camera();

  // Main methods --------------------------------------------------------------

  //! Sets the (potentially time-varying) position
  void setPosition(const Util::VectorCurve &curve);
  //! Sets the (potentially time-varying) orientation
  void setOrientation(const Util::QuatCurve &curve);
  //! Sets the resolution of the camera. This also defines the aspect ratio of 
  //! the frame.
  void setResolution(const Imath::V2i &resolution);

  //! Returns the position at the given time
  Vector position(const PTime time) const;
  //! Returns the resolution of the camera
  const Imath::V2i& resolution() const;

  //! Sets the number of time samples
  void setNumTimeSamples(const uint numSamples);

  //! Returns the camera-space coordinate given a world-space coordinate.
  Vector worldToCamera(const Vector &wsP, const PTime time) const;
  //! Returns the world-space coordinate given a camera-space coordinate.
  Vector cameraToWorld(const Vector &csP, const PTime time) const;

  //! Returns the screen-space coordinate given a world-space coordinate.
  virtual Vector worldToScreen(const Vector &wsP, const PTime time) const = 0;
  //! Returns the world-space coordinate given a screen-space coordinate.
  virtual Vector screenToWorld(const Vector &ssP, const PTime time) const = 0;
  //! Returns the raster-space coordinate given a world-space coordinate.
  virtual Vector worldToRaster(const Vector &wsP, const PTime time) const = 0;
  //! Returns the world-space coordinate given a raster-space coordinate.
  virtual Vector rasterToWorld(const Vector &rsP, const PTime time) const = 0;

  // Cloning -------------------------------------------------------------------

  Camera::Ptr clone() const
  { return Camera::Ptr(rawClone()); }

protected:
  
  // Cloning -------------------------------------------------------------------

  virtual Camera* rawClone() const = 0;

  // Utility methods -----------------------------------------------------------

  //! Should be called whenever a data member is altered that will affect the
  //! transformation matrices of the camera.
  virtual void recomputeTransforms();

  //! Transforms the given point by interpolating between the two closest
  //! matrix transformations, given a [0,1] parametric time sample
  Vector transformPoint(const Vector &p, const std::vector<Matrix> &matrices,
                        const PTime time) const;

  //! Computes the camera to world transform at the given time
  //! \param time Time in [0,1] range
  Matrix computeCameraToWorld(const PTime time) const;

  // Protected data members ----------------------------------------------------

  //! Animation curve for camera position
  Util::VectorCurve m_position;
  //! Animation curve for camera orientation
  Util::QuatCurve m_orientation;

  //! Resolution of camera
  Imath::V2i m_resolution;

  //! Number of time samples to use.
  uint m_numSamples;

  //! Transformation matrices representing camera to world transform
  //! for the [0,dt] time interval
  std::vector<Matrix> m_cameraToWorld;
  //! Transformation matrices representing world to camera transform
  //! for the [0,dt] time interval
  std::vector<Matrix> m_worldToCamera;
  
};

//----------------------------------------------------------------------------//
// PerspectiveCamera
//----------------------------------------------------------------------------//

/*! \brief A simple perspective-projection camera

 */

//----------------------------------------------------------------------------//

class PerspectiveCamera : public Camera
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(PerspectiveCamera);

  // Constructor, destructor, factory ------------------------------------------

  //! Constructs a default PerspectiveCamera 
  PerspectiveCamera();

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  static Ptr create();

  // Main methods --------------------------------------------------------------

  //! Sets the clip planes of the camera. These are constant in time.
  void setClipPlanes(const double near, const double far);
  //! Sets the animation curve for the vertical field-of-view. 
  void setVerticalFOV(const Util::FloatCurve &curve);

  // From Camera ---------------------------------------------------------------
  
  PVR_DEFINE_STATIC_CLONE_FUNC(PerspectiveCamera);

  virtual Vector worldToScreen(const Vector &wsP, const PTime time) const;
  virtual Vector screenToWorld(const Vector &ssP, const PTime time) const;
  virtual Vector worldToRaster(const Vector &wsP, const PTime time) const;
  virtual Vector rasterToWorld(const Vector &rsP, const PTime time) const;

protected:
  
  // From Camera ---------------------------------------------------------------

  virtual void recomputeTransforms();

  // Utility methods -----------------------------------------------------------

  //! Computes the camera to screen transform at the given time
  //! \param time Time in [0,1] range
  void getTransforms(const PTime time, Matrix &cameraToScreen,
                     Matrix &screenToRaster) const;

  // Protected data members ----------------------------------------------------

  //! Vertical field of view. Time-varying in order to support motion blur due 
  //! to zooming cameras.
  Util::FloatCurve m_verticalFOV;  

  //! Near plane of projection
  double m_near;
  //! Far plane of projection
  double m_far;

  //! Transformation matrix representing world to screen transform
  std::vector<Matrix> m_worldToScreen;
  //! Transformation matrix representing screen to world transform
  std::vector<Matrix> m_screenToWorld;
  //! Transformation matrix representing world to raster transform
  std::vector<Matrix> m_worldToRaster;
  //! Transformation matrix representing raster to world transform
  std::vector<Matrix> m_rasterToWorld;

private:

  // From Camera ---------------------------------------------------------------
  
  virtual PerspectiveCamera* rawClone() const
  { return new PerspectiveCamera(*this); }

};

//----------------------------------------------------------------------------//

//! Calculates vertical field of view.
//! \param focalLength The camera's focal length
//! \param hAperture The horizontal film gate size. This is 0.980in = 2.4892cm
//! on a normal Super35 setup.
//! \param resolution Image resolution, used to calculate aspect ratio of the
//! frame.
double calculateVerticalFOV(const double focalLength, const double hAperture,
                            const Imath::V2i &resolution);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
