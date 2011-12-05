//-*-c++-*--------------------------------------------------------------------//

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

  DECLARE_SMART_PTRS(AdaptiveRaymarcher);

  // Ctor, factory -------------------------------------------------------------

  //! Factory method
  static Ptr create()
  { return Ptr(new AdaptiveRaymarcher); }

  // From ParamBase ------------------------------------------------------------

  virtual void setParams(const Util::ParamMap &params);

  // From Raymarcher -----------------------------------------------------------

  virtual std::string typeName() const
  { return "Adaptive"; }
  virtual IntegrationResult integrate(const RenderState &state) const;

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
