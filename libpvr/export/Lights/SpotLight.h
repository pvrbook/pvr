//-*-c++-*--------------------------------------------------------------------//

/*! \file SpotLight.h
  Contains the SpotLight class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_SPOTLIGHT_H__
#define __INCLUDED_PVR_SPOTLIGHT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Camera.h"
#include "pvr/Lights/Light.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// SpotLight
//----------------------------------------------------------------------------//

/*! \class SpotLight
  \brief Implements a simple spot light

 */

//----------------------------------------------------------------------------//

class SpotLight : public Light
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(SpotLight);
  DECLARE_CREATE_FUNC(SpotLight);
  virtual std::string typeName() const
  { return "SpotLight"; }
  virtual ~SpotLight()
  { }
  // From Light
  virtual LightSample sample(const LightSampleState &state) const
  {
    return LightSample(m_intensity, m_wsP);
  }
  // Main methods
  void setCamera(Camera::CPtr camera)
  { 
    m_camera = camera; 
    m_wsP = camera->position(PTime(0.0));
  }
  Camera::CPtr camera() const
  { return m_camera; }
private:
  Vector m_wsP;
  Camera::CPtr m_camera;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
