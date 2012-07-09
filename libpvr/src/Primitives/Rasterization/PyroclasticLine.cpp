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

/*! \file PyroclasticLine.cpp
  Contains implementations of Rast::PyroclasticLine class 
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/Rasterization/PyroclasticLine.h"

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
using namespace pvr::Noise;

//----------------------------------------------------------------------------//
// Defines
//----------------------------------------------------------------------------//

#define PYRO_LINE_INTERP(variable, info)                          \
  Math::fit01(info.t, m_pointAttrs[info.index].variable.value(),  \
              m_pointAttrs[info.index + 1].variable.value())      \

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//

Vector lineWsToLs(const Vector &wsP, const Vector &wsE1, const Vector &wsE2,
                  const Vector &wsE3, const Vector &wsCenter, 
                  const float u, const float radius)
{
  // Build transform
  Matrix lsToWs = Math::coordinateSystem(wsE1, wsE2, wsE3, wsCenter);
  Matrix wsToLs = lsToWs.inverse();
  // Compute coordinate
  Vector lsP = wsP * wsToLs;
  return Vector(lsP.x / radius, lsP.y / radius, u);
}

//----------------------------------------------------------------------------//
// PyroclasticLine
//----------------------------------------------------------------------------//

PyroclasticLine::Ptr PyroclasticLine::create()
{
  return Ptr(new PyroclasticLine);
}

//----------------------------------------------------------------------------//

void PyroclasticLine::getSample(const RasterizationState &state,
                                RasterizationSample &sample) const
{
  if (m_basePointAttrs.size() < 2) {
    return;
  }

  // Per-Polygon attribute
  const bool    isPyroclastic = m_polyAttrs.pyroclastic;
  const bool    isPyro2D      = m_polyAttrs.pyro2D;
  const V3f     scale         = m_polyAttrs.scale;
  Fractal::CPtr fractal       = m_polyAttrs.fractal;

  SegmentInfo info;
  
  if (findClosestSegment(state, info)) {

    // Interpolate values along line
    sample.wsVelocity     = LINE_INTERP(wsVelocity, info);
    Imath::V3f wsCenter   = LINE_INTERP(wsCenter, info);
    Imath::V3f density    = LINE_INTERP(density, info);
    Imath::V3f N          = PYRO_LINE_INTERP(wsNormal, info);
    Imath::V3f T          = PYRO_LINE_INTERP(wsTangent, info);
    float      u          = PYRO_LINE_INTERP(u, info);
    float      gamma      = PYRO_LINE_INTERP(gamma, info);
    float      amplitude  = PYRO_LINE_INTERP(amplitude, info);

    // Transform to local space
    Vector lsP = lineWsToLs(state.wsP, N.cross(T), N, T, 
                            wsCenter, u, info.radius);

    // Normalize the length of the vector in the XY plane
    // if user wants "2D" style displacement.
    if (isPyroclastic && isPyro2D) {
      lsP = Math::normalizeXY(lsP);
    }

    // Transform to noise space
    Vector nsP = lsP / scale;

    // Evaluate fractal
    double fractalVal = fractal->eval(nsP);
    fractalVal = Math::gamma(fractalVal, gamma);
    fractalVal *= amplitude;

    // Calculate sample value
    if (isPyroclastic) {
      float  filterWidth  = state.wsVoxelSize.length() / info.radius;
      double distanceFunc = info.distance / info.radius - 1.0;
      float  pyro         = pyroclastic(distanceFunc, fractalVal, filterWidth);
      sample.value        = pyro * density;
    } else {
      double distanceFunc = 1.0 - info.distance / info.radius;
      float  noise        = std::max(0.0, distanceFunc + fractalVal);
      sample.value        = noise * density;
    }

  }
}

//----------------------------------------------------------------------------//

void PyroclasticLine::updatePolyAttrs(Geo::AttrVisitor::const_iterator i) const
{
  // Update base class
  LineBase::updatePolyAttrs(i);
  // Update this class
  m_polyAttrs.update(i);
  // Update fractal 
  NoiseFunction::CPtr noise;
  if (m_polyAttrs.absNoise) {
    noise = NoiseFunction::CPtr(new AbsPerlinNoise);
  } else {
    noise = NoiseFunction::CPtr(new PerlinNoise);
  }
  m_polyAttrs.fractal.reset(new fBm(noise, 1.0, m_polyAttrs.octaves, 
                                    m_polyAttrs.octaveGain, 
                                    m_polyAttrs.lacunarity));
}

//----------------------------------------------------------------------------//

void PyroclasticLine::updatePointAttrs(Geo::AttrVisitor::const_iterator iPoint, 
                                       const size_t numPoints) const
{
  // Update base class
  LineBase::updatePointAttrs(iPoint, numPoints);
  // Update this class
  m_pointAttrs.resize(numPoints);
  for (size_t i = 0; i < numPoints; ++i, ++iPoint) {
    m_pointAttrs[i].update(iPoint);
  }
}

//----------------------------------------------------------------------------//

float PyroclasticLine::displacementBounds(const size_t index) const
{
  float amplitude  = m_pointAttrs[index].amplitude;
  float scale      = Math::max(m_polyAttrs.scale.value());
  float lacunarity = m_polyAttrs.lacunarity;
  
  NoiseFunction::CPtr noise;
  if (m_polyAttrs.absNoise) {
    noise = NoiseFunction::CPtr(new AbsPerlinNoise);
  } else {
    noise = NoiseFunction::CPtr(new PerlinNoise);
  }

  Fractal::CPtr fractal(new fBm(noise, scale, m_polyAttrs.octaves, 
                                m_polyAttrs.octaveGain, lacunarity));

  return fractal->range().second * amplitude;
}

//----------------------------------------------------------------------------//
// PyroclasticLine::PolyAttrState
//----------------------------------------------------------------------------//

void PyroclasticLine::PolyAttrState::update
(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(scale);
  i.update(octaves);
  i.update(octaveGain);
  i.update(lacunarity);
  i.update(pyro2D);
  i.update(absNoise);
  i.update(pyroclastic);
}

//----------------------------------------------------------------------------//
// PyroclasticLine::PointAttrState
//----------------------------------------------------------------------------//

void PyroclasticLine::PointAttrState::update
(const Geo::AttrVisitor::const_iterator &i)
{
  i.update(wsNormal);
  i.update(wsTangent);
  i.update(u);
  i.update(amplitude);
  i.update(gamma);
}

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
