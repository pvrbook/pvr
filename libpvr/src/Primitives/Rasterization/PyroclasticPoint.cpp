//----------------------------------------------------------------------------//

/*! \file PyroclasticPoint.cpp
  Contains implementations of Rast::PyroclasticPoint class 
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/Rasterization/PyroclasticPoint.h"

// System includes

// Library includes

#include <Field3D/Field.h>

// Project includes

#include "pvr/RenderGlobals.h"
#include "pvr/Interrupt.h"
#include "pvr/Log.h"
#include "pvr/Math.h"
#include "pvr/ModelingUtils.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace pvr;
  
  //--------------------------------------------------------------------------//

  float sphereSDF(const Vector &wsP, const Vector &wsCenter, 
                  const float wsRadius)
  {
    return (wsCenter - wsP).length() - wsRadius;
  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace pvr::Geo;
using namespace pvr::Noise;
using namespace pvr::Sys;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// PyroclasticPoint
//----------------------------------------------------------------------------//

PyroclasticPoint::Ptr PyroclasticPoint::create()
{
  return Ptr(new PyroclasticPoint);
}

//----------------------------------------------------------------------------//

//! \todo Create new base class that only leaves getSample virtual
void PyroclasticPoint::execute(Geo::Geometry::CPtr geometry, 
                               VoxelBuffer::Ptr buffer) const
{
  using namespace Field3D;
  using namespace std;
  
  const AttrTable &points = geometry->particles()->pointAttrs();

  Timer             timer;
  size_t            count = 0;
  ProgressReporter  progress(2.5f, "  ");
  AttrVisitor       visitor(points, m_params);
  FieldMapping::Ptr mapping(buffer->mapping());
 
  PVR_PRIM_SANITY_CHECK("PyroclasticPoint");

  Log::print("Pyroclastic point primitive processing " + str(points.size()) + 
             " input points");

  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i, ++count) {
    // Check if user terminated
    Sys::Interrupt::throwOnAbort();
    // Print progress
    progress.update(static_cast<float>(count) / points.size());
    // Update attributes
    m_attrs.update(i);
    // Transform to voxel space
    Vector vsP;
    mapping->worldToVoxel(m_attrs.wsCenter.as<Vector>(), vsP);
    // Check fractal range
    Fractal::Range range = m_attrs.fractal->range();
    // Calculate rasterization bounds
    float totalRadius = m_attrs.radius + 
      m_attrs.radius * m_attrs.amplitude * range.second;
    BBox vsBounds = vsSphereBounds(mapping, m_attrs.wsCenter.as<Vector>(), 
                                   totalRadius);
    // Call rasterize(), which will come back and query getSample() for values
    rasterize(vsBounds, buffer);
  }

  Log::print("  Time elapsed: " + str(timer.elapsed()));
}

//----------------------------------------------------------------------------//

void PyroclasticPoint::getSample(const RasterizationState &state,
                                 RasterizationSample &sample) const
{
  // Transform to the point's local coordinate system
  float radius = m_attrs.radius;
  Vector lsP, lsPUnrot = (state.wsP - m_attrs.wsCenter.as<Vector>()) / radius;
  m_attrs.rotation.multVecMatrix(lsPUnrot, lsP);
  Vector nsP = m_attrs.displace2D ? lsP.normalized() : lsP;
  // Offset by seed
  Rand32 rng(m_attrs.seed.value());
  Vector offset;
  offset.x = rng.nextf(-100, 100);
  offset.y = rng.nextf(-100, 100);
  offset.z = rng.nextf(-100, 100);
  nsP += offset;
  // Compute fractal function
  double fractalFunc = m_attrs.fractal->eval(nsP);
  fractalFunc = Math::gamma(fractalFunc, m_attrs.gamma.value());
  fractalFunc *= m_attrs.amplitude;
  // Calculate sample value
  if (m_attrs.pyroclastic) {
    // Thresholded
    double sphereFunc = lsP.length() - 1.0;
    float filterWidth = state.wsVoxelSize.length();
    double thresholdWidth = filterWidth * 0.5 / radius;
    double pyroValue;
    if (m_attrs.antialiased) {
      pyroValue = Math::fit(sphereFunc - fractalFunc, 
                            -thresholdWidth, thresholdWidth, 1.0, 0.0);
    } else {
      pyroValue = (sphereFunc - fractalFunc) < 0.0 ? 1.0 : 0.0;
    }
    pyroValue = Imath::clamp(pyroValue, 0.0, 1.0);
    sample.value = m_attrs.density.value() * pyroValue;
  } else {
    // Normal
    double distanceFunc = 1.0 - lsP.length();
    sample.value = m_attrs.density.value() * 
    std::max(0.0, distanceFunc + fractalFunc);
  }
  sample.wsVelocity = m_attrs.wsVelocity.as<Vector>();
}

//----------------------------------------------------------------------------//

BBox PyroclasticPoint::pointWsBounds
(const Geo::AttrVisitor::const_iterator &i) const
{
  BBox wsBBox;
  m_attrs.update(i);
  // Check fractal range
  Fractal::Range range = m_attrs.fractal->range();
  // Compute start and end of motion
  Vector wsStart = m_attrs.wsCenter.value();
  Vector wsEnd = m_attrs.wsCenter.value() + 
    m_attrs.wsVelocity.value() * RenderGlobals::dt();
  // Pad to account for displacement
  Vector padding = Vector(m_attrs.radius.value() + 
                          range.second * m_attrs.amplitude * m_attrs.radius);
  wsBBox.extendBy(wsStart + padding);
  wsBBox.extendBy(wsStart - padding);
  wsBBox.extendBy(wsEnd + padding);
  wsBBox.extendBy(wsEnd - padding);
  return wsBBox;
}
  
//----------------------------------------------------------------------------//
// PyroclasticPoint::AttrState
//----------------------------------------------------------------------------//

void PyroclasticPoint::
AttrState::update(const Geo::AttrVisitor::const_iterator &i)
{
  using namespace Noise;
  // Update point attributes
  i.update(wsCenter);
  i.update(wsVelocity);
  i.update(orientation);
  i.update(radius);
  i.update(density);
  i.update(seed);
  i.update(scale);
  i.update(octaves);
  i.update(octaveGain);
  i.update(lacunarity);
  i.update(amplitude);
  i.update(gamma);
  i.update(displace2D);
  i.update(absNoise);
  i.update(antialiased);
  i.update(pyroclastic);
  // Set up fractal
  NoiseFunction::CPtr noise;
  if (absNoise) {
    noise = NoiseFunction::CPtr(new AbsPerlinNoise);
  } else {
    noise = NoiseFunction::CPtr(new PerlinNoise);
  }
  fractal = Fractal::CPtr(new fBm(noise, scale, octaves, 
                                  octaveGain, lacunarity));
  // Set up rotation matrix
  rotation = Euler(orientation.value()).toMatrix44().transpose();
}

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
