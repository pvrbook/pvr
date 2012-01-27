//----------------------------------------------------------------------------//

/*! \file Light.cpp
  Contains implementations of Light class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Lights/Light.h"

// System includes

// Project includes

#include "pvr/PhaseFunction.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Light implementations
//----------------------------------------------------------------------------//

Light::Light()
  : m_intensity(1.0), m_falloffEnabled(false), m_softRolloff(true),
    m_occluder(NullOccluder::create())
{ 
  // Empty
}

//----------------------------------------------------------------------------//

Light::~Light()
{ 
  // Empty
}

//----------------------------------------------------------------------------//

void Light::setIntensity(const Color &intensity)
{ 
  m_intensity = intensity / Phase::k_isotropic;
}

//----------------------------------------------------------------------------//

const Color& Light::intensity() const
{ 
  return m_intensity;
}

//----------------------------------------------------------------------------//

void Light::setFalloffEnabled(const bool enabled)
{
  m_falloffEnabled = true;
}

//----------------------------------------------------------------------------//

bool Light::falloffEnabled() const
{
  return m_falloffEnabled;
}

//----------------------------------------------------------------------------//

void Light::setOccluder(Occluder::CPtr occluder)
{ 
  assert(occluder != NULL && "Light::setOccluder got null pointer");
  m_occluder = occluder; 
}

//----------------------------------------------------------------------------//

Occluder::CPtr Light::occluder() const
{ 
  return m_occluder; 
}

//----------------------------------------------------------------------------//

float Light::falloffFactor(const Vector &p1, const Vector &p2) const
{ 
  if (m_falloffEnabled) { 
    float distanceSq = (p1 - p2).length2();
    if (m_softRolloff && distanceSq < 1.0) {
      return std::pow(1.0 / distanceSq, 0.25);
    } else {
      return 1.0 / distanceSq;
    }
  } else {
    return 1.0;
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
