//-*-c++-*--------------------------------------------------------------------//

/*! \file Particles.h
  Contains the Particles class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PARTICLES_H__
#define __INCLUDED_PVR_PARTICLES_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/lexical_cast.hpp>

// Project headers

#include "pvr/AttrTable.h"
#include "pvr/Exception.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// Particles
//----------------------------------------------------------------------------//

/*! \class Particles
  \brief Holds a single particle system.
  A particle system is made up of N entries of point attributes and a single
  entry of primitive attributes. A particle system always has the "P" attribute.
 */

//----------------------------------------------------------------------------//

class Particles
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Particles);

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(ConstructionException, 
                     "Failed to create Particles object:");

  // Constructor, destructor, factory ------------------------------------------

  //! Creates an empty Particles object with the "P" position attribute in place
  Particles();

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  static Ptr create();

  // Main methods --------------------------------------------------------------

  //! Resizes the particle system to the given size. Will destroy existing
  //! data but keeps attribute definitions
  void resize(const size_t size);

  //! Adds a given number of particles to the collection
  void add(const size_t numItems);

  //! Returns number of particles in particle set
  size_t size() const;

  //! Sets the position of a given point
  void setPosition(const size_t idx, const Imath::V3f &pos);

  //! Access to point data
  AttrTable& pointAttrs();

  //! Const access to point data
  const AttrTable& pointAttrs() const;

private:

  // Private data members ------------------------------------------------------

  AttrRef m_pRef;

  //! Point attributes
  AttrTable m_pointAttrs;

};

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
