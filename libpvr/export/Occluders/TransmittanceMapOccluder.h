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

/*! \file TransmittanceMapOccluder.h
  Contains the TransmittanceMapOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_TRANSMITTANCEMAPOCCLUDER_H__
#define __INCLUDED_PVR_TRANSMITTANCEMAPOCCLUDER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/export.h"
#include "pvr/Camera.h"
#include "pvr/DeepImage.h"
#include "pvr/Renderer.h"
#include "pvr/Occluders/Occluder.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// TransmittanceMapOccluder
//----------------------------------------------------------------------------//

/*! \class TransmittanceMapOccluder
  \brief Determines occlusion using a transmittance map.
 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC TransmittanceMapOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(TransmittanceMapOccluder);
  
  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(MissingCameraException, 
                     "TransmittanceMapOccluder has no camera.");
  DECLARE_PVR_RT_EXC(MissingTransmittanceMapException, 
                     "TransmittanceMapOccluder has no transmittance map.");

  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  TransmittanceMapOccluder(Renderer::CPtr renderer, Camera::CPtr camera,
                           const size_t numSamples);

  PVR_DEFINE_CREATE_FUNC_3_ARG(TransmittanceMapOccluder, 
                               Renderer::CPtr, Camera::CPtr, const size_t);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(TransmittanceMapOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Data members --------------------------------------------------------------

  bool            m_clipBehindCamera;
  DeepImage::CPtr m_transmittanceMap;
  Camera::CPtr    m_camera;
  Imath::V2f      m_rasterBounds;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
