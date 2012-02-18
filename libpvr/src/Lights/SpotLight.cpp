//----------------------------------------------------------------------------//

/*! \file SpotLight.cpp
  Contains implementations of SpotLight class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Lights/SpotLight.h"

// System includes

// Project includes

#include "pvr/PhaseFunction.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// SpotLight implementations
//----------------------------------------------------------------------------//

SpotLight::SpotLight()
  : m_cosWidth(std::cos(M_PI * 0.5)),
    m_cosStart(std::cos(M_PI * 0.5))
{

}

//----------------------------------------------------------------------------//

LightSample SpotLight::sample(const LightSampleState &state) const
{
  Vector csP = m_camera->worldToCamera(state.wsP, state.rayState.time);
  float cosTheta = csP.normalized().z;
  float coneFalloff = 1.0;
  if (cosTheta < m_cosWidth) {
    coneFalloff = 0.0;
  } else if (cosTheta > m_cosStart) {
    coneFalloff = 1.0;
  } else {
    float delta = (cosTheta - m_cosWidth) / (m_cosStart - m_cosWidth);
    coneFalloff = delta * delta * delta * delta;
  }
  float distanceFalloff = falloffFactor(state.wsP, m_wsP);
  return LightSample(m_intensity * coneFalloff * distanceFalloff, m_wsP);
}
  
//----------------------------------------------------------------------------//

void SpotLight::setCamera(Camera::CPtr camera)
{ 
  m_camera = camera; 
  m_wsP = camera->position(PTime(0.0));
}

//----------------------------------------------------------------------------//

Camera::CPtr SpotLight::camera() const
{ 
  return m_camera; 
}

//----------------------------------------------------------------------------//

void SpotLight::setConeAngles(const float width, const float start)
{
  m_cosWidth = std::cos(width);
  m_cosStart = std::cos(start);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
