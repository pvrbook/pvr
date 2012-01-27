//-*-c++-*--------------------------------------------------------------------//

/*! \file DensitySampler.h
  Contains the DensitySampler class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_DENSITYSAMPLER_H__
#define __INCLUDED_PVR_DENSITYSAMPLER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/RaymarchSamplers/RaymarchSampler.h"
#include "pvr/VolumeAttr.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// DensitySampler
//----------------------------------------------------------------------------//

/*! \class DensitySampler
   \brief Interprets density as both luminance and attenuation.
 */

//----------------------------------------------------------------------------//

class DensitySampler : public RaymarchSampler
{
public:
  // Typedefs ---
  PVR_TYPEDEF_SMART_PTRS(DensitySampler);
  // Ctor, factory ---
  DensitySampler();
  PVR_DEFINE_CREATE_FUNC(DensitySampler);
  // From ParamBase ---
  PVR_DEFINE_TYPENAME(DensitySampler);
  // From RaymarchSampler ---
  virtual RaymarchSample sample(const VolumeSampleState &state) const;
private:
  // Private data members ---
  VolumeAttr m_densityAttr;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
