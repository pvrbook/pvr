//----------------------------------------------------------------------------//

/*! \file AttrUtil.cpp
  Contains implementations of Attr and AttrVisitor class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/AttrUtil.h"

// System includes

// Library includes

// Project headers



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

using namespace boost;
using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// AttrVisitor
//----------------------------------------------------------------------------//

AttrVisitor::AttrVisitor(const AttrTable &points, 
                         const Util::ParamMap &globals)
  : m_points(points), m_globals(globals)
{
  // Empty
}

//----------------------------------------------------------------------------//

AttrVisitor::const_iterator 
AttrVisitor::begin() const
{
  return const_iterator(*this, 0);
}

//----------------------------------------------------------------------------//

AttrVisitor::const_iterator 
AttrVisitor::begin(const size_t idx) const
{
  return const_iterator(*this, idx);
}

//----------------------------------------------------------------------------//

AttrVisitor::const_iterator 
AttrVisitor::end() const
{
  return const_iterator(*this, m_points.size());
}

//----------------------------------------------------------------------------//

AttrVisitor::const_iterator 
AttrVisitor::end(const size_t idx) const
{
  return const_iterator(*this, idx + 1);
}

//----------------------------------------------------------------------------//

//! Specialization for <int>
template <>
void initialize(const AttrTable &points, const Util::ParamMap &globals,
                Attr<int> &attr)
{
  using namespace std;

  AttrRef ref = points.intAttrRef(attr.name());
  if (ref.isValid() && ref.arraySize() == 1) {
    attr.setIterator(points.intAttrElems(ref).begin());
    attr.setActive(true);
  } else {
    int defaultValue;
    map<string, int>::const_iterator i = globals.intMap.find(attr.name());
    if (i != globals.intMap.end()) {
      defaultValue = i->second;
      attr.setDefaultValue(defaultValue);
    } else {
      // No default value found in globals map. Revert to PointArrayAttr's
      // own default.
    }
    attr.setActive(false);
  }
}

//----------------------------------------------------------------------------//

//! Specialization for <float>
template <>
void initialize(const AttrTable &points, const Util::ParamMap &globals,
                Attr<float> &attr)
{
  using namespace std;

  AttrRef ref = points.floatAttrRef(attr.name());
  if (ref.isValid() && ref.arraySize() == 1) {
    attr.setIterator(points.floatAttrElems(ref).begin());
    attr.setActive(true);
  } else {
    float defaultValue;
    map<string, float>::const_iterator i = globals.floatMap.find(attr.name());
    if (i != globals.floatMap.end()) {
      defaultValue = i->second;
      attr.setDefaultValue(defaultValue);
    } else {
      // No default value found in globals map. Revert to PointArrayAttr's
      // own default.
    }
    attr.setActive(false);
  }
}

//----------------------------------------------------------------------------//

//! Specialization for <V3f>
template <>
void initialize(const AttrTable &points, const Util::ParamMap &globals,
                Attr<Imath::V3f> &attr)
{
  using namespace std;
  using namespace Imath;

  AttrRef ref = points.vectorAttrRef(attr.name());
  if (ref.isValid() && ref.arraySize() == 1) {
    attr.setIterator(points.vectorAttrElems(ref).begin());
    attr.setActive(true);
  } else {
    Imath::V3f defaultValue;
    map<string, V3f>::const_iterator i = globals.vectorMap.find(attr.name());
    if (i != globals.vectorMap.end()) {
      defaultValue = i->second;
      attr.setDefaultValue(defaultValue);
    } else {
      // No default value found in globals map. Revert to PointArrayAttr's
      // own default.
    }
    attr.setActive(false);
  }
}

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//
