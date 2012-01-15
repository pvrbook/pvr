//-*-c++-*--------------------------------------------------------------------//

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
  // Typedefs
  PVR_TYPEDEF_SMART_PTRS(Scene);
  typedef Util::SPtr<const Volume>::type VolumePtr;
  typedef Util::SPtr<const Light>::type  LightPtr;
  typedef std::vector<LightPtr>          LightVec;
  // Cloning
  Ptr clone() const;
  // Data members
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
