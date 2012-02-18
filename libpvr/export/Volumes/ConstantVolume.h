//-*-c++-*--------------------------------------------------------------------//

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
