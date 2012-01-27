//-*-c++-*--------------------------------------------------------------------//

/*! \file TransmittanceMapOccluder.h
  Contains the TransmittanceMapOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_TRANSMITTANCEMAPOCCLUDER_H__
#define __INCLUDED_PVR_TRANSMITTANCEMAPOCCLUDER_H__

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
// TransmittanceMapOccluder
//----------------------------------------------------------------------------//

/*! \class TransmittanceMapOccluder
  \brief Determines occlusion using a transmittance map.
 */

//----------------------------------------------------------------------------//

class TransmittanceMapOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(TransmittanceMapOccluder);
  
  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(MissingCameraException, 
                     "TransmittanceMapOccluder has no camera.");
  DECLARE_PVR_RT_EXC(MissingTransmittanceMapException, 
                     "TransmittanceMapOccluder has no transmittance map.");

  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  TransmittanceMapOccluder(Renderer::CPtr renderer, Camera::CPtr camera,
                           const size_t numSamples);

  PVR_DEFINE_CREATE_FUNC_3_ARG(TransmittanceMapOccluder, 
                               Renderer::CPtr, Camera::CPtr, const size_t);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(TransmittanceMapOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Data members --------------------------------------------------------------

  DeepImage::CPtr m_transmittanceMap;
  Camera::CPtr    m_camera;
  Imath::V2f      m_rasterBounds;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
