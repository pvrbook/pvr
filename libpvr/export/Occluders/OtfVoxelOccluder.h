//-*-c++-*--------------------------------------------------------------------//

/*! \file OtfVoxelOccluder.h
  Contains the OtfVoxelOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_OTFVOXELOCCLUDER_H__
#define __INCLUDED_PVR_OTFVOXELOCCLUDER_H__

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
// OtfVoxelOccluder
//----------------------------------------------------------------------------//

/*! \class OtfVoxelOccluder
  \brief Determines occlusion using a transmittance map.
 */

//----------------------------------------------------------------------------//

class OtfVoxelOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(OtfVoxelOccluder);
  
  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  OtfVoxelOccluder(Renderer::CPtr renderer, const Vector &wsLightPos,
                const size_t res);

  DECLARE_CREATE_FUNC_3_ARG(OtfVoxelOccluder, Renderer::CPtr, const Vector&,
                            const size_t);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const;

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Utility methods -----------------------------------------------------------

  void updateVoxel(const int i, const int j, const int k) const;

  // Data members --------------------------------------------------------------

  Renderer::CPtr m_renderer;
  const Vector m_wsLightPos;
  mutable DenseBuffer m_buffer;
  //! Linear interpolator
  Field3D::LinearFieldInterp<Imath::V3f> m_linearInterp;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
