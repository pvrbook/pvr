//-*-c++-*--------------------------------------------------------------------//

/*! \file PointLight.h
  Contains the PointLight class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_POINTLIGHT_H__
#define __INCLUDED_PVR_POINTLIGHT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Lights/Light.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// PointLight
//----------------------------------------------------------------------------//

/*! \class PointLight
  \brief Implements a simple point light

 */

//----------------------------------------------------------------------------//

class PointLight : public Light
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(PointLight);
  static Ptr create()
  { return Ptr(new PointLight); }
  virtual std::string typeName() const
  { return "PointLight"; }
  virtual ~PointLight()
  { }
  // From Light
  virtual LightSample sample(const LightSampleState &state) const
  {
    return LightSample(m_intensity * falloff(state.wsP, m_wsP), m_wsP);
  }
  // Main methods
  void setPosition(const Vector &wsP)
  { m_wsP = wsP; }
  Vector position() const
  { return m_wsP; }
private:
  Vector m_wsP;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
