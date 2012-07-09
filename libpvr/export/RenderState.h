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

/*! \file RenderState.h
  Contains the RenderState class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RENDERSTATE_H__
#define __INCLUDED_PVR_RENDERSTATE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Time.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// RayState
//----------------------------------------------------------------------------//

/*! \class RayState
  \brief Stores information about the current state of the renderer.
 */

//----------------------------------------------------------------------------//

struct RayState
{
  enum RayType {
    FullRaymarch,
    TransmittanceOnly, 
  };
  RayState()
    : tMin(0.0), 
      tMax(std::numeric_limits<double>::max()),
      rayDepth(0), 
      rayType(FullRaymarch),
      time(0.0f),
      doOutputDeepL(false),
      doOutputDeepT(false)
  { }
  Ray     wsRay;
  double  tMin;
  double  tMax;
  size_t  rayDepth;
  RayType rayType;
  PTime   time;
  bool    doOutputDeepL;
  bool    doOutputDeepT;
};

//----------------------------------------------------------------------------//
// LightSampleState
//----------------------------------------------------------------------------//

/*! \class LightSampleState
  \brief Stores the information needed to evaluate a Light at a given point.
 */

//----------------------------------------------------------------------------//

struct LightSampleState
{
  //! Constructor, requires an existing RayState to be passed in
  LightSampleState(const RayState &rState)
    : rayState(rState)
  { }
  //! Const reference to ray state. Used so that each Light can access
  //! sample time, etc.
  const RayState &rayState;
  //! Point in world space that is being sampled.
  Vector wsP;
};

//----------------------------------------------------------------------------//
// VolumeSampleState
//----------------------------------------------------------------------------//

/*! \class VolumeSampleState
  \brief Stores the information needed to evaluate a Volume at a given point.
 */

//----------------------------------------------------------------------------//

struct VolumeSampleState
{
  VolumeSampleState(const RayState &rState)
    : rayState(rState)
  { }
  const RayState &rayState;
  Vector wsP;
};

//----------------------------------------------------------------------------//
// OcclusionSampleState
//----------------------------------------------------------------------------//

/*! \class OcclusionSampleState
  \brief Stores the information needed to evaluate the transmittance between
  two points.
 */

//----------------------------------------------------------------------------//

struct OcclusionSampleState
{
  OcclusionSampleState(const RayState &rState)
    : rayState(rState)
  { }
  //! Returns a RayState that can be used to fire a secondary ray.
  RayState makeSecondaryRayState() const
  {
    RayState state(rayState);
    state.rayDepth++;
    state.rayType = RayState::TransmittanceOnly;
    state.wsRay.pos = wsP;
    state.wsRay.dir = (wsLightP - wsP).normalized();
    state.tMin = 0.0;
    state.tMax = (wsLightP - wsP).length();
    return state;
  }
  const RayState &rayState;
  Vector wsP;
  Vector wsLightP;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
