//----------------------------------------------------------------------------//

/*! \file TransmittanceMapOccluder.cpp
  Contains implementations of TransmittanceMapOccluder class and related 
  functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Occluders/TransmittanceMapOccluder.h"

// System includes

// Library includes

// Project headers

#include "pvr/Constants.h"

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
// TransmittanceMapOccluder
//----------------------------------------------------------------------------//

TransmittanceMapOccluder::TransmittanceMapOccluder(Renderer::CPtr baseRenderer, 
                                                   Camera::CPtr camera,
                                                   const size_t numSamples)
  : m_camera(camera)
{ 
  // Clone Renderer to create a mutable copy
  Renderer::Ptr renderer = baseRenderer->clone();
  // Configure Renderer
  renderer->setCamera(camera);
  renderer->setPrimaryEnabled(false);
  renderer->setTransmittanceMapEnabled(true);
  renderer->setNumDeepSamples(numSamples);
  // Execute render and grab transmittace map
  renderer->execute();
  m_transmittanceMap = renderer->transmittanceMap();
  // Record the bounds of the transmittance map
  m_rasterBounds = static_cast<Imath::V2f>(m_transmittanceMap->size());
  // Check if space behind camera is valid
  m_clipBehindCamera = !camera->canTransformNegativeCamZ();
}

//----------------------------------------------------------------------------//

Color TransmittanceMapOccluder::sample(const OcclusionSampleState &state) const
{
  // Error checking
  if (!m_camera) {
    throw MissingCameraException("");
  }
  if (!m_transmittanceMap) {
    throw MissingTransmittanceMapException("");
  }

  // Transform to camera space for depth and raster space for pixel coordinate
  Vector csP = m_camera->worldToCamera(state.wsP, state.rayState.time);
  Vector rsP = m_camera->worldToRaster(state.wsP, state.rayState.time);
  
  // Bounds checks
  if (m_clipBehindCamera && csP.z < 0.0) {
    return Colors::one();
  }
  if (rsP.x < 0.0 || rsP.x > m_rasterBounds.x ||
      rsP.y < 0.0 || rsP.y > m_rasterBounds.y) {
    return Colors::one();
  }
  
  // Compute depth to sample at
  float depth = (state.wsP - m_camera->position(state.rayState.time)).length();

  // Finally interpolate
  return m_transmittanceMap->lerp(rsP.x, rsP.y, depth);
}

//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
