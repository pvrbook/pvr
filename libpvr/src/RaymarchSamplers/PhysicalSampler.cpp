//----------------------------------------------------------------------------//

/*! \file PhysicalSampler.cpp
  Contains implementations of PhysicalSampler class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/RaymarchSamplers/PhysicalSampler.h"

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
// PhysicalSampler
//----------------------------------------------------------------------------//

PhysicalSampler::PhysicalSampler()
  : m_scatteringAttr("scattering"), 
    m_absorptionAttr("absorption"),
    m_emissionAttr("emission")
{

}

//----------------------------------------------------------------------------//

RaymarchSample PhysicalSampler::sample(const VolumeSampleState &state) const
{
  const Scene::CPtr    scene          = RenderGlobals::scene();
  const Volume::CPtr   volume         = scene->volume;

  LightSampleState     lightState     (state.rayState);
  OcclusionSampleState occlusionState (state.rayState);

  const Vector         wo             = -state.rayState.wsRay.dir;

  VolumeSample         abSample       = volume->sample(state, m_absorptionAttr);
  VolumeSample         emSample       = volume->sample(state, m_emissionAttr);
  VolumeSample         scSample       = volume->sample(state, m_scatteringAttr);

  const Color &        sigma_s        = scSample.value;
  const Color &        sigma_a        = abSample.value;
  const Color &        L_em           = emSample.value;

  // Only perform calculation if ray is primary and scattering coefficient is
  // greater than zero.

  Color                L_sc           = Colors::zero();

  if (Math::max(sigma_s) > 0.0f &&
      state.rayState.rayType == RayState::FullRaymarch) {

    // Update light and occluder sample states
    lightState.wsP     = state.wsP;
    occlusionState.wsP = state.wsP;

    // For each light source
    BOOST_FOREACH (Light::CPtr light, scene->lights) {

      // Sample the light
      LightSample lightSample = light->sample(lightState);

      // Sample the occluder
      occlusionState.wsLightP = lightSample.wsP;
      Color transmittance     = light->occluder()->sample(occlusionState);

      // Find the scattering probability
      const Vector wi = (state.wsP - lightSample.wsP).normalized();
      const float  p  = scSample.phaseFunction->probability(wi, wo);

      // Update luminance
      L_sc += sigma_s * p * lightSample.luminance * transmittance;

    }
  }

  return RaymarchSample(L_sc + L_em, sigma_s + sigma_a);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
