//-*-c++-*--------------------------------------------------------------------//

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

#include "pvr/export.h"
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

class LIBPVR_PUBLIC Geometry
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Geometry);

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
  void                 setParticles(Particles::Ptr particles);
  //! Sets the polygons member
  void                 setPolygons(Polygons::Ptr polygons);
  //! Sets the meshes member
  void                 setMeshes(Meshes::Ptr meshes);
  
  //! Returns pointer to the particles member
  Particles::Ptr       particles(); 
  Particles::CPtr      particles() const;

  //! Returns pointer to the polygons member
  Polygons::Ptr        polygons();
  Polygons::CPtr       polygons() const;

  //! Returns pointer to the meshes member
  Meshes::Ptr          meshes();
  Meshes::CPtr         meshes() const;

  //! Access to global attributes
  AttrTable&           globalAttrs();

  //! Const access to global attributes
  const AttrTable&     globalAttrs() const;

  //! \}

protected:
  
  // Protected data members ----------------------------------------------------

  //! Pointer to particle data
  Particles::Ptr m_particles;
  //! Pointer to polygon data
  Polygons::Ptr m_polygons;
  //! Pointer to mesh data
  Meshes::Ptr m_meshes;

  //! Global attributes. This should always have size 1.
  AttrTable m_globalAttrs;

};

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
