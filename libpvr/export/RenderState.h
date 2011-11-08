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
// Forward declarations
//----------------------------------------------------------------------------//

class Scene;
class Camera;

//----------------------------------------------------------------------------//
// RenderState
//----------------------------------------------------------------------------//

/*! \class RenderState
  \brief Stores information about the current state of the renderer.
 */

//----------------------------------------------------------------------------//

struct RenderState
{
  enum RayType {
    FullRaymarch,
    TransmittanceOnly, 
  };
  RenderState()
    : tMin(0.0), 
      tMax(std::numeric_limits<double>::max()),
      rayDepth(0), 
      rayType(FullRaymarch),
      time(0.0f),
      doOutputTransmittanceFunction(false)
  { }
  Ray                             wsRay;
  double                          tMin;
  double                          tMax;
  size_t                          rayDepth;
  RayType                         rayType;
  PTime                           time;
  bool                            doOutputTransmittanceFunction;
  boost::shared_ptr<const Scene>  scene;
  boost::shared_ptr<const Camera> camera;
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
  LightSampleState(const RenderState &rState)
    : renderState(rState)
  { }
  const RenderState &renderState;
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
  VolumeSampleState(const RenderState &rState)
    : renderState(rState)
  { }
  const RenderState &renderState;
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
  OcclusionSampleState(const RenderState &rState)
    : renderState(rState)
  { }
  //! Returns a RenderState that can be used to fire a secondary ray.
  RenderState makeSecondaryRayState() const
  {
    RenderState state(renderState);
    state.rayDepth++;
    state.rayType = RenderState::TransmittanceOnly;
    state.wsRay.pos = wsP;
    state.wsRay.dir = (wsLightP - wsP).normalized();
    state.tMin = 0.0;
    state.tMax = (wsLightP - wsP).length();
    return state;
  }
  const RenderState &renderState;
  Vector wsP;
  Vector wsLightP;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
