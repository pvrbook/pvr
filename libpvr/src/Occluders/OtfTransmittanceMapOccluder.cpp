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

#include <OpenEXR/ImathRandom.h>

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
                                                         Camera::CPtr camera,
                                                         const size_t numSamples)
  : m_renderer(renderer), m_camera(camera)
{ 
  // Record resolution of camera
  m_resolution = m_camera->resolution();
  m_floatRasterBounds = static_cast<Imath::V2f>(m_resolution);
  m_intRasterBounds = m_resolution - Imath::V2i(1);
  // Update transmittance map size and sample count
  m_transmittanceMap.setSize(m_resolution.x, m_resolution.y);
  m_transmittanceMap.setNumSamples(numSamples);
  // Reset the list of computed pixels
  m_computed.resize(m_resolution.x * m_resolution.y, 0);
  // Check if space behind camera is valid
  m_clipBehindCamera = !camera->canTransformNegativeCamZ();
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
  if (m_clipBehindCamera && csP.z < 0.0) {
    return Colors::one();
  }
  if (rsP.x < 0.0 || rsP.x >= m_floatRasterBounds.x ||
      rsP.y < 0.0 || rsP.y >= m_floatRasterBounds.y) {
    return Colors::one();
  }

  // Compute depth to sample at
  Vector wsCamPosition = m_camera->position(state.rayState.time);
  float depth = (state.wsP - wsCamPosition).length();

  // Ensure all samples are available
  updateCoordinate(rsP);

  // Finally interpolate
  return m_transmittanceMap.lerp(rsP.x, rsP.y, depth);
}

//----------------------------------------------------------------------------//

void
OtfTransmittanceMapOccluder::updateCoordinate(const Vector &rsP) const
{
  size_t x = static_cast<size_t>(std::floor(rsP.x));
  size_t y = static_cast<size_t>(std::floor(rsP.y));
  for (uint j = y; j < y + 2; j++) {
    for (uint i = x; i < x + 2; i++) {
      uint iC = Imath::clamp(i, 0u, static_cast<uint>(m_intRasterBounds.x));
      uint jC = Imath::clamp(j, 0u, static_cast<uint>(m_intRasterBounds.y));
      if (!m_computed[offset(iC, jC)]) {
        updatePixel(iC, jC);
      }
    }
  }
}

//----------------------------------------------------------------------------//

void
OtfTransmittanceMapOccluder::updatePixel(const size_t x, const size_t y) const
{
  // Storage for transmittance functions
  size_t numSamples = m_renderer->numPixelSamples();
  std::vector<Util::ColorCurve::CPtr> tf;
  tf.reserve(numSamples * numSamples);

  // Ensure a unique sampling pattern for each pixel
  Imath::Rand48 rng(x + y * m_resolution.x);

  // Fire N^2 rays, to match the number of rays used in main render
  for (size_t i = 0; i < numSamples * numSamples; ++i) {
    // Set up the next ray with a random time sample
    RayState state;
    PTime ptime           ((i + rng.nextf()) / (numSamples * numSamples));
    state.wsRay         = setupRay(m_camera, Field3D::discToCont(x), 
                                   Field3D::discToCont(y), ptime);
    state.time          = ptime;
    state.rayType       = RayState::TransmittanceOnly;
    state.rayDepth      = 1;
    state.doOutputDeepT = true;
    state.doOutputDeepL = false;
    // Trace ray to find transmittance
    IntegrationResult result = m_renderer->trace(state);
    // Store transmittance function
    if (result.transmittanceFunction) {
      tf.push_back(result.transmittanceFunction);
    }
  }

  // Update transmittance map
  if (tf.size() > 0) {
    m_transmittanceMap.setPixel(x, y, Util::ColorCurve::average(tf));
  } else {
    m_transmittanceMap.setPixel(x, y, Colors::one());
  }

  // Mark pixel as computed
  m_computed[offset(x, y)] = 1;
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
