//----------------------------------------------------------------------------//

/*! \file VoxelOccluder.cpp
  Contains implementations of VoxelOccluder class and related 
  functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Occluders/VoxelOccluder.h"

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
// VoxelOccluder
//----------------------------------------------------------------------------//

VoxelOccluder::VoxelOccluder(Renderer::CPtr renderer, 
                             const Vector &wsLightPos,
                             const size_t res)
{
  Log::print("Building VoxelOccluder");

  BBox wsBounds       = renderer->scene()->volume->wsBounds();
  Matrix localToWorld = Math::coordinateSystem(wsBounds);
  MatrixFieldMapping::Ptr mapping(new MatrixFieldMapping);
  mapping->setLocalToWorld(localToWorld);
  m_buffer.setMapping(mapping);

  V3i bufferRes = wsBounds.size() / Math::max(wsBounds.size()) * res;
  m_buffer.setSize(bufferRes);

  Log::print("  Resolution: " + str(bufferRes));

  RayState state;
  state.rayType  = RayState::TransmittanceOnly;
  state.rayDepth = 1;

  Timer timer;
  ProgressReporter progress(2.5f, "  ");

  size_t count = 0, numVoxels = bufferRes.x * bufferRes.y * bufferRes.z;
  for (DenseBuffer::iterator i = m_buffer.begin(), end = m_buffer.end();
       i != end; ++i, ++count) {
    // Check if user terminated
    Sys::Interrupt::throwOnAbort();
    // Print progress
    progress.update(static_cast<float>(count) / numVoxels);
    // Do work
    Vector wsP;
    m_buffer.mapping()->voxelToWorld(discToCont(V3i(i.x, i.y, i.z)), wsP);
    state.wsRay.pos          = wsP;
    state.wsRay.dir          = (wsLightPos - wsP).normalized();
    state.tMax               = (wsLightPos - wsP).length();
    IntegrationResult result = renderer->trace(state);
    *i = result.transmittance;
  }

  Log::print("  Time elapsed: " + str(timer.elapsed()));
}

//----------------------------------------------------------------------------//

Color VoxelOccluder::sample(const OcclusionSampleState &state) const
{
  Vector vsP;
  m_buffer.mapping()->worldToVoxel(state.wsP, vsP);
  if (!Math::isInBounds(vsP, m_buffer.dataWindow())) {
    return Colors::one();
  }
  Color val = m_linearInterp.sample(m_buffer, vsP);
  return val;
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
