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
#include "pvr/Raymarchers/Raymarcher.h"

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

  DECLARE_SMART_PTRS(RaymarchOccluder);

  // Factory methods -----------------------------------------------------------

  static Ptr create();

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const;

  // From Occluder -------------------------------------------------------------

  virtual Color transmittance(const OcclusionSampleState &state) const;

  // Main methods --------------------------------------------------------------

  void setRaymarcher(Raymarcher::CPtr raymarcher);

protected:

  // Data members --------------------------------------------------------------

  Raymarcher::CPtr m_raymarcher;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
