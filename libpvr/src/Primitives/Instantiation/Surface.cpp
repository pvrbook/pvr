//----------------------------------------------------------------------------//

/*! \file Surface.cpp
  Contains implementations of Inst::Surface class 
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/Instantiation/Surface.h"

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
// Surface
//----------------------------------------------------------------------------//

ModelerInput::Ptr Surface::execute(const Geo::Geometry::CPtr geo) const
{
  typedef AttrVisitor::const_iterator AttrIter;

  assert(geo != NULL);
  assert(geo->meshes() != NULL);

  if (!geo->meshes()) {
    Log::warning("Instantiation primitive has no meshes. "
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

  // Loop over input points ---

  size_t idx = 0;
  ProgressReporter progress(2.5f, "  ");

  Log::print("Surface processing " + str(geo->meshes()->size()) +
             " input meshes");
  Log::print("  Output: " + str(numInstancePoints) + " points");

  // Iteration variables
  Meshes::CPtr meshes      (geo->meshes());
  AttrVisitor  meshVisitor (meshes->meshAttrs(), m_params);
  AttrVisitor  pointVisitor(meshes->pointAttrs(), m_params);

  for (AttrIter iMesh = meshVisitor.begin(), endMesh = meshVisitor.end(); 
       iMesh != endMesh; ++iMesh) {
    // Update mesh attributes
    updateSurfAttrs(iMesh);
    // Update point attribute
    size_t first     = meshes->startPoint(iMesh.index());
    size_t numCols   = meshes->numCols(iMesh.index());
    size_t numRows   = meshes->numRows(iMesh.index());
    size_t numPoints = numCols * numRows;
    updatePointAttrs(pointVisitor.begin(first), numPoints);
    // Seed random number generator
    Imath::Rand48 rng(1);
    // For each instance
    for (int i = 0; i < m_surfAttrs.numPoints; ++i, ++idx) {
      // Check if user terminated
      Sys::Interrupt::throwOnAbort();
      // Print progress
      progress.update(static_cast<float>(idx) / numInstancePoints);
      // Randomize local space position
      V3f lsP;
      lsP.x = rng.nextf();
      lsP.y = rng.nextf();
      lsP.z = rng.nextf();
      // Let s,t be floating-point index
      float s = lsP.x * (numCols - 1);
      float t = lsP.y * (numRows - 1);
      // Interpolate instance attributes
      V3f instanceDensity = SURFACE_INST_INTERP(density, s, t);
      V3f instanceWsP     = SURFACE_INST_INTERP(wsP, s, t);
      V3f instanceWsV     = SURFACE_INST_INTERP(wsVelocity, s, t);
      V3f wsN             = SURFACE_INST_INTERP(wsNormal, s, t).normalized();
      float thickness     = SURFACE_INST_INTERP(thickness, s, t);
      // Offset along normal
      instanceWsP += Math::fit01(lsP.z, -1.0f, 1.0f) * wsN * thickness;
      // Apply noises
      V3f nsP = lsP;
      if (m_surfAttrs.doDensNoise) {
        V3f nsLookupP = nsP / m_surfAttrs.densScale.value();
        float noise = m_surfAttrs.densFractal->eval(nsLookupP);
        float fade = edgeFade(lsP.x, lsP.y, lsP.z);
        instanceDensity *= noise + fade;
      }
      if (m_surfAttrs.doDispNoise) {
        V3f nsLookupP = nsP / m_surfAttrs.dispScale.value();
        V3f disp = m_surfAttrs.dispFractal->evalVec(nsLookupP);
        instanceWsP += disp.x * V3f(0.0) * thickness * 
          m_surfAttrs.dispAmplitude.value();
        instanceWsP += disp.y * V3f(0.0) * thickness * 
          m_surfAttrs.dispAmplitude.value();
        instanceWsP += disp.z * wsN * thickness * 
          m_surfAttrs.dispAmplitude.value();
      }
      // Set instance attributes
      particles->setPosition(idx, instanceWsP);
      points.setVectorAttr(vRef, idx, instanceWsV);
      points.setVectorAttr(densityRef, idx, instanceDensity);
      points.setFloatAttr(radiusRef, idx, 0, m_surfAttrs.instanceRadius);
    }
  }

  // Update state
  m_batch.geo = geo;

  return result;
}

//----------------------------------------------------------------------------//

BBox Surface::wsBounds(Geo::Geometry::CPtr geometry) const
{
  typedef AttrVisitor::const_iterator AttrIter;

  assert(geometry != NULL);

  if (!geometry->meshes()) {
    Log::warning("Instantiation primitive has no meshes. "
                 "Skipping bounds generation.");
    return BBox();
  }

  // Iteration variables
  BBox           wsBBox;
  Meshes::CPtr   meshes = geometry->meshes();
  AttrVisitor    meshVisitor(meshes->meshAttrs(), m_params);
  AttrVisitor    pointVisitor(meshes->pointAttrs(), m_params);

  for (AttrIter iMesh = meshVisitor.begin(), endMesh = meshVisitor.end(); 
       iMesh != endMesh; ++iMesh) {
    // Update mesh attributes
    updateSurfAttrs(iMesh);
    // Update point attribute
    size_t first = meshes->startPoint(iMesh.index());
    size_t numRows = meshes->numRows(iMesh.index());
    size_t numCols = meshes->numCols(iMesh.index());
    size_t numPoints = numRows * numCols;
    updatePointAttrs(pointVisitor.begin(first), numPoints);
    // Compute world-space bounds
    size_t index = 0;
    for (std::vector<PointAttrState>::const_iterator i = m_pointAttrs.begin(),
           end = m_pointAttrs.end(); i != end; ++i, ++index) {
      Vector thickness = Vector(i->thickness.value());
      float displacement = 0.0f;
      if (m_surfAttrs.doDispNoise) {
        Noise::Fractal::Range range = m_surfAttrs.dispFractal->range();
        double maxDispl = m_surfAttrs.dispAmplitude * 
          std::max(std::abs(range.first), std::abs(range.second));
        displacement = Math::max(Vector(maxDispl));
      }
      Vector wsV   = i->wsVelocity.value();
      Vector wsP   = i->wsP.value();
      Vector wsEnd = wsP + wsV * RenderGlobals::dt();
      wsBBox = extendBounds(wsBBox, wsP, thickness * (1.0 + displacement));
      wsBBox = extendBounds(wsBBox, wsEnd, thickness * (1.0 + displacement));
    }
  }

  return wsBBox;
}

//----------------------------------------------------------------------------//
  
size_t Surface::pointIdx(const float s, const float t, 
                         const size_t numCols, const size_t numRows, 
                         const int xOff, const int yOff) const
{
  size_t x = static_cast<size_t>(std::floor(s)) + xOff;
  size_t y = static_cast<size_t>(std::floor(t)) + yOff;
  x = Imath::clamp(x, static_cast<size_t>(0), numCols - 1);
  y = Imath::clamp(y, static_cast<size_t>(0), numRows - 1);
  return x + y * numCols;
}

//----------------------------------------------------------------------------//

float Surface::edgeFade(float x, float y, float z) const
{
  // Mirror at 0.5 for each dimension
  x = std::min(x, 1.0f - x);
  y = std::min(y, 1.0f - y);
  z = std::min(z, 1.0f - z);
  
  // Get fit param 
  V3f fit = m_surfAttrs.densFade.value();

  // Re-fit according to param
  if (fit.x > 0.0f) {
    x = Math::fit(x, 0.0f, fit.x, -1.0f, 1.0f);
  } else {
    x = 0.0f;
  }
  if (fit.y > 0.0f) {
    y = Math::fit(y, 0.0f, fit.y, -1.0f, 1.0f);
  } else {
    y = 0.0f;
  }
  if (fit.z > 0.0f) {
    z = Math::fit(z, 0.0f, fit.z, -1.0f, 1.0f);
  } else {
    z = 0.0f;
  }

  // Clamp
  x = Imath::clamp(x, -1.0f, 1.0f);
  y = Imath::clamp(y, -1.0f, 1.0f);
  z = Imath::clamp(z, -1.0f, 1.0f);

  // Return min
  return std::min(std::min(x, y), z);
}


//----------------------------------------------------------------------------//

size_t Surface::numOutputPoints(const Geo::Geometry::CPtr geo) const
{
  size_t count = 0;
  AttrVisitor visitor(geo->meshes()->meshAttrs(), m_params);

  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i) {
    m_surfAttrs.update(i);
    count += m_surfAttrs.numPoints.value();
  }
  
  return count;
}

//----------------------------------------------------------------------------//

void Surface::updateSurfAttrs(Geo::AttrVisitor::const_iterator i) const
{
  // Update base class
  // SurfaceBase::updateSurfAttrs(i);
  // Update this class
  m_surfAttrs.update(i);
}

//----------------------------------------------------------------------------//

void Surface::updatePointAttrs(Geo::AttrVisitor::const_iterator iPoint, 
                            const size_t numPoints) const
{
  // Update base class
  // SurfaceBase::updatePointAttrs(iPoint, numPoints);
  // Update this class
  m_pointAttrs.resize(numPoints);
  for (size_t i = 0; i < numPoints; ++i, ++iPoint) {
    m_pointAttrs[i].update(iPoint);
  }
}

//----------------------------------------------------------------------------//
// Point::SurfAttrState
//----------------------------------------------------------------------------//

void Surface::SurfAttrState::update(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(instanceRadius);
  i.update(numPoints);
  i.update(doFill);
  i.update(densScale);
  i.update(densFade);
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
// Surface::PointAttrState
//----------------------------------------------------------------------------//

void Surface::PointAttrState::update(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(wsP);
  i.update(wsNormal);
  i.update(wsVelocity);
  i.update(thickness);
  i.update(density);
}

//----------------------------------------------------------------------------//

} // namespace Inst
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
