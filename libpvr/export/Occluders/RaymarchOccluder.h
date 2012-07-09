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

/*! \file RaymarchOccluder.h
  Contains the RaymarchOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RAYMARCHOCCLUDER_H__
#define __INCLUDED_PVR_RAYMARCHOCCLUDER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Occluders/Occluder.h"
#include "pvr/Renderer.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// RaymarchOccluder
//----------------------------------------------------------------------------//

/*! \class RaymarchOccluder
  \brief Calculates occlusion by raymarching
 */

//----------------------------------------------------------------------------//

class RaymarchOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(RaymarchOccluder);

  // Factory methods -----------------------------------------------------------

  //! Default constructor
  RaymarchOccluder(Renderer::CPtr renderer);
  PVR_DEFINE_CREATE_FUNC_1_ARG(RaymarchOccluder, Renderer::CPtr)

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(RaymarchOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Data members --------------------------------------------------------------

  Renderer::CPtr m_renderer;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
