//----------------------------------------------------------------------------//

/*! \file RaymarchOccluder.cpp
  Contains implementations of RaymarchOccluder class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Occluders/RaymarchOccluder.h"

// System includes

// Library includes

// Project headers

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// RaymarchOccluder
//----------------------------------------------------------------------------//

RaymarchOccluder::RaymarchOccluder(Renderer::CPtr renderer)
  : m_renderer(renderer)
{
  
}

//----------------------------------------------------------------------------//

Color RaymarchOccluder::sample(const OcclusionSampleState &state) const
{
  RayState raymarchState = state.makeSecondaryRayState();
  return m_renderer->trace(raymarchState).transmittance;
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
