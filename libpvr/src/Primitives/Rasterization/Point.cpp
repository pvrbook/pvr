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
    // Skip if density at or below zero
    if (Math::max(m_attrs.density.value()) <= 0.0) {
      continue;
    }
    // Transform to voxel space
    Vector vsP;
    mapping->worldToVoxel(m_attrs.wsCenter.as<Vector>(), vsP);
    // Determine relative size of the point, compared to the shortest
    // edge of a voxel.
    Vector wsVoxelSize = pvr::Model::wsVoxelSize(mapping, vsP);
    double relativeSize = m_attrs.radius / Math::min(wsVoxelSize);
    
    // Check relative size of point
    if (relativeSize > 1.0) {

      // If we're larger than a voxel, rasterize the point as a sphere
    
      // Calculate filter width
      float filterWidth = wsVoxelSize.length();
      // Calculate rasterization bounds
      //! \bug Rasterization bounds do not include filter width
      BBox vsBounds = 
        vsSphereBounds(mapping, m_attrs.wsCenter.as<Vector>(), 
                       m_attrs.radius + filterWidth * 0.5);
      // Call rasterize(), which will query getSample() for values
      rasterize(vsBounds, buffer);

    } else {

      // If we are smaller than a voxel we splat the point, 
      // compensating for pscale by varying density.

      // Account for voxel size
      V3f voxelVolume = V3f(wsVoxelSize.x * wsVoxelSize.y * wsVoxelSize.z);
      V3f voxelDensity = 
        m_attrs.density.value() / voxelVolume * sphereVolume(m_attrs.radius);
      // Write points
      if (m_attrs.wsVelocity.value().length() == 0) {
        if (m_attrs.antialiased) {
          writeAntialiasedPoint(vsP, voxelDensity, buffer);
        } else {
          writePoint(vsP, voxelDensity, buffer);
        }
      } else {
        Vector wsEnd = m_attrs.wsCenter.as<Vector>() + 
          m_attrs.wsVelocity.as<Vector>() * RenderGlobals::dt();
        Vector vsEnd;
        mapping->worldToVoxel(wsEnd, vsEnd);
        if (m_attrs.antialiased) {
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
