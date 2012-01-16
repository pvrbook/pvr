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

LightSample SpotLight::sample(const LightSampleState &state) const
{
  return LightSample(m_intensity, m_wsP);
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

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
