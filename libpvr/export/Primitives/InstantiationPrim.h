//-*-c++-*--------------------------------------------------------------------//

/*! \file InstantiationPrimitive.h
  Contains the InstantiationPrimitive class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INSTANTIATIONPRIMITIVE_H__
#define __INCLUDED_PVR_INSTANTIATIONPRIMITIVE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers



// Project headers

#include "pvr/ModelerInput.h"
#include "pvr/Primitives/Primitive.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Inst {

//----------------------------------------------------------------------------//
// InstantiationPrim
//----------------------------------------------------------------------------//

/*! \class InstantiationPrim
  \brief Base class for instantiation-based primitives.

  Instantiation-based primitives are covered in chapter XX.
 */

//----------------------------------------------------------------------------//

class InstantiationPrim : public Primitive
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(InstantiationPrim);

  // Main methods --------------------------------------------------------------

  //! Executes the instantiation primitive
  virtual ModelerInput::Ptr execute(const Geo::Geometry::CPtr geo) const = 0;

};

//----------------------------------------------------------------------------//

} // namespace Inst
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
