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

RaymarchOccluder::Ptr RaymarchOccluder::create()
{ 
  return Ptr(new RaymarchOccluder); 
}

//----------------------------------------------------------------------------//

std::string RaymarchOccluder::typeName() const
{ 
  return "RaymarchOccluder"; 
}

//----------------------------------------------------------------------------//

Color RaymarchOccluder::transmittance(const OcclusionSampleState &state) const
{
  RenderState raymarchState = state.makeSecondaryRayState();
  return m_raymarcher->integrate(raymarchState).transmittance;
}

//----------------------------------------------------------------------------//

void RaymarchOccluder::setRaymarcher(Raymarcher::CPtr raymarcher)
{ 
  m_raymarcher = raymarcher; 
}


//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
