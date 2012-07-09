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

/*! \file Scene.h
  Contains the Scene class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_SCENE_H__
#define __INCLUDED_PVR_SCENE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Forward declarations
//----------------------------------------------------------------------------//

class Light;
class Volume;

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

/*! \class Scene
  \brief Contains the scene elements (Volume and Light instances)
 */

//----------------------------------------------------------------------------//

class Scene
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Scene);
  typedef Util::SPtr<const Volume>::type VolumePtr;
  typedef Util::SPtr<const Light>::type  LightPtr;
  typedef std::vector<LightPtr>          LightVec;

  // Ctor, dtor, cloning -------------------------------------------------------

  Ptr clone() const;

  // Data members --------------------------------------------------------------

  VolumePtr volume;
  LightVec  lights;
};

//----------------------------------------------------------------------------//
// Implementations
//----------------------------------------------------------------------------//

inline Scene::Ptr Scene::clone() const
{ 
  return Ptr(new Scene(*this)); 
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
