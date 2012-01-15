//-*-c++-*--------------------------------------------------------------------//

/*! \file VoxelOccluder.h
  Contains the VoxelOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_VOXELOCCLUDER_H__
#define __INCLUDED_PVR_VOXELOCCLUDER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <Field3D/FieldInterp.h>

// Project headers

#include "pvr/Renderer.h"
#include "pvr/VoxelBuffer.h"
#include "pvr/Occluders/Occluder.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// VoxelOccluder
//----------------------------------------------------------------------------//

/*! \class VoxelOccluder
  \brief Determines occlusion using a transmittance map.
 */

//----------------------------------------------------------------------------//

class VoxelOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(VoxelOccluder);
  
  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  VoxelOccluder(Renderer::CPtr renderer, const Vector &wsLightPos,
                const size_t res);

  PVR_DEFINE_CREATE_FUNC_3_ARG(VoxelOccluder, Renderer::CPtr, const Vector&,
                            const size_t);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(VoxelOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Data members --------------------------------------------------------------

  DenseBuffer m_buffer;
  //! Linear interpolator
  Field3D::LinearFieldInterp<Imath::V3f> m_linearInterp;


};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
