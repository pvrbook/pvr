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
#include "pvr/PhaseFunction.h"
#include "pvr/RenderState.h"
#include "pvr/VolumeAttr.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// VolumeSample
//----------------------------------------------------------------------------//

struct VolumeSample
{
  
  // Constructors ---

  VolumeSample(const Color &v, Phase::PhaseFunction::CPtr p)
    : value(v), phaseFunction(p)
  { }

  // Public data members ---

  Color value;
  Phase::PhaseFunction::CPtr phaseFunction;

};

//----------------------------------------------------------------------------//
// Volume
//----------------------------------------------------------------------------//

//! Base class for volumes that are used in the rendering/raymarching.
class Volume : public Util::ParamBase
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Volume);
  typedef std::vector<std::string>  AttrNameVec;
  typedef std::vector<std::string>  StringVec;
  typedef std::vector<Volume::CPtr> CVec;

  // Constructors and destructor -----------------------------------------------

  //! Default constructor. Initializes the scattering attribute
  Volume()
    : m_phaseFunction(new Phase::Isotropic)
  { }
  //! Virtual destructor
  ~Volume()
  { }
  
  // Main methods --------------------------------------------------------------

  //! Sets the phase function to use
  void setPhaseFunction(Phase::PhaseFunction::CPtr phaseFunction);
  //! Returns the phase function
  Phase::PhaseFunction::CPtr phaseFunction() const;

  // To be implemented by subclasses -------------------------------------------

  //! Returns the names of the attributes that the volume provides.
  virtual AttrNameVec attributeNames() const = 0;
  //! Returns the value of the volume node for a given point.
  //! \note The point position is found in state.wsP
  virtual VolumeSample sample(const VolumeSampleState &state,
                              const VolumeAttr &attribute) const = 0;
  //! Returns an axis-aligned world space bounding box.
  virtual BBox wsBounds() const = 0;
  //! Returns a vector of intersection intervals (start/end points) that
  //! intersect the ray.
  //! \note The ray is found in state.wsRay
  virtual IntervalVec intersect(const RayState &state) const = 0;

  // Optionally implemented by subclasses --------------------------------------

  //! Returns string-formatted information about the volume
  virtual StringVec info() const;
  //! Returns a vector of other volumes that the volume references
  virtual CVec inputs() const;

protected:

  // Utility methods -----------------------------------------------------------

  float computeProbability(const Vector &in, const Vector &out,
                           const VolumeAttr &attribute) const;

  // Data members --------------------------------------------------------------
  
  //! Pointer to phase function
  Phase::PhaseFunction::CPtr m_phaseFunction;

};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

void setIndexForName(const VolumeAttr &attr, const Volume::StringVec &v);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
