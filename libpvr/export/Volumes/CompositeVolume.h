//-*-c++-*--------------------------------------------------------------------//

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

#include "pvr/export.h"
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

class LIBPVR_PUBLIC CompositeVolume : public Volume
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

  PVR_TYPEDEF_SMART_PTRS(CompositeVolume);
  typedef std::vector<ChildAttrs> ChildAttrsVec;

  // Ctor, factory -------------------------------------------------------------

  //! Default constructor
  CompositeVolume()
    : m_compositePhaseFunction(new Phase::Composite)
  { 
    m_phaseFunction = m_compositePhaseFunction;
  }

  //! Specific factory method
  PVR_DEFINE_CREATE_FUNC(CompositeVolume);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(CompositeVolume);

  // From Volume ---------------------------------------------------------------

  virtual AttrNameVec  attributeNames() const;
  virtual VolumeSample sample(const VolumeSampleState &state,
                              const VolumeAttr &attribute) const;
  virtual BBox         wsBounds() const;
  virtual IntervalVec  intersect(const RayState &state) const;
  virtual CVec         inputs() const;

  // Main methods --------------------------------------------------------------

  //! Adds a new volume to the composite.
  void                 add(Volume::CPtr child);

protected:

  // Utility methods -----------------------------------------------------------

  void                 setupAttribute(const VolumeAttr &attribute) const;

  // Protected data members ----------------------------------------------------

  //! Array of volumes
  std::vector<Volume::CPtr> m_volumes;
  //! Array of child attributes. Used for sampling the child volumes.
  mutable ChildAttrsVec     m_childAttrs;
  //! Pointer to composite phase function
  Phase::Composite::Ptr     m_compositePhaseFunction;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
