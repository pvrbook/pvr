//----------------------------------------------------------------------------//

/*! \file OtfVoxelOccluder.cpp
  Contains implementations of OtfVoxelOccluder class and related 
  functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Occluders/OtfVoxelOccluder.h"

// System includes

// Library includes

// Project headers

#include "pvr/Constants.h"
#include "pvr/Interrupt.h"
#include "pvr/Log.h"
#include "pvr/Math.h"

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
using namespace Field3D;

using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// OtfVoxelOccluder
//----------------------------------------------------------------------------//

OtfVoxelOccluder::OtfVoxelOccluder(Renderer::CPtr renderer, 
                             const Vector &wsLightPos,
                             const size_t res)
  : m_renderer(renderer), m_wsLightPos(wsLightPos)
{
  BBox wsBounds = renderer->scene()->volume->wsBounds();
  V3i bufferRes = wsBounds.size() / Math::max(wsBounds.size()) * res;
  m_buffer.setMapping(Math::makeMatrixMapping(wsBounds));
  m_buffer.setSize(bufferRes);
  m_buffer.clear(Color(-1.0));
}

//----------------------------------------------------------------------------//

Color OtfVoxelOccluder::sample(const OcclusionSampleState &state) const
{
  Vector vsP;
  m_buffer.mapping()->worldToVoxel(state.wsP, vsP);
  
  if (!Math::isInBounds(vsP, m_buffer.dataWindow())) {
    return Colors::one();
  }

  int x0 = static_cast<int>(std::floor(vsP.x));
  int y0 = static_cast<int>(std::floor(vsP.y));
  int z0 = static_cast<int>(std::floor(vsP.z));

  for (int k = z0; k < z0 + 2; ++k) {
    for (int j = y0; j < y0 + 2; ++j) {
      for (int i = x0; i < x0 + 2; ++i) {
        if (Math::max(m_buffer.fastValue(i, j, k)) < 0.0) {
          updateVoxel(i, j, k);
        }
      }
    }
  }

  return m_linearInterp.sample(m_buffer, vsP);
}

//----------------------------------------------------------------------------//

void
OtfVoxelOccluder::updateVoxel(const int i, const int j, const int k) const
{
  // Transform point from voxel to world space
  Vector wsP;
  m_buffer.mapping()->voxelToWorld(discToCont(V3i(i, j, k)), wsP);
  // Set up the ray state
  RayState state;
  state.rayType = RayState::TransmittanceOnly;
  state.rayDepth = 1;
  state.wsRay.pos = wsP;
  state.wsRay.dir = (m_wsLightPos - wsP).normalized();
  state.tMax = (m_wsLightPos - wsP).length();
  // Trace ray and record transmittance
  IntegrationResult result = m_renderer->trace(state);
  m_buffer.fastLValue(i, j, k) = result.transmittance;
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
