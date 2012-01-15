//-*-c++-*--------------------------------------------------------------------//

/*! \file PhysicalSampler.h
  Contains the PhysicalSampler class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PHYSICALSAMPLER_H__
#define __INCLUDED_PVR_PHYSICALSAMPLER_H__

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
// PhysicalSampler
//----------------------------------------------------------------------------//

/*! \class PhysicalSampler
   \brief Looks for scattering and performs a simplified lighting calculation.
 */

//----------------------------------------------------------------------------//

class PhysicalSampler : public RaymarchSampler
{
public:
  
  // Typedefs ---

  DECLARE_SMART_PTRS(PhysicalSampler);

  // Constructor, factory ---

  PhysicalSampler();
  static Ptr create();

  // From ParamBase ---

  virtual std::string typeName() const
  { return "PhysicalSampler"; }
  
  // From RaymarchSampler ---

  virtual RaymarchSample sample(const VolumeSampleState &state) const;

private:

  // Private data members ---

  //! Used for sampling the scattering attribute
  VolumeAttr m_scatteringAttr;
  //! Used for sampling the absorption attribute
  VolumeAttr m_absorptionAttr;
  //! Used for sampling the emission attribute
  VolumeAttr m_emissionAttr;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
