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

/*! \file Particles.cpp
  Contains implementations of Particles class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Particles.h"

// System includes



//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//



  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// Particles
//----------------------------------------------------------------------------//

Particles::Particles()
{
  m_pRef = m_pointAttrs.addVectorAttr(std::string("P"), Imath::V3f(0.0f));
  // Check that attrs were created successfully
  if (!m_pRef.isValid()) {
    throw ConstructionException("Couldn't create attributes");
  }
}

//----------------------------------------------------------------------------//

void Particles::resize(const size_t size)
{ 
  m_pointAttrs.resize(size); 
}

//----------------------------------------------------------------------------//

void Particles::add(const size_t numItems)
{ 
  m_pointAttrs.addElems(numItems); 
}

//----------------------------------------------------------------------------//

size_t Particles::size() const
{
  return m_pointAttrs.size();
}

//----------------------------------------------------------------------------//

void Particles::setPosition(const size_t idx, const Imath::V3f &pos)
{
  m_pointAttrs.setVectorAttr(m_pRef, idx, pos);
}

//----------------------------------------------------------------------------//

AttrTable& Particles::pointAttrs()
{ 
  return m_pointAttrs; 
}

//----------------------------------------------------------------------------//

const AttrTable& Particles::pointAttrs() const
{ 
  return m_pointAttrs; 
}

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//
