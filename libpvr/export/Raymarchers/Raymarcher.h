//-*-c++-*--------------------------------------------------------------------//

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
  IntegrationResult(const Color &L, const Color &T, 
                    Util::TransmittanceFunction::CPtr Tf)
    : luminance(L), transmittance(T), transmittanceFunction(Tf)
  { }
  //! Ray's incoming radiance
  Color luminance;
  //! Ray's total transmittance
  Color transmittance;
  //! Transmittance function defined by raymarch. May be NULL if not computed.
  Util::TransmittanceFunction::CPtr transmittanceFunction;
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

  DECLARE_SMART_PTRS(Raymarcher);

  // Main methods --------------------------------------------------------------

  void setRaymarchSampler(RaymarchSampler::CPtr sampler)
  { m_raymarchSampler = sampler; }

  // To be implemented by subclasses -------------------------------------------

  virtual IntegrationResult integrate(const RenderState &state) const = 0;

protected:

  // Protected data members ----------------------------------------------------

  RaymarchSampler::CPtr m_raymarchSampler;

};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

IntervalVec splitIntervals(const IntervalVec &intervals);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
