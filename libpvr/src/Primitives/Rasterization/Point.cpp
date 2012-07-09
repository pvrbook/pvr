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

/*! \file Point.cpp
  Contains implementations of Rast::Point class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/Rasterization/Point.h"

// System includes

// Library includes

// Project includes

#include "pvr/Geometry.h"
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

  using namespace pvr;

  //--------------------------------------------------------------------------//

  Imath::V3f evaluateSphere(const Vector &wsP, const Vector &wsCenter, 
                            const float wsRadius, const Imath::V3f &density,
                            const float falloffWidth)
  {
    float dist = (wsCenter - wsP).length();
    return Math::fit(dist, falloffWidth * wsRadius, wsRadius, 
                     1.0f, 0.0f) * density;
  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace pvr::Geo;
using namespace pvr::Util;
using namespace pvr::Sys;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// Point
//----------------------------------------------------------------------------//

Point::Ptr Point::create()
{
  return Ptr(new Point);
}

//----------------------------------------------------------------------------//

void Point::execute(Geo::Geometry::CPtr geometry, 
                    VoxelBuffer::Ptr buffer) const
{
  using namespace Field3D;
  using namespace std;
  
  const AttrTable &points = geometry->particles()->pointAttrs();

  if (!points.vectorAttrRef("P").isValid()) {
    Log::warning("Point primitive - no P attribute in input");
    return;
  }

  Timer             timer;
  size_t            count = 0;
  ProgressReporter  progress(2.5f, "  ");
  FieldMapping::Ptr mapping(buffer->mapping());
  AttrVisitor       visitor(points, m_params);

  Log::print("Point primitive processing " + str(points.size()) + 
             " input points");

  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i, ++count) {

    // Check if user terminated
    Sys::Interrupt::throwOnAbort();
    // Print progress
    progress.update(static_cast<float>(count) / points.size());
    // Update attributes
    m_attrs.update(i);
    // Point attributes
    const V3f    &density     = m_attrs.density;
    const float   wsRadius    = m_attrs.radius;
    const Vector  wsCenter    = m_attrs.wsCenter.as<Vector>();
    const Vector  wsVelocity  = m_attrs.wsVelocity.as<Vector>();
    const bool    antialiased = m_attrs.antialiased;
    // Skip if density at or below zero
    if (Math::max(density) <= 0.0) {
      continue;
    }
    // Transform to voxel space
    Vector vsP;
    mapping->worldToVoxel(wsCenter, vsP);
    // Determine relative size of the point, compared to the shortest
    // edge of a voxel.
    Vector wsVoxelSize = pvr::Model::wsVoxelSize(mapping, vsP);
    double relativeSize = wsRadius / Math::min(wsVoxelSize);
    
    // Check relative size of point
    if (relativeSize > 1.0) {

      // If we're larger than a voxel, rasterize the point as a sphere
    
      // Calculate filter width
      float filterWidth = wsVoxelSize.length();
      // Calculate rasterization bounds
      //! \bug Rasterization bounds do not include filter width
      BBox vsBounds = 
        vsSphereBounds(mapping, wsCenter, wsRadius + filterWidth * 0.5);
      // Call rasterize(), which will query getSample() for values
      rasterize(vsBounds, buffer);

    } else {

      // If we are smaller than a voxel we splat the point, 
      // compensating for pscale by varying density.

      // Account for voxel size
      V3f voxelVolume = V3f(wsVoxelSize.x * wsVoxelSize.y * wsVoxelSize.z);
      V3f voxelDensity = 
        density / voxelVolume * sphereVolume(wsRadius);
      // Write points
      if (wsVelocity.length() == 0) {
        if (antialiased) {
          writeAntialiasedPoint(vsP, voxelDensity, buffer);
        } else {
          writePoint(vsP, voxelDensity, buffer);
        }
      } else {
        Vector wsEnd = wsCenter + wsVelocity * RenderGlobals::dt();
        Vector vsEnd;
        mapping->worldToVoxel(wsEnd, vsEnd);
        if (antialiased) {
          writeLine<true>(vsP, vsEnd, voxelDensity, buffer);
        } else {
          writeLine<false>(vsP, vsEnd, voxelDensity, buffer);
        }
      }

    }

  }

  Log::print("  Time elapsed: " + str(timer.elapsed()));
}

//----------------------------------------------------------------------------//

void Point::getSample(const RasterizationState &state,
                      RasterizationSample &sample) const
{
  float filterWidth = state.wsVoxelSize.length();
  float halfWidth = 0.5 * filterWidth;
  float factor = 1.0 / (1.0 + pow(halfWidth / m_attrs.radius, 3.0f));
  sample.value = evaluateSphere(state.wsP, m_attrs.wsCenter.as<Vector>(), 
                                m_attrs.radius + halfWidth, 
                                m_attrs.density.value() * factor, 
                                (m_attrs.radius - halfWidth) / m_attrs.radius);
  sample.wsVelocity = m_attrs.wsVelocity.as<Vector>();
}

//----------------------------------------------------------------------------//

BBox Point::pointWsBounds(const Geo::AttrVisitor::const_iterator &i) const
{
  BBox wsBBox;
  // Update point attrs
  m_attrs.update(i);
  // Pad to account for radius and motion
  Vector wsStart = m_attrs.wsCenter.value();
  Vector wsEnd = m_attrs.wsCenter.value() + 
    m_attrs.wsVelocity.value() * RenderGlobals::dt();
  wsBBox.extendBy(wsStart + m_attrs.radius.as<Vector>());
  wsBBox.extendBy(wsStart - m_attrs.radius.as<Vector>());
  wsBBox.extendBy(wsEnd + m_attrs.radius.as<Vector>());
  wsBBox.extendBy(wsEnd - m_attrs.radius.as<Vector>());
  return wsBBox;
}
  
//----------------------------------------------------------------------------//
// Point::AttrState
//----------------------------------------------------------------------------//

void Point::AttrState::update(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(wsCenter);
  i.update(wsVelocity);
  i.update(radius);
  i.update(density);
  i.update(antialiased);
}

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
