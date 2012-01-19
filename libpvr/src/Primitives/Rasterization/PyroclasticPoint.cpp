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
  // Point attributes
  const V3f    &density       = m_attrs.density;  
  const float   wsRadius      = m_attrs.radius;
  const Vector  wsCenter      = m_attrs.wsCenter.as<Vector>();
  const Vector  wsVelocity    = m_attrs.wsVelocity.as<Vector>();
  const Matrix &rotation      = m_attrs.rotation;
  const bool    isPyroclastic = m_attrs.pyroclastic;
  const bool    isPyro2D      = m_attrs.pyro2D;
  const int     seed          = m_attrs.seed;
  const float   gamma         = m_attrs.gamma;
  const float   amplitude     = m_attrs.amplitude;
  Fractal::CPtr fractal       = m_attrs.fractal;

  // Transform to the point's local coordinate system
  Vector lsP, lsPUnrot = (state.wsP - wsCenter) / wsRadius;
  rotation.multVecMatrix(lsPUnrot, lsP);
  Vector nsP = lsP;

  // Normalize noise coordinate if '2D' displacement is desired
  if (isPyroclastic && isPyro2D) {
    nsP.normalize();
  }

  // Offset by seed
  nsP += Math::offsetVector<double>(seed);

  // Compute fractal function
  double fractalVal = fractal->eval(nsP);
  fractalVal = Math::gamma(fractalVal, gamma);
  fractalVal *= amplitude;

  // Calculate sample value
  if (isPyroclastic) {
    double sphereFunc   = lsP.length() - 1.0;
    float  filterWidth  = state.wsVoxelSize.length() / wsRadius;
    float  pyro         = pyroclastic(sphereFunc, fractalVal, filterWidth);
    sample.value        = density * pyro;
  } else {
    double distanceFunc = 1.0 - lsP.length();
    float  noise        = std::max(0.0, distanceFunc + fractalVal);
    sample.value        = density * noise;
  }

  // Update velocity
  sample.wsVelocity = wsVelocity;
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
  i.update(pyro2D);
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
