//-*-c++-*--------------------------------------------------------------------//

/*! \file FractalCloud.h
  Contains the FractalCloud class
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_FRACTALCLOUD_H__
#define __INCLUDED_PVR_FRACTALCLOUD_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

// Project headers

#include "pvr/Volumes/Volume.h"
#include "pvr/Noise/Noise.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// FractalCloud
//----------------------------------------------------------------------------//

/*! \class FractalCloud
  \brief A simple spherical puff of cloud based on a Fractal instance
 */

//----------------------------------------------------------------------------//

class FractalCloud : public Volume
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(FractalCloud);

  // Ctor, factory -------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(FractalCloud);
  FractalCloud()
    : m_density(1.0f), m_stepLength(1.0)
  { }

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(FractalCloud);

  // From Volume ---------------------------------------------------------------

  virtual AttrNameVec attributeNames() const;
  virtual VolumeSample sample(const VolumeSampleState &state,
                              const VolumeAttr &attribute) const;
  virtual BBox wsBounds() const { return BBox(); }
  virtual IntervalVec intersect(const RayState &state) const;

  // Main methods --------------------------------------------------------------

  void setFractal(const Noise::Fractal::CPtr fractal)
  { m_fractal = fractal; }
  void setDensity(const float density)
  { m_density = density; }
  //! \todo Replace this with a calculation of smallest noise feature
  void setStepLength(const float stepLength)
  { m_stepLength = stepLength; }

protected:

  // Utility methods -----------------------------------------------------------

  // Protected data members ----------------------------------------------------

  //! Fractal function
  Noise::Fractal::CPtr m_fractal;

  //! Overall density
  float m_density;
  //! Step length
  double m_stepLength;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
