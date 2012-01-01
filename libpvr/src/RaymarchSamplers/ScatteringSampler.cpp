//----------------------------------------------------------------------------//

/*! \file ScatteringSampler.cpp
  Contains implementations of ScatteringSampler class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/RaymarchSamplers/ScatteringSampler.h"

// System includes

// Library includes

#include <boost/foreach.hpp>

// Project headers

#include "pvr/Math.h"
#include "pvr/Scene.h"
#include "pvr/Volumes/Volume.h"
#include "pvr/Constants.h"

#include "pvr/Lights/Light.h"
#include "pvr/RenderGlobals.h"

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
// ScatteringSampler
//----------------------------------------------------------------------------//

ScatteringSampler::ScatteringSampler()
  : m_scatteringAttr("scattering")
{

}

//----------------------------------------------------------------------------//

ScatteringSampler::Ptr ScatteringSampler::create()
{
  return Ptr(new ScatteringSampler); 
}

//----------------------------------------------------------------------------//

//! \todo This is messy. Tidy up.
RaymarchSample
ScatteringSampler::sample(const VolumeSampleState &state) const
{
  const Scene          &scene = *RenderGlobals::scene();
  Volume::CPtr         volume = scene.volume;

  LightSampleState     lightState(state.rayState);
  OcclusionSampleState occlusionState(state.rayState);

  Color                L = Colors::zero();
  VolumeSample         sample = volume->sample(state, m_scatteringAttr);
  const Color &        sigma_s = sample.value;
  const Vector         wo = -state.rayState.wsRay.dir;
  
  lightState.wsP = state.wsP;

  // Only perform calculation if ray is primary and scattering coefficient is
  // greater than zero.
  if (Math::max(sigma_s) > 0.0f &&
      state.rayState.rayType == RayState::FullRaymarch) {
    // For each light source
    BOOST_FOREACH (Light::CPtr light, scene.lights) {
      // Sample the light
      LightSample lightSample = light->sample(lightState);
      occlusionState.wsP = state.wsP;
      occlusionState.wsLightP = lightSample.wsP;
      // Sample the occluder
      Color transmittance = light->occluder()->sample(occlusionState);
      // Find the scattering probability
      const Vector wi = (state.wsP - lightSample.wsP).normalized();
      const float p = sample.phaseFunction->probability(wi, wo);
      // Update luminance
      L += sigma_s * p * lightSample.luminance * transmittance;
    }
  }

  return RaymarchSample(L, sigma_s);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
