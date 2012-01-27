//----------------------------------------------------------------------------//

/*! \file Line.cpp
  Contains implementations of Rast::Line class 
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/Rasterization/Line.h"

// System includes

// Library includes

// Project includes

#include "pvr/RenderGlobals.h"
#include "pvr/Interrupt.h"
#include "pvr/Log.h"
#include "pvr/ModelingUtils.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace pvr::Geo;
using namespace pvr::Math;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// Line
//----------------------------------------------------------------------------//

void Line::getSample(const RasterizationState &state,
                     RasterizationSample &sample) const
{
  if (m_basePointAttrs.size() < 2) {
    return;
  }

  SegmentInfo info;

  if (findClosestSegment(state, info)) {

    // Compute falloff depending on whether we want antialiasing
    float falloff;
    if (m_basePolyAttrs.antialiased.value()) {
      float halfWidth = state.wsVoxelSize.length() * 0.5;
      falloff = 1.0 - Math::smoothStep(info.distance, 
                                       info.radius - halfWidth, 
                                       info.radius + halfWidth);
    } else {
      falloff = info.distance < info.radius ? 1.0 : 0.0;
    }

    // Set values in RasterizationSample
    sample.value      = falloff * LINE_INTERP(density, info);
    sample.wsVelocity = LINE_INTERP(wsVelocity, info);

  }
}

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
