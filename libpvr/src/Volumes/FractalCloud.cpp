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

/*! \file FractalCloud.cpp
  Contains implementations of FractalCloud class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Volumes/FractalCloud.h"

// System includes

// Library includes

// Project headers

#include "pvr/Constants.h"
#include "pvr/Log.h"
#include "pvr/Math.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

using namespace pvr::Util; 
using namespace pvr::Noise;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// FractalCloud
//----------------------------------------------------------------------------//

Volume::AttrNameVec FractalCloud::attributeNames() const
{
  AttrNameVec attrs;
  attrs.push_back("scattering");
  return attrs;
}

//----------------------------------------------------------------------------//

VolumeSample FractalCloud::sample(const VolumeSampleState &state,
                                  const VolumeAttr &attribute) const
{
  if (attribute.index() == VolumeAttr::IndexNotSet) {
    if (attribute.name() == "scattering") {
      attribute.setIndex(0);
    } else {
      attribute.setIndexInvalid();
    }
  }
  if (attribute.index() == VolumeAttr::IndexInvalid) {
    return VolumeSample(Colors::zero(), m_phaseFunction);
  }
  
  double distFunc = 1.0 - state.wsP.length();

  return VolumeSample(Color(distFunc + m_fractal->eval(state.wsP)) * m_density,
                      m_phaseFunction);
}

//----------------------------------------------------------------------------//

IntervalVec FractalCloud::intersect(const RayState &state) const
{
  Fractal::Range range = m_fractal->range();
  double maxDisplacement = (1.0 + range.second) * Vector(1.0).length();
  BBox wsBounds(Vector(-maxDisplacement), Vector(maxDisplacement));

  double t0, t1;
  if (Math::intersect(state.wsRay, wsBounds, t0, t1)) {
    double stepLength = 0.1 / std::log(1.0 + m_density);
    return IntervalVec(1, Interval(t0, t1, stepLength));
  } else {
    return IntervalVec();
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
