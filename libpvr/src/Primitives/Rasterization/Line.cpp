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
