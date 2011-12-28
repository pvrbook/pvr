//----------------------------------------------------------------------------//

/*! \file DensitySampler.cpp
  Contains implementations of DensitySampler class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/RaymarchSamplers/DensitySampler.h"

// System includes

// Library includes

// Project headers

#include "pvr/RenderGlobals.h"
#include "pvr/Scene.h"
#include "pvr/Volumes/Volume.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// DensitySampler
//----------------------------------------------------------------------------//

DensitySampler::DensitySampler()
  : m_densityAttr("density")
{
  
}

//----------------------------------------------------------------------------//

DensitySampler::Ptr DensitySampler::create()
{
  return Ptr(new DensitySampler); 
}

//----------------------------------------------------------------------------//

RaymarchSample
DensitySampler::sample(const VolumeSampleState &state) const
{
  Color density = RenderGlobals::scene()->volume->sample(state, m_densityAttr);
  return RaymarchSample(density, density);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
