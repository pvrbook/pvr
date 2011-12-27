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
#include "pvr/TransmittanceMap.h"
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

  DECLARE_SMART_PTRS(TransmittanceMapOccluder);
  
  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(MissingCameraException, 
                     "TransmittanceMapOccluder has no camera.");
  DECLARE_PVR_RT_EXC(MissingTransmittanceMapException, 
                     "TransmittanceMapOccluder has no transmittance map.");

  // Factory method ------------------------------------------------------------

  static Ptr create();

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const;

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

  // Main methods --------------------------------------------------------------

  void setCamera(Camera::CPtr camera);
  void setTransmittanceMap(TransmittanceMap::CPtr map);

protected:

  // Data members --------------------------------------------------------------

  TransmittanceMap::CPtr m_transmittanceMap;
  Camera::CPtr           m_camera;
  Imath::V2f             m_rasterBounds;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
