//----------------------------------------------------------------------------//

/*! \file RasterizationPrimitive.cpp
  Contains implementations of RasterizationPrimitive class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/RasterizationPrim.h"

// System includes

// Library includes

#include <Field3D/Field.h>

// Project includes

#include "pvr/AttrUtil.h"
#include "pvr/Interrupt.h"
#include "pvr/Log.h"
#include "pvr/Math.h"
#include "pvr/ModelingUtils.h"
#include "pvr/RenderGlobals.h"

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
using namespace Imath;
using namespace Field3D;

using namespace pvr::Geo;
using namespace pvr::Sys;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// RasterizationPrim
//----------------------------------------------------------------------------//

void RasterizationPrim::rasterize(const BBox &vsBounds,
                                  VoxelBuffer::Ptr buffer) const
{
  FieldMapping::Ptr mapping(buffer->mapping());

  DiscreteBBox dvsBounds = Math::discreteBounds(vsBounds);
  dvsBounds.min -= Imath::V3i(1);
  dvsBounds.max += Imath::V3i(1);
  DiscreteBBox bufferBounds = buffer->dataWindow();
  dvsBounds = Math::clipBounds(dvsBounds, bufferBounds);
  
  V3i size = dvsBounds.size();
  size_t numVoxels = size.x * size.y * size.z;

  Util::Timer interruptTimer;

  ProgressReporter progress(2.5f, "  Rasterization: ");
  size_t count = 0;

  // Iterate over voxels
  for (VoxelBuffer::iterator i = buffer->begin(dvsBounds), 
         end = buffer->end(dvsBounds); i != end; ++i, ++count) {
    RasterizationState rState;
    RasterizationSample rSample;
    // Check if user terminated
    if (interruptTimer.elapsed() > 1.0) {
      Sys::Interrupt::throwOnAbort();
      interruptTimer.reset();
    }
    // Print progress
    if (count % 100 == 0) {
      progress.update(static_cast<float>(count) / numVoxels);
    }
    // Get sampling derivatives/voxel size
    rState.wsVoxelSize = mapping->wsVoxelSize(i.x, i.y, i.z);
    // Transform voxel position to world space
    Vector vsP = discToCont(V3i(i.x, i.y, i.z));
    mapping->voxelToWorld(vsP, rState.wsP);
    // Sample the primitive
    this->getSample(rState, rSample);
    if (Math::max(rSample.value) > 0.0f) {
      if (rSample.wsVelocity.length2() == 0.0) {
        *i += rSample.value;
      } else {
        Vector vsEnd;
        Vector wsMotion = rSample.wsVelocity * RenderGlobals::dt();
        mapping->worldToVoxel(rState.wsP + wsMotion, vsEnd);
        writeLine<true>(vsP, vsEnd, rSample.value, buffer);
      }
    }
  }
}

//----------------------------------------------------------------------------//
// PointBase
//----------------------------------------------------------------------------//

BBox PointBase::wsBounds(Geo::Geometry::CPtr geometry) const
{
  assert(geometry != NULL);
  assert(geometry->particles() != NULL);

  if (!geometry->particles()) {
    Log::warning("Rasterization primitive has no particles. "
                 "Skipping bounds generation.");
    return BBox();
  }

  BBox wsBBox;
  AttrVisitor visitor(geometry->particles()->pointAttrs(), m_params);

  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i) {
    BBox pointBounds = pointWsBounds(i);
    wsBBox.extendBy(pointBounds);
  }

  return wsBBox;
}

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
