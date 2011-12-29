//-*-c++-*--------------------------------------------------------------------//

/*! \file CompositeVolume.h
  Contains the CompositeVolume class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_COMPOSITEVOLUME_H__
#define __INCLUDED_PVR_COMPOSITEVOLUME_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

#include <boost/foreach.hpp>

// Project headers

#include "pvr/Volumes/Volume.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// CompositeVolume
//----------------------------------------------------------------------------//

/*! \class CompositeVolume
  \brief Takes an arbitrary number of input Volumes, and exposes them as a
  single instance. Attribute values are composited/blended.
 */

//----------------------------------------------------------------------------//

class CompositeVolume : public Volume
{
public:

  // Structs -------------------------------------------------------------------

  //! This struct is used to hold a set of VolumeAttr instances that can be
  //! used to sample the child volumes. In general, ChildAtts::attrs will be
  //! the same length as CompositeVolume::m_volumes.
  struct ChildAttrs
  {
    std::string             name; 
    std::vector<VolumeAttr> attrs;
  };

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(CompositeVolume);
  typedef std::vector<ChildAttrs> ChildAttrsVec;

  // Ctor, factory -------------------------------------------------------------

  //! Specific factory method
  static Ptr create()
  { return Ptr(new CompositeVolume); }

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "CompositeVolume"; }

  // From Volume ---------------------------------------------------------------

  virtual AttrNameVec attributeNames() const;
  virtual VolumeSample sample(const VolumeSampleState &state,
                              const VolumeAttr &attribute) const;
  virtual IntervalVec intersect(const RayState &state) const;

  // Main methods --------------------------------------------------------------

  void add(Volume::CPtr child);

protected:

  // Utility methods -----------------------------------------------------------

  void setupAttribute(const VolumeAttr &attribute) const;

  // Protected data members ----------------------------------------------------

  std::vector<Volume::CPtr> m_volumes;
  mutable ChildAttrsVec     m_childAttrs;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
