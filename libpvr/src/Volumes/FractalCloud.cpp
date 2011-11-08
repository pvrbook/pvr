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

Color FractalCloud::sample(const VolumeSampleState &state,
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
    return Colors::zero();
  }
  
  double distFunc = 1.0 - state.wsP.length();

  return Color(distFunc + m_fractal->eval(state.wsP)) * m_density;
}

//----------------------------------------------------------------------------//

IntervalVec FractalCloud::intersect(const RenderState &state) const
{
  Fractal::Range range = m_fractal->range();
  double maxDisplacement = (1.0 + range.second) * Vector(1.0).length();
  BBox wsBounds(Vector(-maxDisplacement), Vector(maxDisplacement));

  double t0, t1;
  if (Math::intersect(state.wsRay, wsBounds, t0, t1)) {
    return IntervalVec(1, Interval(t0, t1, m_stepLength));
  } else {
    return IntervalVec();
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
