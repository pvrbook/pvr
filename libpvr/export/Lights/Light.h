//-*-c++-*--------------------------------------------------------------------//

/*! \file Light.h
  Contains the Light class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_LIGHT_H__
#define __INCLUDED_PVR_LIGHT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Constants.h"
#include "pvr/ParamBase.h"
#include "pvr/RenderState.h"
#include "pvr/Occluders/Occluder.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// LightSample
//----------------------------------------------------------------------------//

struct LightSample
{
  LightSample()
    : luminance(Colors::zero()), wsP(Vectors::zero())
  { }
  LightSample(const Color &L, const Vector &wsPos)
    : luminance(L), wsP(wsPos)
  { }
  //! \todo should be radiance?
  Color  luminance;
  //! Position of light source
  Vector wsP;
};

//----------------------------------------------------------------------------//
// Light
//----------------------------------------------------------------------------//

/*! \class Light
  \brief Base class for all lights.
  \todo Put implementations in .cpp file
 */

//----------------------------------------------------------------------------//

class Light : public Util::ParamBase
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Light);

  // Constructor, destructor ---------------------------------------------------

  Light()
    : m_occluder(NullOccluder::create())
  { }
  virtual ~Light()
  { }

  // To be implemented by subclasses -------------------------------------------

  virtual LightSample sample(const LightSampleState &state) const = 0;

  // Main methods --------------------------------------------------------------

  void setIntensity(const Color &intensity)
  { m_intensity = intensity; }
  void setOccluder(Occluder::CPtr occluder)
  { 
    assert(occluder != NULL && "Light::setOccluder got null pointer");
    m_occluder = occluder; 
  }
  Occluder::CPtr occluder() const
  { return m_occluder; }

protected:

  // Data members --------------------------------------------------------------

  Color m_intensity;
  Occluder::CPtr m_occluder;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
