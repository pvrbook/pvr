//----------------------------------------------------------------------------//

/*! \file LineBase.cpp
  Contains implementations of LineBase class.
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
using namespace pvr::Geo;
using namespace pvr::Math;
using namespace pvr::Sys;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// LineBase
//----------------------------------------------------------------------------//

BBox LineBase::wsBounds(Geo::Geometry::CPtr geometry) const
{
  typedef AttrVisitor::const_iterator AttrIter;

  assert(geometry != NULL);
  assert(geometry->polygons() != NULL);

  if (!geometry->polygons()) {
    Log::warning("Line primitive(s) have no polygons. "
                 "Skipping bounds generation.");
    return BBox();
  }

  // Iteration variables
  BBox           wsBBox;
  Polygons::CPtr polys = geometry->polygons();
  AttrVisitor    polyVisitor(polys->polyAttrs(), m_params);
  AttrVisitor    pointVisitor(polys->pointAttrs(), m_params);

  for (AttrIter iPoly = polyVisitor.begin(), endPoly = polyVisitor.end(); 
       iPoly != endPoly; ++iPoly) {
    // Update poly attributes
    updatePolyAttrs(iPoly);
    // Update point attribute
    size_t first = polys->pointForVertex(iPoly.index(), 0);
    size_t numPoints = polys->numVertices(iPoly.index());
    updatePointAttrs(pointVisitor.begin(first), numPoints);
    // Compute world-space bounds
    size_t index = 0;
    for (std::vector<PointAttrState>::const_iterator i = m_basePointAttrs.begin(),
           end = m_basePointAttrs.end(); i != end; ++i, ++index) {
      Vector radius      = Vector(i->radius.value());
      float displacement = displacementBounds(index);
      Vector wsV         = i->wsVelocity.value();
      Vector wsP         = i->wsCenter.value();
      Vector wsEnd       = wsP + wsV * RenderGlobals::dt();
      wsBBox = extendBounds(wsBBox, wsP, radius * (1.0 + displacement));
      wsBBox = extendBounds(wsBBox, wsEnd, radius * (1.0 + displacement));
    }
  }

  return wsBBox;
}
  
//----------------------------------------------------------------------------//

void LineBase::execute(Geo::Geometry::CPtr geometry, 
                                         VoxelBuffer::Ptr buffer) const
{
  using namespace Field3D;
  using namespace std;
  
  typedef AttrVisitor::const_iterator AttrIter;

  Polygons::CPtr   polys  = geometry->polygons();
  const AttrTable &points = polys->pointAttrs();
  
  // Check for P attribute - all other attributes can be defaults
  if (!points.vectorAttrRef("P").isValid()) {
    Log::warning("Line primitive - no P attribute in input");
    return;
  }

  Log::print("Line primitive processing " + 
             str(polys->polyAttrs().size()) + " input polys, " + 
             str(points.size()) + " points");

  // Iteration variables
  size_t           count = 0;
  Timer            timer;
  ProgressReporter progress(2.5f, "  ");
  AttrVisitor      polyVisitor(polys->polyAttrs(), m_params);
  AttrVisitor      pointVisitor(polys->pointAttrs(), m_params);
  
  for (AttrIter iPoly = polyVisitor.begin(), endPoly = polyVisitor.end(); 
       iPoly != endPoly; ++iPoly, ++count) {
    // Check if user terminated
    Sys::Interrupt::throwOnAbort();
    // Print progress
    progress.update(static_cast<float>(count) / polys->polyAttrs().size());
    // Update attributes
    updatePolyAttrs(iPoly);
    size_t first = polys->pointForVertex(iPoly.index(), 0);
    size_t numPoints = polys->numVertices(iPoly.index());
    updatePointAttrs(pointVisitor.begin(first), numPoints);
    // Compute voxel-space bounds
    BBox vsBounds;
    // Loop over each point
    size_t ptIndex = 0;
    for (std::vector<PointAttrState>::const_iterator i = m_basePointAttrs.begin(),
           end = m_basePointAttrs.end(); i != end; ++i, ++ptIndex) {
      float displ = displacementBounds(ptIndex);
      vsBounds.extendBy(vsSphereBounds(buffer->mapping(), i->wsCenter.value(), 
                                       i->radius.value() * (1.0 + displ)));
    }
    // Update acceleration structure
    updateAccelStruct();
    // Finally rasterize
    rasterize(vsBounds, buffer);
  }

  Log::print("  Time elapsed: " + str(timer.elapsed()));
}

//----------------------------------------------------------------------------//

void LineBase::updateAccelStruct() const
{
  // Compute bounds and average radius
  BBox wsBounds;
  double sumRadius = 0.0;
  for (size_t i = 0, size =  m_basePointAttrs.size() - 1; i < size; ++i) {
    float radius = m_basePointAttrs[i].radius;
    wsBounds = extendBounds(wsBounds, m_basePointAttrs[i].wsCenter.value(),
                            radius);
    sumRadius += radius;
  }
  double avgRadius = sumRadius / m_basePointAttrs.size();
  // Update acceleration structure domain
  //! \todo Cellsize should never be < 2 * buffer's voxel size
  Vector origin = wsBounds.min;
  size_t res = 32;
  Vector voxelSize = wsBounds.size() / static_cast<double>(res);
  float cellSize = max(voxelSize);
  if (cellSize < avgRadius) {
    res = std::ceil(max(wsBounds.size()) / avgRadius);
    cellSize = avgRadius;
  }
  m_gridAccel.clear(cellSize, res, origin);
  // Add line segments to hash
  for (size_t i = 0, size = m_basePointAttrs.size() - 1; i < size; ++i) {
    Vector p0(m_basePointAttrs[i].wsCenter.value());
    Vector p1(m_basePointAttrs[i + 1].wsCenter.value());
    float displ = std::max(displacementBounds(i), displacementBounds(i + 1));
    float radius = std::max(m_basePointAttrs[i].radius.value(),
                            m_basePointAttrs[i + 1].radius.value());
    m_gridAccel.addLine(p0, p1, radius * (1.0 + displ) + cellSize, i);
  }
}

//----------------------------------------------------------------------------//

bool LineBase::findClosestSegment(const RasterizationState &state, 
                                  SegmentInfo &info) const
{
  typedef Accel::UniformGrid<size_t>::HashVec HashVec;

  double minRelDist   = std::numeric_limits<double>::max();
  double t            = 0.0;
  double tExtend      = 0.0;
  double displacement = 0.0;
  const HashVec &vec  = m_gridAccel.get(state.wsP);

  if (vec.size() == 0) {
    return false;
  }

  for (HashVec::const_iterator iIdx = vec.begin(), end = vec.end();
       iIdx != end; ++iIdx) {
    // Segment index
    const size_t i = *iIdx;
    // Find closest point on Line
    Vector p0(m_basePointAttrs[i].wsCenter.value());
    Vector p1(m_basePointAttrs[i + 1].wsCenter.value());
    Vector pOnLine = Math::closestPointOnLineSegment(p0, p1, state.wsP, 
                                                     t, tExtend);
    // Compare distance to radius
    double radius = Math::fit01(t, m_basePointAttrs[i].radius.value(), 
                                m_basePointAttrs[i + 1].radius.value());
    double dist = (state.wsP - pOnLine).length();
    double relDist = dist / radius;
    // If within radius, update info
    if (relDist < minRelDist) {
      minRelDist = relDist;
      displacement = std::max(displacementBounds(i), displacementBounds(i + 1));
      info.radius = radius;
      info.distance = dist;
      info.index = i;
      if (i == 0 && t == 0.0 || i == (m_basePointAttrs.size() - 2) && t == 1.0) {
        info.t = tExtend;
      } else {
        info.t = t;
      }
    }
  }

  double maxDist = (info.radius * (1.0 + displacement) + 
                    state.wsVoxelSize.length() * 0.5);
  
  return info.distance < maxDist;
}

//----------------------------------------------------------------------------//

void LineBase::updatePolyAttrs
(Geo::AttrVisitor::const_iterator i) const
{
  m_basePolyAttrs.update(i);
}

//----------------------------------------------------------------------------//

void LineBase::updatePointAttrs
(Geo::AttrVisitor::const_iterator iPoint, const size_t numPoints) const
{
  m_basePointAttrs.resize(numPoints);
  for (size_t i = 0; i < numPoints; ++i, ++iPoint) {
    m_basePointAttrs[i].update(iPoint);
  }
}

//----------------------------------------------------------------------------//
// LineBase::PointAttrState
//----------------------------------------------------------------------------//

void LineBase::PointAttrState::update
(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(wsCenter);
  i.update(wsVelocity);
  i.update(radius);
  i.update(density);
}

//----------------------------------------------------------------------------//
// LineBase::PolyAttrState
//----------------------------------------------------------------------------//

void LineBase::PolyAttrState::update
(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(antialiased);
}

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
