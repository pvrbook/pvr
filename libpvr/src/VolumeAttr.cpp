//----------------------------------------------------------------------------//

/*! \file VolumeAttr.cpp
  Contains implementations of VolumeAttr class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/VolumeAttr.h"

// Project includes



//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// VolumeAttr implementations
//----------------------------------------------------------------------------//

VolumeAttr::VolumeAttr(const std::string &name)
  : m_name(name), m_index(IndexNotSet)
{ 
  // Empty
}

//----------------------------------------------------------------------------//

const std::string& VolumeAttr::name() const
{ 
  return m_name; 
}
  
//----------------------------------------------------------------------------//

int VolumeAttr::index() const
{ 
  return m_index; 
}

//----------------------------------------------------------------------------//

void VolumeAttr::setIndex(const int index) const
{ 
  if (m_index != IndexNotSet) {
    throw SetIndexException("Trying to set index more than once");
  }
  if (index < 0) {
    throw SetIndexException("Index provided was less than zero");
  }
  m_index = index; 
}

//----------------------------------------------------------------------------//

void VolumeAttr::setIndexInvalid() const
{ 
  m_index = IndexInvalid; 
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
