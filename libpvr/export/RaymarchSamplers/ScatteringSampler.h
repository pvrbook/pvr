//-*-c++-*--------------------------------------------------------------------//

/*! \file ScatteringSampler.h
  Contains the ScatteringSampler class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_SCATTERINGSAMPLER_H__
#define __INCLUDED_PVR_SCATTERINGSAMPLER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/RenderState.h"
#include "pvr/Types.h"
#include "pvr/VolumeAttr.h"
#include "pvr/RaymarchSamplers/RaymarchSampler.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// ScatteringSampler
//----------------------------------------------------------------------------//

/*! \class ScatteringSampler
   \brief Looks for scattering and performs a simplified lighting calculation.
 */

//----------------------------------------------------------------------------//

class ScatteringSampler : public RaymarchSampler
{
public:
  
  // Typedefs ---

  DECLARE_SMART_PTRS(ScatteringSampler);

  // Constructor, factory ---

  ScatteringSampler();
  static Ptr create();

  // From ParamBase ---

  virtual std::string typeName() const
  { return "ScatteringSampler"; }
  
  // From RaymarchSampler ---

  virtual RaymarchSample sample(const VolumeSampleState &state) const;

private:

  // Private data members ---

  VolumeAttr m_scatteringAttr;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
