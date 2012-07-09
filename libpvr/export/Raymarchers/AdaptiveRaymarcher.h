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

/*! \file AdaptiveRaymarcher.h
  Contains the AdaptiveRaymarcher class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_ADAPTIVERAYMARCHER_H__
#define __INCLUDED_PVR_ADAPTIVERAYMARCHER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Raymarchers/Raymarcher.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// AdaptiveRaymarcher
//----------------------------------------------------------------------------//

/*! \class AdaptiveRaymarcher
  \brief Implements an adaptive raymarch scheme, where the extinction 
  coefficient controls the step length.
 */

//----------------------------------------------------------------------------//

class AdaptiveRaymarcher : public Raymarcher
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(AdaptiveRaymarcher);

  // Ctor, factory -------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(AdaptiveRaymarcher);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(Adaptive);
  virtual void setParams(const Util::ParamMap &params);

  // From Raymarcher -----------------------------------------------------------

  virtual IntegrationResult integrate(const RayState &state) const;

protected:

  struct Params
  {
    Params();
    //! Error threshold. Controls the sensitivity of the adaptive algorithm
    double threshold;
    //! Step length multiplier. Upper bound on step length.
    double volumeStepLengthMult;
    //! Whether to use trapezoid integration
    int doTrapezoidIntegration;
    //! Threshold at which transparency is considered to be zero. 
    //! Used when doEarlyTermination is true.
    double earlyTerminationThreshold;
  };

  // Protected data members ----------------------------------------------------
  
  Params m_params;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
