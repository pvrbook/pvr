//----------------------------------------------------------------------------//

/*! \file CompositeVolume.cpp
  Contains implementations of CompositeVolume class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Volumes/CompositeVolume.h"

// System includes

// Library includes

#include <boost/foreach.hpp>

#include <Field3D/Field3DFile.h>
#include <Field3D/DenseField.h>
#include <Field3D/FieldInterp.h>

// Project headers

#include "pvr/Constants.h"
#include "pvr/Log.h"
#include "pvr/VoxelBuffer.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace pvr;
  using namespace pvr::Render;

  //--------------------------------------------------------------------------//

  struct MatchName : 
    public std::unary_function<CompositeVolume::ChildAttrs, bool>
  {
    MatchName(const std::string &name)
      : m_match(name)
    { }
    bool operator()(const CompositeVolume::ChildAttrs &test)
    {
      return test.name == m_match;
    }
  private:
    const std::string m_match;
  };

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

using namespace pvr::Util; 

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// CompositeVolume
//----------------------------------------------------------------------------//

//! \todo Finish implementing
Volume::AttrNameVec CompositeVolume::attributeNames() const
{
  AttrNameVec attrs;
  // Collect all attributes
  BOOST_FOREACH (Volume::CPtr child, m_volumes) {
    AttrNameVec childAttrs = child->attributeNames();
    attrs.insert(attrs.begin(), childAttrs.begin(), childAttrs.end());
  }
  // Unique attributes
  // Return unique'd vector
  return attrs;
}

//----------------------------------------------------------------------------//

Color CompositeVolume::sample(const VolumeSampleState &state,
                              const VolumeAttr &attribute) const
{
  if (attribute.index() == VolumeAttr::IndexNotSet) {
    setupAttribute(attribute);
  }
  if (attribute.index() == VolumeAttr::IndexInvalid) {
    return Colors::zero();
  }

  Color value = Colors::zero();
  int attrIndex = attribute.index();

  for (size_t i = 0, size = m_volumes.size(); i < size; ++i) {
    const VolumeAttr &childAttr = m_childAttrs[attrIndex].attrs[i];
    value += m_volumes[i]->sample(state, childAttr);
  }
  
  return value;
}

//----------------------------------------------------------------------------//

IntervalVec CompositeVolume::intersect(const RenderState &state) const
{
  IntervalVec intervals;
  BOOST_FOREACH (Volume::CPtr child, m_volumes) {
    IntervalVec childIntervals = child->intersect(state);
    intervals.insert(intervals.begin(),
                     childIntervals.begin(), childIntervals.end());
  }
  return intervals;
}

//----------------------------------------------------------------------------//

void CompositeVolume::add(Volume::CPtr child)
{
  m_volumes.push_back(child);
}

//----------------------------------------------------------------------------//

void CompositeVolume::setupAttribute(const VolumeAttr &attribute) const
{
  ChildAttrsVec::const_iterator i = 
    find_if(m_childAttrs.begin(), m_childAttrs.end(), 
            MatchName(attribute.name()));

  if (i != m_childAttrs.end()) {
    // Attribute already exists in m_childAttrs, so set index directly
    attribute.setIndex(i - m_childAttrs.begin());
  } else {
    // Attribute does not yet exist, look for it among children
    AttrNameVec attrs = attributeNames();
    if (find(attrs.begin(), attrs.end(), attribute.name()) != attrs.end()) {
      // One or more children have the attribute
      ChildAttrs newAttr;
      newAttr.name = attribute.name();
      newAttr.attrs.resize(m_volumes.size(), VolumeAttr(attribute.name()));
      m_childAttrs.push_back(newAttr);
      attribute.setIndex(m_childAttrs.size() - 1);
    } else {
      // No child has the attribute. Mark as invalid
      attribute.setIndexInvalid();
    }
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
