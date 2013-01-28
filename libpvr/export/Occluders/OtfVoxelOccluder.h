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

/*! \file OtfVoxelOccluder.h
  Contains the OtfVoxelOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_OTFVOXELOCCLUDER_H__
#define __INCLUDED_PVR_OTFVOXELOCCLUDER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <Field3D/FieldInterp.h>

// Project headers

#include "pvr/export.h"
#include "pvr/Renderer.h"
#include "pvr/VoxelBuffer.h"
#include "pvr/Occluders/Occluder.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// OtfVoxelOccluder
//----------------------------------------------------------------------------//

/*! \class OtfVoxelOccluder
  \brief Determines occlusion using a transmittance map.
 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC OtfVoxelOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(OtfVoxelOccluder);
  
  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  OtfVoxelOccluder(Renderer::CPtr renderer, const Vector &wsLightPos,
                const size_t res);

  PVR_DEFINE_CREATE_FUNC_3_ARG(OtfVoxelOccluder, Renderer::CPtr, const Vector&,
                            const size_t);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(OtfVoxelOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Utility methods -----------------------------------------------------------

  void updateVoxel(const int i, const int j, const int k) const;

  // Data members --------------------------------------------------------------

  Renderer::CPtr m_renderer;
  const Vector m_wsLightPos;
  mutable DenseBuffer m_buffer;
  //! Linear interpolator
  Field3D::LinearFieldInterp<Imath::V3f> m_linearInterp;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
