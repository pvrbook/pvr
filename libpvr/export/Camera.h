//-*-c++-*--------------------------------------------------------------------//

/*
  PVR source code Copyright(c) 2012 Magnus Wrenninge

  This file is part of PVR.

  PVR is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.  Note that the text contents of
  the book "Production Volume Rendering" are *not* licensed under the
  GNU GPL.

  PVR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

//----------------------------------------------------------------------------//

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

#include "pvr/export.h"
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

  A note on the definition of time. The start time (in seconds) of the current 
  frame being rendered is always 0.0. The end time (in seconds) of the shutter 
  open/close interval is always Globals::dt(). 

  PTime refers to the time between shutter open and shutter close, which 
  defines its own [0,1] interval.

  It is assumed that the Curve<T> instances used to provide parameters to
  the camera are adjusted so that 0.0 refers to shutter open and 1.0 refers to
  shutter close time.
 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC Camera
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Camera);

  typedef std::vector<Matrix> MatrixVec;

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

  // Placement -----------------------------------------------------------------

  //! Sets the (potentially time-varying) position
  void setPosition(const Util::VectorCurve &curve);
  //! Returns the position at the given time
  Vector position(const PTime time) const;
  //! Sets the (potentially time-varying) orientation
  void setOrientation(const Util::QuatCurve &curve);
  //! Returns the position at the given time
  Quat orientation(const PTime time) const;

  // Resolution ----------------------------------------------------------------

  //! Sets the resolution of the camera. This also defines the aspect ratio of 
  //! the frame.
  void setResolution(const Imath::V2i &resolution);
  //! Returns the resolution of the camera
  const Imath::V2i& resolution() const;

  // Motion blur settings ------------------------------------------------------

  //! Sets the number of time samples
  void setNumTimeSamples(const unsigned int numSamples);
  //! Returns the number of time samples
  unsigned int numTimeSamples() const ;

  // Transforms ----------------------------------------------------------------

  //! Returns the camera-space coordinate given a world-space coordinate.
  Vector worldToCamera(const Vector &wsP, const PTime time) const;
  //! Returns the world-space coordinate given a camera-space coordinate.
  Vector cameraToWorld(const Vector &csP, const PTime time) const;

  //! Returns the world to camera transform matrices
  const MatrixVec& worldToCameraMatrices() const;
  //! Returns the camera to world transform matrices
  const MatrixVec& cameraToWorldMatrices() const;

  // To be implemented by subclasses -------------------------------------------

  //! Returns the screen-space coordinate given a world-space coordinate.
  virtual Vector worldToScreen(const Vector &wsP, const PTime time) const = 0;
  //! Returns the world-space coordinate given a screen-space coordinate.
  virtual Vector screenToWorld(const Vector &ssP, const PTime time) const = 0;
  //! Returns the raster-space coordinate given a world-space coordinate.
  virtual Vector worldToRaster(const Vector &wsP, const PTime time) const = 0;
  //! Returns the world-space coordinate given a raster-space coordinate.
  virtual Vector rasterToWorld(const Vector &rsP, const PTime time) const = 0;
  //! Whether the camera's transforms are well-defined behind the camera,
  //! i.e. where csP.z < 0.0
  virtual bool canTransformNegativeCamZ() const = 0;

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
  Vector transformPoint(const Vector &p, const MatrixVec &matrices,
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
  unsigned int m_numSamples;

  //! Transformation matrices representing camera to world transform
  //! for the [0,dt] time interval
  MatrixVec m_cameraToWorld;
  //! Transformation matrices representing world to camera transform
  //! for the [0,dt] time interval
  MatrixVec m_worldToCamera;
  
};

//----------------------------------------------------------------------------//
// PerspectiveCamera
//----------------------------------------------------------------------------//

/*! \brief A simple perspective-projection camera

 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC PerspectiveCamera : public Camera
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(PerspectiveCamera);

  // Constructor, destructor, factory ------------------------------------------

  //! Constructs a default PerspectiveCamera 
  PerspectiveCamera();

  PVR_DEFINE_CREATE_FUNC(PerspectiveCamera);

  // Main methods --------------------------------------------------------------

  //! Sets the clip planes of the camera. These are constant in time.
  void setClipPlanes(const double near, const double far);
  //! Sets the animation curve for the vertical field-of-view. 
  void setVerticalFOV(const Util::FloatCurve &curve);

  // Access to transformation matrices -----------------------------------------

  //! Returns the world to screen transform matrices
  const MatrixVec& worldToScreenMatrices() const;
  //! Returns the world to screen transform matrices
  const MatrixVec& screenToWorldMatrices() const;

  // From Camera ---------------------------------------------------------------
  
  PVR_DEFINE_STATIC_CLONE_FUNC(PerspectiveCamera);

  virtual Vector worldToScreen(const Vector &wsP, const PTime time) const;
  virtual Vector screenToWorld(const Vector &ssP, const PTime time) const;
  virtual Vector worldToRaster(const Vector &wsP, const PTime time) const;
  virtual Vector rasterToWorld(const Vector &rsP, const PTime time) const;
  virtual bool canTransformNegativeCamZ() const;

  // Cloning -------------------------------------------------------------------

  PerspectiveCamera::Ptr clone() const
  { return PerspectiveCamera::Ptr(rawClone()); }

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
  MatrixVec m_worldToScreen;
  //! Transformation matrix representing screen to world transform
  MatrixVec m_screenToWorld;
  //! Transformation matrix representing world to raster transform
  MatrixVec m_worldToRaster;
  //! Transformation matrix representing raster to world transform
  MatrixVec m_rasterToWorld;

private:

  // Cloning -------------------------------------------------------------------
  
  virtual PerspectiveCamera* rawClone() const
  { return new PerspectiveCamera(*this); }

};

//----------------------------------------------------------------------------//
// SphericalCoords
//----------------------------------------------------------------------------//

//! Represents spherical coordinates
//! The spherical coordinate system is aligned such that zero longitude, zero
//! latitude corresponds to a cartesian vector along the positive z axis.
struct SphericalCoords
{
  //! Radial coordinate
  float radius;
  //! Zenith angle, latitude
  //! PVR follows a convention of [-pi/2,pi/2] range for latitude, with the
  //! positive direction going up, looking down the cartesian z axis.
  float latitude;
  //! Azimuthal angle, longitude
  //! PVR follows a convention of [-pi,pi] range for longitude, with the 
  //! positive direction going to the right, looking down the cartesian z axis.
  float longitude; 
  //! Default constructor
  SphericalCoords()
    : radius(0.0f), latitude(0.0f), longitude(0.0f)
  { }
};

//----------------------------------------------------------------------------//
// SphericalCamera
//----------------------------------------------------------------------------//

/*! \brief A simple spherical projection camera
  
  The SphericalCamera is mainly used to produce transmittance maps for point
  lights. It projects a spherical coordinate space which is mapped to 
  raster space using rectangular (phi, theta) coordinates. Spherical coordinate
  (0,0) maps to the center pixel.

  Raster space depth is translated to world space distance directly, so the
  camera has no near and far plane.

 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC SphericalCamera : public Camera
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(SphericalCamera);

  // Constructor, destructor, factory ------------------------------------------

  PVR_DEFINE_CREATE_FUNC(SphericalCamera);
  
  // From Camera ---------------------------------------------------------------
  
  PVR_DEFINE_STATIC_CLONE_FUNC(SphericalCamera);

  virtual Vector worldToScreen(const Vector &wsP, const PTime time) const;
  virtual Vector screenToWorld(const Vector &ssP, const PTime time) const;
  virtual Vector worldToRaster(const Vector &wsP, const PTime time) const;
  virtual Vector rasterToWorld(const Vector &rsP, const PTime time) const;
  virtual bool canTransformNegativeCamZ() const;

  // Cloning -------------------------------------------------------------------

  SphericalCamera::Ptr clone() const
  { return SphericalCamera::Ptr(rawClone()); }

protected:

  // From Camera ---------------------------------------------------------------

  virtual void recomputeTransforms();

  // Utility methods -----------------------------------------------------------

  //! Transforms cartesian coordinates to spherical coordinates
  SphericalCoords cartToSphere(const Vector &cs) const;
  //! Transforms spherical coordinates to cartesian coordinates
  Vector sphereToCart(const SphericalCoords &ss) const;

  // Protected data members ----------------------------------------------------
  
  //! Pre-computed screen to raster transform. This is never time-depedent.
  Matrix m_screenToRaster;
  //! Pre-computed raster to screen transform. This is never time-depedent.
  Matrix m_rasterToScreen;

private:

  // From Camera ---------------------------------------------------------------
  
  virtual SphericalCamera* rawClone() const
  { return new SphericalCamera(*this); }

};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

//! Calculates vertical field of view.
//! \param focalLength The camera's focal length
//! \param hAperture The horizontal film gate size. This is 0.980in = 2.4892cm
//! on a normal Super35 setup.
//! \param resolution Image resolution, used to calculate aspect ratio of the
//! frame.
double LIBPVR_EXPORT calculateVerticalFOV(const double focalLength, const double hAperture,
                            const Imath::V2i &resolution);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
