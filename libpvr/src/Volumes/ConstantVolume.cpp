//----------------------------------------------------------------------------//

/*! \file ConstantVolume.cpp
  Contains implementations of ConstantVolume utility functions
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Volumes/ConstantVolume.h"

// System includes

// Library includes

#include <OpenEXR/ImathBoxAlgo.h>

// Project headers

#include "pvr/Constants.h"
#include "pvr/Log.h"
#include "pvr/Math.h"
#include "pvr/Strings.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//

ConstantVolume::ConstantVolume(const Matrix &localToWorld)
  : m_localToWorld(localToWorld),
    m_maxAttrValue(0.0f)
{
  m_worldToLocal = localToWorld.inverse();
}

//----------------------------------------------------------------------------//

Volume::AttrNameVec ConstantVolume::attributeNames() const
{
  return m_attrNames;
}

//----------------------------------------------------------------------------//

VolumeSample ConstantVolume::sample(const VolumeSampleState &state,
                                    const VolumeAttr &attribute) const
{
  if (attribute.index() == VolumeAttr::IndexNotSet) {
    setIndexForName(attribute, m_attrNames);
  }
  if (attribute.index() == VolumeAttr::IndexInvalid) {
    return VolumeSample(Colors::zero(), m_phaseFunction);
  }

  // Check if sample falls within volume
  Vector lsP;
  m_worldToLocal.multVecMatrix(state.wsP, lsP);
  if (Bounds::zeroOne().intersects(lsP)) {
    return VolumeSample(m_attrValues[attribute.index()], m_phaseFunction);
  } else {
    return VolumeSample(Colors::zero(), m_phaseFunction);
  }
}

//----------------------------------------------------------------------------//

BBox ConstantVolume::wsBounds() const
{
  return Imath::transform(Bounds::zeroOne(), m_localToWorld);
}

//----------------------------------------------------------------------------//

IntervalVec ConstantVolume::intersect(const RayState &state) const
{
  // Transform ray to local space
  Ray lsRay;
  m_worldToLocal.multVecMatrix(state.wsRay.pos, lsRay.pos);
  m_worldToLocal.multDirMatrix(state.wsRay.dir, lsRay.dir);
  // Intersect against unity bounds
  double t0, t1;
  if (Math::intersect(lsRay, Bounds::zeroOne(), t0, t1)) {
    return IntervalVec(1, Interval(t0, t1, (t1 - t0) / 
                                   (std::sqrt(m_maxAttrValue) * 20.0)));
  } else {
    return IntervalVec();
  }
}

//----------------------------------------------------------------------------//

Volume::StringVec ConstantVolume::info() const
{
  StringVec info;
  for (size_t i = 0, size = m_attrNames.size(); i < size; ++i) {
    info.push_back(m_attrNames[i] + " : " + Util::str(m_attrValues[i]));
  }
  return info;
}

//----------------------------------------------------------------------------//

void ConstantVolume::addAttribute(const std::string &attrName, 
                               const Imath::V3f &value)
{
  // No need to add an attribute whose value is zero.
  if (Math::max(value) == 0.0) {
    return;
  }

  m_attrNames.push_back(attrName);
  m_attrValues.push_back(value);
  m_maxAttrValue = std::max(m_maxAttrValue, Math::max(value));
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
