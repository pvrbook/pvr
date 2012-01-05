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
  //! Default constructor
  LightSample()
    : luminance(Colors::zero()), wsP(Vectors::zero())
  { }
  //! Construct from luminance and position values
  LightSample(const Color &L, const Vector &wsPos)
    : luminance(L), wsP(wsPos)
  { }
  //! Luminance arriving at sampled location
  Color  luminance;
  //! Position of light source that was sampled
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

  Light();
  virtual ~Light();

  // To be implemented by subclasses -------------------------------------------

  //! Samples the given light.
  //! \note This should compute the luminance arriving at a given position
  //! withouth taking into consideration occlusion.
  virtual LightSample sample(const LightSampleState &state) const = 0;

  // Main methods --------------------------------------------------------------

  //! Sets the intesity of the light source.
  void setIntensity(const Color &intensity);
  //! Returns the intensity
  const Color& intensity() const;
  //! Sets the Occluder to use for the light. By default, each light has the
  //! NullOccluder assigned.
  void setOccluder(Occluder::CPtr occluder);
  //! Returns the light's Occluder.
  Occluder::CPtr occluder() const;

protected:

  // Data members --------------------------------------------------------------

  //! Intensity of light source
  Color m_intensity;
  //! Pointer to the light's Occluder
  Occluder::CPtr m_occluder;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
