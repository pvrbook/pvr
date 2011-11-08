//-*-c++-*--------------------------------------------------------------------//

/*! \file NaiveRaymarcher.h
  Contains the NaiveRaymarcher class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_NAIVERAYMARCHER_H__
#define __INCLUDED_PVR_NAIVERAYMARCHER_H__

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
// NaiveRaymarcher
//----------------------------------------------------------------------------//

/*! \class NaiveRaymarcher
  \brief Implements the simplest raymarcher, with a fixed step length
 */

//----------------------------------------------------------------------------//

class NaiveRaymarcher : public Raymarcher
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(NaiveRaymarcher);

  // Ctor, factory -------------------------------------------------------------

  //! Factory method
  static Ptr create()
  { return Ptr(new NaiveRaymarcher); }

  // From ParamBase ------------------------------------------------------------

  virtual void setParams(const Util::ParamMap &params);

  // From Raymarcher -----------------------------------------------------------

  virtual std::string typeName() const
  { return "Naive"; }
  virtual IntegrationResult integrate(const RenderState &state) const;

protected:

  struct Params
  {
    Params();
    //! Global step length. Used when doStepLengthFromVolume is false.
    double stepLength;
    //! Whether to use a global step length or what the scene volumes suggest
    int useVolumeStepLength;
    //! Step length multiplier. Used when doStepLengthFromVolume is true.
    double volumeStepLengthMult;
    //! Whether to terminate rays early.
    int doEarlyTermination;
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
