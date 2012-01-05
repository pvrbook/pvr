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
  : m_occluder(NullOccluder::create())
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

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
