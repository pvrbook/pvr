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

  PVR_TYPEDEF_SMART_PTRS(OtfTransmittanceMapOccluder);
  
  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(MissingCameraException, 
                     "OtfTransmittanceMapOccluder has no camera.");

  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  OtfTransmittanceMapOccluder(Renderer::CPtr renderer, 
                              Camera::CPtr camera);

  PVR_DEFINE_CREATE_FUNC_2_ARG(OtfTransmittanceMapOccluder, 
                            Renderer::CPtr, Camera::CPtr);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(OtfTransmittanceMapOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Utility methods -----------------------------------------------------------

  size_t offset(const size_t x, const size_t y) const
  { return x + m_resolution.x * y; }
  void updatePixel(const size_t x, const size_t y) const;

  // Data members --------------------------------------------------------------

  bool                      m_clipBehindCamera;
  Renderer::CPtr            m_renderer;
  Camera::CPtr              m_camera;
  Imath::V2f                m_floatRasterBounds;
  Imath::V2i                m_intRasterBounds;
  Imath::V2i                m_resolution;
  mutable DeepImage         m_transmittanceMap;
  mutable std::vector<char> m_computed;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
