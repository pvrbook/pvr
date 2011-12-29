//-*-c++-*--------------------------------------------------------------------//

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

  DECLARE_SMART_PTRS(UniformRaymarcher);

  // Ctor, factory -------------------------------------------------------------

  //! Factory method
  static Ptr create()
  { return Ptr(new UniformRaymarcher); }

  // From ParamBase ------------------------------------------------------------

  virtual void setParams(const Util::ParamMap &params);
  virtual std::string typeName() const
  { return "Uniform"; }

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
  
  //! Holds user parameters.
  Params m_params;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
