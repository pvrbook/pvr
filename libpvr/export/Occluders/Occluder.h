//-*-c++-*--------------------------------------------------------------------//

/*! \file Occluder.h
  Contains the Occluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_OCCLUDER_H__
#define __INCLUDED_PVR_OCCLUDER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Constants.h"
#include "pvr/ParamBase.h"
#include "pvr/RenderState.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Occluder
//----------------------------------------------------------------------------//

/*! \class Occluder
  \brief Base class for transmission functions that determine how much late can
  travel between two points in the scene. 

  These are used both for shadowing and holdouts.
 */

//----------------------------------------------------------------------------//

class Occluder : public Util::ParamBase
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Occluder);

  // Constructors, destructor --------------------------------------------------

  virtual ~Occluder()
  { }

  // To be implemented by subclasses -------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const = 0;

};

//----------------------------------------------------------------------------//
// NullOccluder
//----------------------------------------------------------------------------//

/*! \class NullOccluder
  \brief Occluder that always returns full transmittance.
 */

class NullOccluder : public Occluder
{
public:
  PVR_TYPEDEF_SMART_PTRS(NullOccluder);
  PVR_DEFINE_CREATE_FUNC(NullOccluder);
  PVR_DEFINE_TYPENAME(NullOccluder);
  virtual Color sample(const OcclusionSampleState &state) const
  { return Colors::one(); }
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
