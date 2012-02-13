//-*-c++-*--------------------------------------------------------------------//

/*! \file RaymarchOccluder.h
  Contains the RaymarchOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RAYMARCHOCCLUDER_H__
#define __INCLUDED_PVR_RAYMARCHOCCLUDER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Occluders/Occluder.h"
#include "pvr/Renderer.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// RaymarchOccluder
//----------------------------------------------------------------------------//

/*! \class RaymarchOccluder
  \brief Calculates occlusion by raymarching
 */

//----------------------------------------------------------------------------//

class RaymarchOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(RaymarchOccluder);

  // Factory methods -----------------------------------------------------------

  //! Default constructor
  RaymarchOccluder(Renderer::CPtr renderer);
  PVR_DEFINE_CREATE_FUNC_1_ARG(RaymarchOccluder, Renderer::CPtr)

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(RaymarchOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Data members --------------------------------------------------------------

  Renderer::CPtr m_renderer;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
