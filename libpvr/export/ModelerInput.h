//-*-c++-*--------------------------------------------------------------------//

/*! \file ModelerInput.h
  Contains the ModelerInput class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_MODELERINPUT_H__
#define __INCLUDED_PVR_MODELERINPUT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <boost/foreach.hpp>

// Project headers

#include "pvr/Geometry.h"
#include "pvr/ParamBase.h"
#include "pvr/Types.h"
#include "pvr/Primitives/Primitive.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {

//----------------------------------------------------------------------------//
// ModelerInput
//----------------------------------------------------------------------------//

/*! \class ModelerInput
  \brief Holds the data structures that define a single input to the Modeler
  class.
 */

//----------------------------------------------------------------------------//

class ModelerInput
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(ModelerInput);

  // Constructor, destructor, factory ------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC(ModelerInput);

  // Main methods --------------------------------------------------------------

  //! Sets the geometry data that the input should use
  void                  setGeometry(Geo::Geometry::CPtr geometry);
  //! Returns a pointer to the geometry contained by the ModelerInput.
  Geo::Geometry::CPtr   geometry() const;
  //! Sets the volumetric primitive to use for generating data
  void                  setVolumePrimitive(Prim::Primitive::CPtr primitive);
  //! Returns the volumetric primitive currently assigned.
  Prim::Primitive::CPtr volumePrimitive() const;

protected:
  
  // Protected data members ----------------------------------------------------

  Geo::Geometry::CPtr   m_geometry;
  Prim::Primitive::CPtr m_primitive;

};

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
