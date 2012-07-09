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

/*! \file Raymarcher.h
  Contains the Raymarcher class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RAYMARCHER_H__
#define __INCLUDED_PVR_RAYMARCHER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Curve.h"
#include "pvr/ParamBase.h"
#include "pvr/RenderState.h"
#include "pvr/Types.h"
#include "pvr/RaymarchSamplers/RaymarchSampler.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Structs
//----------------------------------------------------------------------------//

//! Stores the result of a single fired ray.
struct IntegrationResult
{
  IntegrationResult()
    : luminance(0.0), transmittance(1.0)
  { }
  IntegrationResult(const Color &L, const Color &T)
    : luminance(L), transmittance(T)
  { }
  IntegrationResult(const Color &L, Util::ColorCurve::CPtr Lf,
                    const Color &T, Util::ColorCurve::CPtr Tf)
    : luminance(L), transmittance(T), 
      luminanceFunction(Lf), transmittanceFunction(Tf)
  { }
  //! Ray's incoming radiance
  Color luminance;
  //! Ray's total transmittance
  Color transmittance;
  //! Luminance function defined by raymarch. May be NULL if not computed.
  Util::ColorCurve::CPtr luminanceFunction;
  //! Transmittance function defined by raymarch. May be NULL if not computed.
  Util::ColorCurve::CPtr transmittanceFunction;
};

//----------------------------------------------------------------------------//
// Raymarcher
//----------------------------------------------------------------------------//

/*! \class Raymarcher
  \brief Base class for raymarchers.

  See chapter XX for details.
 */

//----------------------------------------------------------------------------//

class Raymarcher : public Util::ParamBase
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Raymarcher);

  // Main methods --------------------------------------------------------------

  //! Sets the raymarch sampler to use during integration.
  void setRaymarchSampler(RaymarchSampler::CPtr sampler)
  { m_raymarchSampler = sampler; }

  // To be implemented by subclasses -------------------------------------------

  //! Integrates the volume along the given ray, and returns the resulting
  //! luminance and transmittance in the IntegrationResult struct.
  virtual IntegrationResult integrate(const RayState &state) const = 0;

protected:

  // Protected data members ----------------------------------------------------

  RaymarchSampler::CPtr m_raymarchSampler;

};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

//! Splits a set of possible overlapping Interval instances into a new set,
//! which is guaranteed to have no overlap (but possible continuity).
IntervalVec splitIntervals(const IntervalVec &intervals);

//! Allocates and initializes the deep luminance function based on 
//! whether the RayState has requested it.
Util::ColorCurve::Ptr setupDeepLCurve(const RayState &state, const float first);

//! Allocates and initializes the deep transmittance function based on 
//! whether the RayState has requested it.
Util::ColorCurve::Ptr setupDeepTCurve(const RayState &state, const float first);

//! Updates the deep functions (luminance and transmittance) with the
//! provided L and T values, at a depth computed from wsP.
void updateDeepFunctions(const float t, const Color &L, const Color &T, 
                         Util::ColorCurve::Ptr lf, Util::ColorCurve::Ptr tf);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
