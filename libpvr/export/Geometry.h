//-*-c++-*--------------------------------------------------------------------//

/*! \file Geometry.h
  Contains the Geometry class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_GEOMETRY_H__
#define __INCLUDED_PVR_GEOMETRY_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/lexical_cast.hpp>

// Project headers

#include "pvr/AttrTable.h"
#include "pvr/Exception.h"
#include "pvr/Meshes.h"
#include "pvr/Particles.h"
#include "pvr/Polygons.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// Geometry
//----------------------------------------------------------------------------//

/*! \class Geometry
  \brief Holds the various geometry types (Particles, Splines, Polygons).
 */

//----------------------------------------------------------------------------//

class Geometry
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Geometry);

  // Exceptions ----------------------------------------------------------------

  // Constructor, destructor, factory ------------------------------------------

  //! Constructs an empty Geometry object
  Geometry();

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  static Ptr create();

  // Main methods --------------------------------------------------------------

  /*! \{
    \name File I/O
  */

  //! Creates a new Geometry object from the contents of a file
  //! \note Currently supports: .geo, .bgeo
  static Geometry::Ptr read(const std::string &filename);

  //! \}

  /*! \{
    \name Main methods 
  */

  //! Sets the particles member
  void setParticles(Particles::Ptr particles);
  //! Sets the polygons member
  void setPolygons(Polygons::Ptr polygons);
  //! Sets the meshes member
  void setMeshes(Meshes::Ptr meshes);
  
  //! Returns pointer to the particles member
  Particles::Ptr particles(); 
  Particles::CPtr particles() const;

  //! Returns pointer to the polygons member
  Polygons::Ptr polygons();
  Polygons::CPtr polygons() const;

  //! Returns pointer to the meshes member
  Meshes::Ptr meshes();
  Meshes::CPtr meshes() const;

  //! Access to object attributes
  AttrTable& objectAttrs();

  //! Const access to object attributes
  const AttrTable& objectAttrs() const;

  //! \}

protected:
  
  // Protected data members ----------------------------------------------------

  //! Pointer to particle data
  Particles::Ptr m_particles;
  //! Pointer to polygon data
  Polygons::Ptr m_polygons;
  //! Pointer to mesh data
  Meshes::Ptr m_meshes;

  //! Primitive attributes. This should always have size 1.
  AttrTable m_objectAttrs;

};

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
