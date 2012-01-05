//-*-c++-*--------------------------------------------------------------------//

/*! \file OtfTransmittanceMapOccluder.h
  Contains the OtfTransmittanceMapOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_OTFTRANSMITTANCEMAPOCCLUDER_H__
#define __INCLUDED_PVR_OTFTRANSMITTANCEMAPOCCLUDER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Camera.h"
#include "pvr/DeepImage.h"
#include "pvr/Renderer.h"
#include "pvr/Occluders/Occluder.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// OtfTransmittanceMapOccluder
//----------------------------------------------------------------------------//

/*! \class OtfTransmittanceMapOccluder
  \brief Determines occlusion using a transmittance map generated on the fly.
 */

//----------------------------------------------------------------------------//

class OtfTransmittanceMapOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(OtfTransmittanceMapOccluder);
  
  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(MissingCameraException, 
                     "OtfTransmittanceMapOccluder has no camera.");

  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  OtfTransmittanceMapOccluder(Renderer::CPtr renderer, 
                              Camera::CPtr camera);

  DECLARE_CREATE_FUNC_2_ARG(OtfTransmittanceMapOccluder, 
                            Renderer::CPtr, Camera::CPtr);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const;

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Utility methods -----------------------------------------------------------

  size_t offset(const size_t x, const size_t y) const
  { return x + m_rasterBounds.x * y; }
  void updatePixel(const size_t x, const size_t y) const;

  // Data members --------------------------------------------------------------

  Renderer::CPtr            m_renderer;
  Camera::CPtr              m_camera;
  Imath::V2f                m_rasterBounds;
  mutable DeepImage         m_transmittanceMap;
  mutable std::vector<char> m_computed;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
