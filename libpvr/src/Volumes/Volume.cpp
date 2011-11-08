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

void setupVolumeAttr(const VolumeAttr &attribute, const std::string &name,
                     const int index) 
{
  if (attribute.name() == name) {
    attribute.setIndex(index);
  } else {
    attribute.setIndexInvalid();
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
