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
  DECLARE_SMART_PTRS(DensitySampler);
  DensitySampler();
  static Ptr create();
  virtual std::string typeName() const
  { return "DensitySampler"; }
  virtual RaymarchSample sample(const VolumeSampleState &state) const;
private:
  VolumeAttr m_densityAttr;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
