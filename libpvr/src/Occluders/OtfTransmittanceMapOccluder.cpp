//----------------------------------------------------------------------------//

/*! \file OtfTransmittanceMapOccluder.cpp
  Contains implementations of OtfTransmittanceMapOccluder class and related 
  functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Occluders/OtfTransmittanceMapOccluder.h"

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
// OtfTransmittanceMapOccluder
//----------------------------------------------------------------------------//

OtfTransmittanceMapOccluder::OtfTransmittanceMapOccluder(Renderer::CPtr renderer, 
                                                         Camera::CPtr camera)
  : m_camera(camera)
{ 
  m_rasterBounds = static_cast<Imath::V2f>(m_camera->resolution());
  m_renderer = renderer;
  m_transmittanceMap.setSize(m_rasterBounds.x, m_rasterBounds.y);
  m_computed.resize(m_rasterBounds.x * m_rasterBounds.y);
  std::fill(m_computed.begin(), m_computed.end(), 0);
}

//----------------------------------------------------------------------------//

std::string OtfTransmittanceMapOccluder::typeName() const
{ 
  return "OtfTransmittanceMapOccluder"; 
}

//----------------------------------------------------------------------------//

Color OtfTransmittanceMapOccluder::sample(const OcclusionSampleState &state) const
{
  // Error checking
  if (!m_camera) {
    throw MissingCameraException("");
  }

  // Transform to camera space for depth and raster space for pixel coordinate
  Vector csP = m_camera->worldToCamera(state.wsP, state.rayState.time);
  Vector rsP = m_camera->worldToRaster(state.wsP, state.rayState.time);
  
  // Bounds checks
  if (csP.z > 0.0) {
    return Colors::one();
  }
  if (rsP.x < 0.0 || rsP.x > m_rasterBounds.x ||
      rsP.y < 0.0 || rsP.y > m_rasterBounds.y) {
    return Colors::one();
  }
  
  // Compute depth to sample at
  float depth = (state.wsP - m_camera->position(state.rayState.time)).length();

  // Ensure all samples are available
  size_t x = static_cast<size_t>(std::floor(rsP.x));
  size_t y = static_cast<size_t>(std::floor(rsP.y));
  for (size_t j = y; j < y + 2; j++) {
    for (size_t i = x; i < x + 2; i++) {
      if (!m_computed[offset(i, j)]) {
        updatePixel(i, j);
      }
    }
  }

  // Finally interpolate
  return m_transmittanceMap.lerp(rsP.x, rsP.y, depth);
}

//----------------------------------------------------------------------------//

void 
OtfTransmittanceMapOccluder::updatePixel(const size_t x, const size_t y) const
{
  RayState state;
  state.wsRay         = setupRay(m_camera, Field3D::discToCont(x), 
                                 Field3D::discToCont(y), PTime(0.0));
  state.rayType       = RayState::TransmittanceOnly;
  state.rayDepth      = 1;
  state.doOutputDeepT = true;
  state.doOutputDeepL = false;

  IntegrationResult result = m_renderer->trace(state);

  if (result.transmittanceFunction) {
    m_transmittanceMap.setPixel(x, y, result.transmittanceFunction);
  } else {
    m_transmittanceMap.setPixel(x, y, Colors::one());
  }

  m_computed[offset(x, y)] = 1;
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
