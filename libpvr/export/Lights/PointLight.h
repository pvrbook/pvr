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

  PVR_TYPEDEF_SMART_PTRS(PointLight);

  // Ctor, factory -------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(PointLight);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(PointLight);

  // From Light ----------------------------------------------------------------

  virtual LightSample sample(const LightSampleState &state) const;

  // Main methods --------------------------------------------------------------

  //! Sets the position of the point light
  void                setPosition(const Vector &wsP);
  //! Returns the position of the point light
  Vector              position() const;

private:

  // Private data members ------------------------------------------------------

  //! Position of point light
  Vector m_wsP;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
