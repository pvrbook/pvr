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
// Volume
//----------------------------------------------------------------------------//

Volume::Volume()
  : m_phaseFunction(new Phase::Isotropic)
{ 

}

//----------------------------------------------------------------------------//


Volume::~Volume()
{ 

}

//----------------------------------------------------------------------------//

void Volume::setPhaseFunction(Phase::PhaseFunction::CPtr phaseFunction)
{
  assert(phaseFunction != NULL && "Got null pointer for phase function");
  if (phaseFunction) {
    m_phaseFunction = phaseFunction;
  }
}

//----------------------------------------------------------------------------//

Phase::PhaseFunction::CPtr Volume::phaseFunction() const
{
  return m_phaseFunction;
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
// Utility functions
//----------------------------------------------------------------------------//

void setIndexForName(const VolumeAttr &attr, const Volume::StringVec &v)
{
  Volume::AttrNameVec::const_iterator i = 
    std::find(v.begin(), v.end(), attr.name());
  if (i != v.end()) {
    attr.setIndex(std::distance(i, v.begin()));
  } else {
    attr.setIndexInvalid();
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
