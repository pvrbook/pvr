//----------------------------------------------------------------------------//

/*
    This file is part of PVR. Copyright (C) 2012 Magnus Wrenninge

    PVR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PVR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
