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

/*! \file ConstantVolume.h
  Contains the ConstantVolume class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_CONSTANTVOLUME_H__
#define __INCLUDED_PVR_CONSTANTVOLUME_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Project headers

#include "pvr/Curve.h"
#include "pvr/Volumes/Volume.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// ConstantVolume
//----------------------------------------------------------------------------//

class ConstantVolume : public Volume
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(ConstantVolume);

  // Constructors and destructor -----------------------------------------------

  ConstantVolume(const Util::MatrixCurve &localToWorld);
  PVR_DEFINE_CREATE_FUNC_1_ARG(ConstantVolume, const Util::MatrixCurve &);
  
  // Main methods --------------------------------------------------------------

  //! Adds an attribute to be exposed. The value provided will be assumed for
  //! the entire volume.
  void addAttribute(const std::string &attrName, const Imath::V3f &value);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(ConstantVolume);

  // From Volume ---------------------------------------------------------------

  virtual AttrNameVec       attributeNames() const;
  virtual VolumeSample      sample(const VolumeSampleState &state,
                                   const VolumeAttr &attribute) const;
  virtual BBox              wsBounds() const;
  virtual IntervalVec       intersect(const RayState &state) const;
  virtual Volume::StringVec info() const;

protected:

  // Utility methods -----------------------------------------------------------

  

  // Data members --------------------------------------------------------------
  
  //! Local to world transform
  Util::MatrixCurve       m_localToWorld;
  //! World to local transform
  Util::MatrixCurve       m_worldToLocal;
  //! World space bounds
  BBox                    m_wsBounds;
  //! Attribute names
  AttrNameVec             m_attrNames;
  //! Attribute scaling values
  std::vector<Imath::V3f> m_attrValues;
  //! Maximum value of all attributes. Used to compute a base 
  float                   m_maxAttrValue;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
