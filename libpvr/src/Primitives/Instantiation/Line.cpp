//----------------------------------------------------------------------------//

/*! \file Line.cpp
  Contains implementations of Inst::Line class 
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/Instantiation/Line.h"

// System includes

// Library includes

#include <OpenEXR/ImathRandom.h>

// Project includes

#include "pvr/AttrUtil.h"
#include "pvr/Geometry.h"
#include "pvr/Interrupt.h"
#include "pvr/Log.h"
#include "pvr/Math.h"
#include "pvr/ModelerInput.h"
#include "pvr/ModelingUtils.h"
#include "pvr/RenderGlobals.h"

#include "pvr/Primitives/Rasterization/Point.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace Field3D;

using namespace pvr::Geo;
using namespace pvr::Math;
using namespace pvr::Noise;
using namespace pvr::Sys;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Inst {

//----------------------------------------------------------------------------//
// Line
//----------------------------------------------------------------------------//

ModelerInput::Ptr Line::execute(const Geo::Geometry::CPtr geo) const
{
  typedef AttrVisitor::const_iterator AttrIter;

  assert(geo != NULL);
  assert(geo->polygons() != NULL);

  if (!geo->polygons()) {
    Log::warning("Instantiation primitive has no polygons. "
                 "Skipping execution.");
    return ModelerInput::Ptr();
  }

  // Handle batches ---

  if (geo == m_batch.geo) {
    return ModelerInput::Ptr();
  }

  // Set up output geometry ---

  size_t numInstancePoints = numOutputPoints(geo);

  Prim::Rast::Point::Ptr pointPrim = Prim::Rast::Point::create();
  ModelerInput::Ptr result = ModelerInput::create();
  Geometry::Ptr outGeo = Geometry::create();
  Particles::Ptr particles = Particles::create();

  AttrTable &points = particles->pointAttrs();
  particles->add(numInstancePoints);

  std::vector<float> oneDefault(1, 1.0);

  AttrRef vRef = points.addVectorAttr("v", Vector(0.0));
  AttrRef radiusRef = points.addFloatAttr("radius", 1, oneDefault);
  AttrRef densityRef = points.addVectorAttr("density", Vector(1.0));

  outGeo->setParticles(particles);
  result->setGeometry(outGeo);
  result->setVolumePrimitive(pointPrim);

  // Loop over input polygons ---

  size_t idx = 0;
  ProgressReporter progress(2.5f, "  ");

  Log::print("Line processing " + str(geo->polygons()->size()) +
             " input polygons");
  Log::print("  Output: " + str(numInstancePoints) + " points");

  // Iteration variables
  Polygons::CPtr polys = geo->polygons();
  AttrVisitor    polyVisitor(polys->polyAttrs(), m_params);
  AttrVisitor    pointVisitor(polys->pointAttrs(), m_params);

  for (AttrIter iPoly = polyVisitor.begin(), endPoly = polyVisitor.end(); 
       iPoly != endPoly; ++iPoly) {
    // Update poly attributes
    updatePolyAttrs(iPoly);
    // Update point attributes
    size_t first = polys->pointForVertex(iPoly.index(), 0);
    size_t numPoints = polys->numVertices(iPoly.index());
    updatePointAttrs(pointVisitor.begin(first), numPoints);
    // Seed random number generator
    Imath::Rand48 rng(1);
    // For each instance
    for (int i = 0; i < m_polyAttrs.numPoints; ++i, ++idx) {
      // Check if user terminated
      Sys::Interrupt::throwOnAbort();
      // Print progress
      progress.update(static_cast<float>(idx) / numInstancePoints);
      // Randomize local space position 
      V2f disk;
      if (m_polyAttrs.doFill) {
        disk = solidSphereRand<V2f>(rng);
      } else {
        disk = hollowSphereRand<V2f>(rng);
      }
      V3f lsP(disk.x, disk.y, rng.nextf());
      // Let t be floating-point index
      float t = lsP.z * (m_pointAttrs.size() - 1);
      // Interpolate instance attributes
      V3f instanceDensity = LINE_INST_INTERP(density, t);
      V3f instanceWsP     = LINE_INST_INTERP(wsP, t);
      V3f instanceWsV     = LINE_INST_INTERP(wsVelocity, t);
      V3f wsN             = LINE_INST_INTERP(wsNormal, t).normalized();
      V3f wsT             = LINE_INST_INTERP(wsTangent, t).normalized();
      float radius        = LINE_INST_INTERP(radius, t);
      // Compute third basis vector from N and T
      V3f wsNxT = wsN.cross(wsT);
      // Offset instance position based on local space coordinate
      instanceWsP += lsP.x * wsNxT * radius;
      instanceWsP += lsP.y * wsN * radius;
      // Apply noises
      V3f nsP = lsP;
      if (m_polyAttrs.doDensNoise) {
        V3f nsLookupP = nsP / m_polyAttrs.densScale.value();
        instanceDensity *= m_polyAttrs.densFractal->eval(nsLookupP);
      }
      if (m_polyAttrs.doDispNoise) {
        V3f nsLookupP = nsP / m_polyAttrs.dispScale.value();
        V3f disp = m_polyAttrs.dispFractal->evalVec(nsLookupP);
        instanceWsP += disp.x * wsNxT * radius * 
          m_polyAttrs.dispAmplitude.value();
        instanceWsP += disp.y * wsN * radius * 
          m_polyAttrs.dispAmplitude.value();
        instanceWsP += disp.z * wsT * radius * 
          m_polyAttrs.dispAmplitude.value();
      }
      // Set instance attributes
      particles->setPosition(idx, instanceWsP);
      points.setVectorAttr(vRef, idx, instanceWsV);
      points.setVectorAttr(densityRef, idx, instanceDensity);
      points.setFloatAttr(radiusRef, idx, 0, m_polyAttrs.instanceRadius);
    }
  }

  // Update state
  m_batch.geo = geo;

  return result;
}

//----------------------------------------------------------------------------//

BBox Line::wsBounds(Geo::Geometry::CPtr geometry) const
{
  typedef AttrVisitor::const_iterator AttrIter;

  assert(geometry != NULL);
  // assert(geometry->polygons() != NULL);

  if (!geometry->polygons()) {
    Log::warning("Instantiation primitive has no polygons. "
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
    for (std::vector<PointAttrState>::const_iterator i = m_pointAttrs.begin();
         i != m_pointAttrs.end(); ++i, ++index) {
      Vector radius      = Vector(i->radius.value());
      float displacement = 0.0f;
      if (m_polyAttrs.doDispNoise) {
        Noise::Fractal::Range range = m_polyAttrs.dispFractal->range();
        double maxDispl = m_polyAttrs.dispAmplitude * 
          std::max(std::abs(range.first), std::abs(range.second));
        displacement = Math::max(Vector(maxDispl));
      }
      Vector wsV   = i->wsVelocity.value();
      Vector wsP   = i->wsP.value();
      Vector wsEnd = wsP + wsV * RenderGlobals::dt();
      wsBBox = extendBounds(wsBBox, wsP, radius * (1.0 + displacement));
      wsBBox = extendBounds(wsBBox, wsEnd, radius * (1.0 + displacement));
    }
  }

  return wsBBox;
}

//----------------------------------------------------------------------------//

size_t Line::numOutputPoints(const Geo::Geometry::CPtr geo) const
{
  size_t count = 0;
  AttrVisitor visitor(geo->polygons()->polyAttrs(), m_params);

  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i) {
    m_polyAttrs.update(i);
    count += m_polyAttrs.numPoints.value();
  }
  
  return count;
}

//----------------------------------------------------------------------------//

void Line::updatePolyAttrs(Geo::AttrVisitor::const_iterator i) const
{
  // Update base class
  // LineBase::updatePolyAttrs(i);
  // Update this class
  m_polyAttrs.update(i);
}

//----------------------------------------------------------------------------//

void Line::updatePointAttrs(Geo::AttrVisitor::const_iterator iPoint, 
                            const size_t numPoints) const
{
  // Update base class
  // LineBase::updatePointAttrs(iPoint, numPoints);
  // Update this class
  m_pointAttrs.resize(numPoints);
  for (size_t i = 0; i < numPoints; ++i, ++iPoint) {
    m_pointAttrs[i].update(iPoint);
  }
}

//----------------------------------------------------------------------------//
// Point::PolyAttrState
//----------------------------------------------------------------------------//

void Line::PolyAttrState::update(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(instanceRadius);
  i.update(numPoints);
  i.update(doFill);
  i.update(densScale);
  i.update(densOctaves);
  i.update(densOctaveGain);
  i.update(densLacunarity);
  i.update(dispScale);
  i.update(dispOctaves);
  i.update(dispOctaveGain);
  i.update(dispLacunarity);
  i.update(dispAmplitude);
  i.update(doDensNoise);
  i.update(doDispNoise);
  // Set up fractals
  NoiseFunction::CPtr densNoise = NoiseFunction::CPtr(new PerlinNoise);
  densFractal = Fractal::CPtr(new fBm(densNoise, 1.0, densOctaves, 
                                      densOctaveGain, densLacunarity));
  NoiseFunction::CPtr dispNoise = NoiseFunction::CPtr(new PerlinNoise);
  dispFractal = Fractal::CPtr(new fBm(dispNoise, 1.0, dispOctaves, 
                                      dispOctaveGain, dispLacunarity));
}

//----------------------------------------------------------------------------//
// Line::PointAttrState
//----------------------------------------------------------------------------//

void Line::PointAttrState::update(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(wsP);
  i.update(wsNormal);
  i.update(wsTangent);
  i.update(wsVelocity);
  // i.update(u);
  i.update(radius);
  i.update(density);
}

//----------------------------------------------------------------------------//

} // namespace Inst
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
