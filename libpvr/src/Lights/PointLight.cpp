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

/*! \file PointLight.cpp
  Contains implementations of PointLight class.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Lights/PointLight.h"

// System includes

// Project includes

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// PointLight implementations
//----------------------------------------------------------------------------//

LightSample PointLight::sample(const LightSampleState &state) const
{
  return LightSample(m_intensity * falloffFactor(state.wsP, m_wsP), m_wsP);
}

//----------------------------------------------------------------------------//
  
void PointLight::setPosition(const Vector &wsP)
{ 
  m_wsP = wsP; 
}

//----------------------------------------------------------------------------//

Vector PointLight::position() const
{ 
  return m_wsP; 
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
