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

/*! \file UniformRaymarcher.h
  Contains the UniformRaymarcher class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_UNIFORMRAYMARCHER_H__
#define __INCLUDED_PVR_UNIFORMRAYMARCHER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Raymarchers/Raymarcher.h"
#include "pvr/Volumes/Volume.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// UniformRaymarcher
//----------------------------------------------------------------------------//

/*! \class UniformRaymarcher
  \brief Implements the simplest raymarcher, with a fixed step length
 */

//----------------------------------------------------------------------------//

class UniformRaymarcher : public Raymarcher
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(UniformRaymarcher);

  // Ctor, factory -------------------------------------------------------------

  //! Default constructor
  UniformRaymarcher();
  //! Factory method
  static Ptr create()
  { return Ptr(new UniformRaymarcher); }

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(Uniform);
  virtual void setParams(const Util::ParamMap &params);

  // From Raymarcher -----------------------------------------------------------

  virtual IntegrationResult integrate(const RayState &state) const;

protected:

  // Structs -------------------------------------------------------------------

  struct Params
  {
    Params();
    //! Global step length. Used when doStepLengthFromVolume is false.
    double stepLength;
    //! Whether to use a global step length or what the scene volumes suggest
    int    useVolumeStepLength;
    //! Step length multiplier. Used when doStepLengthFromVolume is true.
    double volumeStepLengthMult;
    //! Whether to terminate rays early.
    int    doEarlyTermination;
    //! Threshold at which transparency is considered to be zero. 
    //! Used when doEarlyTermination is true.
    double earlyTerminationThreshold;
  };

  // Protected data members ----------------------------------------------------
  
  //! Holds user parameters.
  Params m_params;
  //! Holdout attribute
  VolumeAttr m_holdoutAttr;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
