//----------------------------------------------------------------------------//

/*! \file Camera.cpp
  Contains implementations of Camera class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Camera.h"

// System includes

// Project includes

#include "pvr/RenderGlobals.h"
#include "pvr/Log.h" // DEBUG

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

Camera::Camera()
  : m_resolution(640, 480), m_numSamples(2)
{
  Camera::recomputeTransforms();
}

//----------------------------------------------------------------------------//

Camera::~Camera()
{ 
  
}

//----------------------------------------------------------------------------//

void Camera::setPosition(const Util::VectorCurve &curve)
{
  m_position = curve;
  recomputeTransforms();
}

//----------------------------------------------------------------------------//

Vector Camera::position(const PTime time) const
{
  return m_position.interpolate(time);
}

//----------------------------------------------------------------------------//

void Camera::setOrientation(const Util::QuatCurve &curve)
{
  m_orientation = curve;
  recomputeTransforms();
}

//----------------------------------------------------------------------------//

Quat Camera::orientation(const PTime time) const
{
  return m_orientation.interpolate(time);
}

//----------------------------------------------------------------------------//

void Camera::setResolution(const Imath::V2i &resolution)
{
  m_resolution = resolution;
  recomputeTransforms();    
}

//----------------------------------------------------------------------------//

const Imath::V2i& Camera::resolution() const
{
  return m_resolution;
}

//----------------------------------------------------------------------------//

void Camera::setNumTimeSamples(const uint numSamples)
{
  if (numSamples == 0) {
    throw TimeSamplesException("Zero");
  }

  m_numSamples = numSamples;
 
  recomputeTransforms();
}

//----------------------------------------------------------------------------//

uint Camera::numTimeSamples() const
{
  return m_numSamples;
}

//----------------------------------------------------------------------------//

Vector Camera::worldToCamera(const Vector &wsP, const PTime time) const
{
  return transformPoint(wsP, m_worldToCamera, time);
}

//----------------------------------------------------------------------------//

Vector Camera::cameraToWorld(const Vector &csP, const PTime time) const
{
  return transformPoint(csP, m_cameraToWorld, time);
}

//----------------------------------------------------------------------------//

const Camera::MatrixVec& Camera::worldToCameraMatrices() const
{
  return m_worldToCamera;
}

//----------------------------------------------------------------------------//

const Camera::MatrixVec& Camera::cameraToWorldMatrices() const
{
  return m_cameraToWorld;
}

//----------------------------------------------------------------------------//

void Camera::recomputeTransforms()
{
  m_cameraToWorld.resize(m_numSamples);
  m_worldToCamera.resize(m_numSamples);

  for (uint i = 0; i < m_numSamples; ++i) {
    // Compute position in open/close parametric shutter time
    float fraction = static_cast<float>(i) / 
      static_cast<float>(m_numSamples - 1);
    // Compute transformation matrix at given time
    m_cameraToWorld[i] = computeCameraToWorld(PTime(fraction));
    m_worldToCamera[i] = m_cameraToWorld[i].inverse();
  }
}

//----------------------------------------------------------------------------//

Vector Camera::transformPoint(const Vector &p, 
                              const std::vector<Matrix> &matrices,
                              const PTime time) const
{
  // Calculate which interval to interpolate in
  double stepSize = 1.0 / static_cast<float>(m_numSamples - 1);
  double slicePos = time / stepSize;
  uint first = static_cast<uint>(std::floor(slicePos));
  uint second = static_cast<uint>(std::ceil(slicePos));
  second = std::min(second, m_numSamples - 1);
  double lerpFactor = slicePos - static_cast<double>(first);
  // Transform point twice
  Vector t0 = p * matrices[Imath::clamp(first, 0u, m_numSamples - 1)];
  Vector t1 = p * matrices[Imath::clamp(second, 0u, m_numSamples - 1)];
  // Interpolate transformed positions
  return lerp(t0, t1, lerpFactor);
}

//----------------------------------------------------------------------------//

Matrix Camera::computeCameraToWorld(const PTime time) const
{
  Matrix translation, rotation;
  translation.setTranslation(m_position.interpolate(time));
  rotation = m_orientation.interpolate(time).toMatrix44();
  return rotation * translation;
}

//----------------------------------------------------------------------------//
// PerspectiveCamera
//----------------------------------------------------------------------------//

PerspectiveCamera::PerspectiveCamera()
  : Camera(),
    m_near(1.0), 
    m_far(100.0)
{
  Util::FloatCurve fov;
  fov.addSample(0.0, 45.0);
  setVerticalFOV(fov);
}

//----------------------------------------------------------------------------//

PerspectiveCamera::Ptr PerspectiveCamera::create()
{
  return Ptr(new PerspectiveCamera);
}

//----------------------------------------------------------------------------//

void PerspectiveCamera::setClipPlanes(const double near, const double far)
{
  m_near = near;
  m_far = far;
  recomputeTransforms();
}

//----------------------------------------------------------------------------//

void PerspectiveCamera::setVerticalFOV(const Util::FloatCurve &curve)
{
  m_verticalFOV = curve;
  recomputeTransforms();
}

//----------------------------------------------------------------------------//

const Camera::MatrixVec& PerspectiveCamera::worldToScreenMatrices() const
{
  return m_worldToScreen;
}


//----------------------------------------------------------------------------//

const Camera::MatrixVec& PerspectiveCamera::screenToWorldMatrices() const
{
  return m_screenToWorld;
}

//----------------------------------------------------------------------------//

Vector PerspectiveCamera::worldToScreen(const Vector &wsP, const PTime time) const
{
  return transformPoint(wsP, m_worldToScreen, time);
}

//----------------------------------------------------------------------------//

Vector PerspectiveCamera::screenToWorld(const Vector &ssP, const PTime time) const
{
  return transformPoint(ssP, m_screenToWorld, time);
}

//----------------------------------------------------------------------------//

Vector PerspectiveCamera::worldToRaster(const Vector &wsP, const PTime time) const
{
  return transformPoint(wsP, m_worldToRaster, time);
}

//----------------------------------------------------------------------------//

Vector PerspectiveCamera::rasterToWorld(const Vector &rsP, const PTime time) const
{
  return transformPoint(rsP, m_rasterToWorld, time);
}

//----------------------------------------------------------------------------//

void PerspectiveCamera::recomputeTransforms()
{
  Camera::recomputeTransforms();

  m_worldToScreen.resize(m_numSamples);
  m_screenToWorld.resize(m_numSamples);
  m_worldToRaster.resize(m_numSamples);
  m_rasterToWorld.resize(m_numSamples);

  Matrix cameraToScreen, screenToRaster;

  for (uint i = 0; i < m_numSamples; ++i) {
    // Compute position in open/close parametric shutter time
    float fraction = static_cast<float>(i) / 
      static_cast<float>(m_numSamples - 1);
    // Compute matrices
    getTransforms(PTime(fraction), cameraToScreen, screenToRaster);
    m_worldToScreen[i] = m_worldToCamera[i] * cameraToScreen;
    m_screenToWorld[i] = m_worldToScreen[i].inverse();
    m_worldToRaster[i] = m_worldToScreen[i] * screenToRaster;
    m_rasterToWorld[i] = m_worldToRaster[i].inverse();
  }
}

//----------------------------------------------------------------------------//

void PerspectiveCamera::getTransforms(const PTime time, 
                                      Matrix &cameraToScreen,
                                      Matrix &screenToRaster) const
{
  double fovDegrees = m_verticalFOV.interpolate(0.0);
  double fovRadians = fovDegrees * M_PI / 180.0;
  double invTan = 1.0f / std::tan(fovRadians / 2.0f);
  double imageAspectRatio = static_cast<double>(m_resolution.x) / 
    static_cast<double>(m_resolution.y);
  // Standard projection matrix
  Matrix perspective(1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, (m_far) / (m_far - m_near),            1,
                     0, 0, (- m_far * m_near) / (m_far - m_near), 0);
  // NDC to screen space
  Matrix ndcTranslate, ndcScale;
  ndcTranslate.setTranslation(Vector(1.0, 1.0, 0.0));
  ndcScale.setScale(Vector(0.5, 0.5, 1.0));
  Matrix screenToNdc = ndcTranslate * ndcScale;
  // Raster to NDC space
  Matrix rasterToNdc, ndcToRaster;
  rasterToNdc.setScale(Vector(1.0 / m_resolution.x, 1.0 / m_resolution.y, 1.0));
  ndcToRaster = rasterToNdc.inverse();
  // Field of view scaling
  Matrix fov;
  fov.setScale(Vector(invTan / imageAspectRatio, invTan, 1.0));
  // Flip z axis
  Matrix flipZ;
  flipZ.setScale(Vector(1.0, 1.0, -1.0));
  // Calculate matrices
  cameraToScreen = flipZ * perspective * fov;
  screenToRaster = screenToNdc * ndcToRaster; 
}

//----------------------------------------------------------------------------//
// SphericalCamera
//----------------------------------------------------------------------------//

Vector SphericalCamera::worldToScreen(const Vector &wsP, const PTime time) const
{
  Vector csP = worldToCamera(csP, time);
  SphericalCoords sc = cartToSphere(csP * Vector(1.0, 1.0, -1.0));
  return Vector(sc.longitude / M_PI, sc.latitude / (M_PI * 0.5), sc.radius);
}

//----------------------------------------------------------------------------//

Vector SphericalCamera::screenToWorld(const Vector &ssP, const PTime time) const
{
  SphericalCoords sc;
  sc.longitude = ssP.x * M_PI;
  sc.latitude  = ssP.y * (M_PI * 0.5);
  sc.radius    = ssP.z;
  Vector csP = sphereToCart(sc) * Vector(1.0, 1.0, -1.0);
  return cameraToWorld(csP, time);
}

//----------------------------------------------------------------------------//

Vector SphericalCamera::worldToRaster(const Vector &wsP, const PTime time) const
{
  Vector ssP = worldToScreen(wsP, time);
  Vector rsP;
  m_screenToRaster.multVecMatrix(ssP, rsP);
  return rsP;
}
  
//----------------------------------------------------------------------------//

Vector SphericalCamera::rasterToWorld(const Vector &rsP, const PTime time) const
{
  Vector ssP;
  m_rasterToScreen.multVecMatrix(rsP, ssP);
  return screenToWorld(ssP, time);
}

//----------------------------------------------------------------------------//

void SphericalCamera::recomputeTransforms()
{
  Camera::recomputeTransforms();

  // NDC to screen space
  Matrix ndcTranslate, ndcScale;
  ndcTranslate.setTranslation(Vector(1.0, 1.0, 0.0));
  ndcScale.setScale(Vector(0.5, 0.5, 1.0));
  Matrix screenToNdc = ndcTranslate * ndcScale;
  // Raster to NDC space
  Matrix rasterToNdc, ndcToRaster;
  rasterToNdc.setScale(Vector(1.0 / m_resolution.x, 1.0 / m_resolution.y, 1.0));
  ndcToRaster = rasterToNdc.inverse();
  // Store screen to raster and inverse
  m_screenToRaster = screenToNdc * ndcToRaster;
  m_rasterToScreen = m_screenToRaster.inverse();
}

//----------------------------------------------------------------------------//

SphericalCoords SphericalCamera::cartToSphere(const Vector &cc) const
{
  SphericalCoords sc;

  sc.radius = std::sqrt(cc.x * cc.x + cc.y * cc.y + cc.z * cc.z);

  if (sc.radius == 0.0) {
    return SphericalCoords();
  }

  sc.longitude = atan2(cc.x, cc.z);
  sc.latitude  = M_PI * 0.5 - acos(cc.y / sc.radius);

  return sc;
}

//----------------------------------------------------------------------------//

Vector SphericalCamera::sphereToCart(const SphericalCoords &sc) const
{
  const float rho   = sc.radius;
  const float phi   = sc.longitude;
  const float theta = 0.5 * M_PI - sc.latitude;

  return Vector(rho * std::sin(phi) * std::sin(theta), 
                rho * std::cos(theta), 
                rho * std::cos(phi) * std::sin(theta));
}

//----------------------------------------------------------------------------//
// Function implementations
//----------------------------------------------------------------------------//

double calculateVerticalFOV(const double focalLength, 
                            const double hAperture,
                            const Imath::V2i   &resolution)
{
  double vAperture = (static_cast<double>(resolution.y) * hAperture) / 
    static_cast<double>(resolution.x);
  return 2.0 * std::atan((vAperture / 2.0) / focalLength);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
