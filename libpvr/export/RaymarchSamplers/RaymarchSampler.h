//-*-c++-*--------------------------------------------------------------------//

/*! \file RaymarchSampler.h
  Contains the RaymarchSampler class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RAYMARCHSAMPLER_H__
#define __INCLUDED_PVR_RAYMARCHSAMPLER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Constants.h"
#include "pvr/ParamBase.h"
#include "pvr/RenderState.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Structs
//----------------------------------------------------------------------------//

//! \brief Stores the result of a RaymarchSampler's sample call (luminance and
//! attenuation).
struct RaymarchSample
{
  RaymarchSample()
    : luminance(Colors::zero()), attenuation(Colors::zero())
  { }
  RaymarchSample(const Color &L, const Color &A)
    : luminance(L), attenuation(A)
  { }
  //! Radiance at the sample's point in space. This should not be scaled
  //! by the step length.
  Color luminance;
  //! Attenuation coefficient for the sample's point in space. This should
  //! not be scaled by the step length
  Color attenuation;
};

//----------------------------------------------------------------------------//
// RaymarchSampler
//----------------------------------------------------------------------------//

/*! \class RaymarchSampler
   \brief Base class for raymarch samplers. 

   These classes perform the task of sampling arbitrary attributes in the 
   scene, and turning them into Luminance and Attenuation, which the 
   Raymarcher can understand.
 */

//----------------------------------------------------------------------------//

class RaymarchSampler : public Util::ParamBase
{
public:
  DECLARE_SMART_PTRS(RaymarchSampler);
  virtual RaymarchSample sample(const VolumeSampleState &state) const = 0;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
