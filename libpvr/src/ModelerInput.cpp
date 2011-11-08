//----------------------------------------------------------------------------//

/*! \file Modeler.cpp
  Contains implementations of Modeler class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Modeler.h"

// System includes

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {

//----------------------------------------------------------------------------//
// ModelerInput
//----------------------------------------------------------------------------//

ModelerInput::Ptr 
ModelerInput::create()
{
  return Ptr(new ModelerInput);
}

//----------------------------------------------------------------------------//

void ModelerInput::setGeometry(Geo::Geometry::Ptr geometry)
{
  m_geometry = geometry;
}
  
//----------------------------------------------------------------------------//

Geo::Geometry::Ptr ModelerInput::geometry() const
{
  return m_geometry;
}

//----------------------------------------------------------------------------//

void ModelerInput::setVolumePrimitive(Prim::Primitive::CPtr primitive)
{
  m_primitive = primitive;
}

//----------------------------------------------------------------------------//

Prim::Primitive::CPtr ModelerInput::volumePrimitive() const
{
  return m_primitive;
}
  
//----------------------------------------------------------------------------//

void ModelerInput::addFilter(Filter::CPtr filter) 
{
  m_filters.push_back(filter);
}

//----------------------------------------------------------------------------//

void ModelerInput::applyFilters()
{
  //! \todo Implement!
}

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
