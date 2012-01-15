//----------------------------------------------------------------------------//

/*! \file PointLight.cpp
  Contains implementations of PointLight class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Lights/PointLight.h"

// System includes

// Project includes

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// PointLight implementations
//----------------------------------------------------------------------------//

LightSample PointLight::sample(const LightSampleState &state) const
{
  return LightSample(m_intensity * falloff(state.wsP, m_wsP), m_wsP);
}

//----------------------------------------------------------------------------//
  
void PointLight::setPosition(const Vector &wsP)
{ 
  m_wsP = wsP; 
}

//----------------------------------------------------------------------------//

Vector PointLight::position() const
{ 
  return m_wsP; 
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
