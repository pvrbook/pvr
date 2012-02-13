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

void ModelerInput::setGeometry(Geo::Geometry::CPtr geometry)
{
  m_geometry = geometry;
}
  
//----------------------------------------------------------------------------//

Geo::Geometry::CPtr ModelerInput::geometry() const
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

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
