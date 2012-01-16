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

  PVR_TYPEDEF_SMART_PTRS(SpotLight);

  // Ctor, factory -------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(SpotLight);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(SpotLight);

  // From Light ----------------------------------------------------------------

  virtual LightSample sample(const LightSampleState &state) const;

  // Main methods --------------------------------------------------------------

  //! Sets the camera to use for projection
  void setCamera(Camera::CPtr camera);
  //! Returns the camera used for projection
  Camera::CPtr camera() const;

private:

  // Private data members ------------------------------------------------------

  //! Position of spot light
  Vector m_wsP;
  //! Camera to use for projection
  Camera::CPtr m_camera;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
