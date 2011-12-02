//-*-c++-*--------------------------------------------------------------------//

/*! \file Volume.h
  Contains the Volume class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_VOLUME_H__
#define __INCLUDED_PVR_VOLUME_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Project headers

#include "pvr/ParamBase.h"
#include "pvr/VolumeAttr.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Volume
//----------------------------------------------------------------------------//

//! Base class for volumes that are used in the rendering/raymarching.
class Volume : public Util::ParamBase
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Volume);
  typedef std::vector<std::string> AttrNameVec;

  // To be implemented by subclasses -------------------------------------------

  virtual AttrNameVec attributeNames() const = 0;

  //! Returns the value of the volume node for a given point.
  //! \note The point position is found in state.wsP
  virtual Color sample(const VolumeSampleState &state,
                       const VolumeAttr &attribute) const = 0;

  //! Returns a vector of intersection intervals (start/end points) that
  //! intersect the ray.
  //! \note The ray is found in state.wsRay
  virtual IntervalVec intersect(const RenderState &state) const = 0;

};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

//! Sets the attribute index if attribute.name() == name, otherwise sets it to 
//! IndexInvalid
void setupVolumeAttr(const VolumeAttr &attribute, const std::string &name,
                     const int index);
  
//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
