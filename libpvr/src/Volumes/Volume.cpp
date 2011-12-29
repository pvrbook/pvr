//----------------------------------------------------------------------------//

/*! \file Volume.cpp
  Contains implementations of volume utility functions
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Volumes/Volume.h"

// System includes

// Library includes

// Project headers

#include "pvr/Strings.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//

void Volume::setPhaseFunction(Phase::PhaseFunction::CPtr phaseFunction)
{
  assert(phaseFunction != NULL && "Got null pointer for phase function");
  m_phaseFunction = phaseFunction;
}

//----------------------------------------------------------------------------//

Volume::StringVec Volume::info() const
{
  return StringVec();
}

//----------------------------------------------------------------------------//

Volume::CVec Volume::inputs() const
{
  return Volume::CVec();
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
