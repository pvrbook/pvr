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

RaymarchSample
ScatteringSampler::sample(const VolumeSampleState &state) const
{
  const Scene          &scene = *state.renderState.scene;
  Volume::CPtr         volume = scene.volume;

  LightSampleState     lightState(state.renderState);
  OcclusionSampleState occlusionState(state.renderState);

  Color                L = Colors::zero();
  Color                scattering = volume->sample(state, m_scatteringAttr);
  
  lightState.wsP = state.wsP;

  if (state.renderState.rayType == RenderState::FullRaymarch) {
    BOOST_FOREACH (Light::CPtr light, scene.lights) {
      LightSample lightSample = light->sample(lightState);
      occlusionState.wsP = state.wsP;
      occlusionState.wsLightP = lightSample.wsP;
      if (Math::max(scattering) > 0.0f) {
        Color transmittance = light->occluder()->sample(occlusionState);
        L += scattering * lightSample.luminance * transmittance;
      }
    }
  }

  return RaymarchSample(L, scattering);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
