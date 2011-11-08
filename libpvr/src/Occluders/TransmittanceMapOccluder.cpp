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

TransmittanceMapOccluder::Ptr TransmittanceMapOccluder::create()
{ 
  return Ptr(new TransmittanceMapOccluder); 
}

//----------------------------------------------------------------------------//

std::string TransmittanceMapOccluder::typeName() const
{ 
  return "TransmittanceMapOccluder"; 
}

//----------------------------------------------------------------------------//

Color 
TransmittanceMapOccluder::transmittance(const OcclusionSampleState &state) const
{
  // Error checking
  if (!m_camera) {
    throw MissingCameraException("");
  }
  if (!m_transmittanceMap) {
    throw MissingTransmittanceMapException("");
  }

  // Transform to camera space for depth and raster space for pixel coordinate
  Vector csP = m_camera->worldToCamera(state.wsP, state.renderState.time);
  Vector rsP = m_camera->worldToRaster(state.wsP, state.renderState.time);
  
  // Bounds checks
  if (csP.z > 0.0) {
    return Colors::one();
  }
  if (rsP.x < 0.0 || rsP.x > m_rasterBounds.x ||
      rsP.y < 0.0 || rsP.y > m_rasterBounds.y) {
    return Colors::one();
  }
  
  //! \bug Transmittance map stores z distance, not camera space depth!!!

  // Finally interpolate
  return m_transmittanceMap->lerp(rsP.x, rsP.y, std::abs(csP.z));
}

//----------------------------------------------------------------------------//

void TransmittanceMapOccluder::setCamera(Camera::CPtr camera)
{ 
  m_camera = camera; 
}

//----------------------------------------------------------------------------//

void TransmittanceMapOccluder::setTransmittanceMap(TransmittanceMap::CPtr map)
{ 
  if (!map) {
    Util::Log::warning("TransmittanceMapOccluder::setTransmittanceMap "
                       "got null pointer");
  }
  m_transmittanceMap = map; 
  m_rasterBounds = static_cast<Imath::V2f>(map->size());
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
