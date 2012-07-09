//----------------------------------------------------------------------------//

/*
    This file is part of PVR. Copyright (C) 2012 Magnus Wrenninge

    PVR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PVR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
