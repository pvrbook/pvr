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
