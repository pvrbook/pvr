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

/*! \file Light.h
  Contains the Light class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_LIGHT_H__
#define __INCLUDED_PVR_LIGHT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Constants.h"
#include "pvr/ParamBase.h"
#include "pvr/RenderState.h"
#include "pvr/Occluders/Occluder.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// LightSample
//----------------------------------------------------------------------------//

struct LightSample
{
  //! Default constructor
  LightSample()
    : luminance(Colors::zero()), wsP(Vectors::zero())
  { }
  //! Construct from luminance and position values
  LightSample(const Color &L, const Vector &wsPos)
    : luminance(L), wsP(wsPos)
  { }
  //! Luminance arriving at sampled location
  Color  luminance;
  //! Position of light source that was sampled
  Vector wsP;
};

//----------------------------------------------------------------------------//
// Light
//----------------------------------------------------------------------------//

/*! \class Light
  \brief Base class for all lights.
 */

//----------------------------------------------------------------------------//

class Light : public Util::ParamBase
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Light);

  // Constructor, destructor ---------------------------------------------------

  //! Default constructor
  Light();
  //! Virtual destructor
  virtual ~Light();

  // To be implemented by subclasses -------------------------------------------

  //! Samples the given light.
  //! \note This should compute the luminance arriving at a given position
  //! withouth taking into consideration occlusion.
  virtual LightSample sample(const LightSampleState &state) const = 0;

  // Main methods --------------------------------------------------------------

  //! Sets the intesity of the light source.
  void                setIntensity(const Color &intensity);
  //! Returns the intensity
  const Color&        intensity() const;
  //! Sets whether falloff is enabled
  void                setFalloffEnabled(const bool enabled);
  //! Returns whether falloff is enabled
  bool                falloffEnabled() const;
  //! Sets the Occluder to use for the light. By default, each light has the
  //! NullOccluder assigned.
  void                setOccluder(Occluder::CPtr occluder);
  //! Returns the light's Occluder.
  Occluder::CPtr      occluder() const;

protected:

  // Utility methods -----------------------------------------------------------

  //! Computes falloff given a distance
  float               falloffFactor(const Vector &p1, const Vector &p2) const;
  
  // Data members --------------------------------------------------------------

  //! Intensity of light source
  Color          m_intensity;
  //! Whether falloff is enabled
  bool           m_falloffEnabled;
  //! Whether soft rolloff is used close to the light source
  bool           m_softRolloff;
  //! Pointer to the light's Occluder
  Occluder::CPtr m_occluder;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
