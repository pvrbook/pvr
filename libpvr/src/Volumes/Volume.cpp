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
