//-*-c++-*--------------------------------------------------------------------//

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
  LightSampleState(const RayState &rState)
    : rayState(rState)
  { }
  const RayState &rayState;
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
