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
