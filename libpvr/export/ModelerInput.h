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

  DECLARE_SMART_PTRS(ModelerInput);

  // Constructor, destructor, factory ------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  static Ptr create();

  // Main methods --------------------------------------------------------------

  //! Sets the geometry data that the input should use
  void setGeometry(Geo::Geometry::Ptr geometry);
  //! Returns a pointer to the geometry contained by the ModelerInput.
  Geo::Geometry::Ptr geometry() const;
  //! Sets the volumetric primitive to use for generating data
  void setVolumePrimitive(Prim::Primitive::CPtr primitive);
  //! Returns the volumetric primitive currently assigned.
  Prim::Primitive::CPtr volumePrimitive() const;
  //! Adds the given Filter to the chain of Filters that gets called after
  //! each new set of ModelerInput is created by InstantiationPrims.
  void addFilter(Filter::CPtr filter);
  //! Executes the Filters
  void applyFilters();

protected:
  
  // Protected data members ----------------------------------------------------

  Geo::Geometry::Ptr        m_geometry;
  Prim::Primitive::CPtr     m_primitive;
  std::vector<Filter::CPtr> m_filters;

};

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
