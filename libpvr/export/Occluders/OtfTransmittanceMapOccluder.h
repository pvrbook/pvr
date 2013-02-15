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

/*! \file OtfTransmittanceMapOccluder.h
  Contains the OtfTransmittanceMapOccluder class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_OTFTRANSMITTANCEMAPOCCLUDER_H__
#define __INCLUDED_PVR_OTFTRANSMITTANCEMAPOCCLUDER_H__

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
// OtfTransmittanceMapOccluder
//----------------------------------------------------------------------------//

/*! \class OtfTransmittanceMapOccluder
  \brief Determines occlusion using a transmittance map generated on the fly.
 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC OtfTransmittanceMapOccluder : public Occluder
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(OtfTransmittanceMapOccluder);
  
  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(MissingCameraException, 
                     "OtfTransmittanceMapOccluder has no camera.");

  // Constructor, factory method -----------------------------------------------

  //! Constructor requires a Renderer and Camera to use for precomputation.
  OtfTransmittanceMapOccluder(Renderer::CPtr renderer, Camera::CPtr camera,
                              const size_t numSamples);

  PVR_DEFINE_CREATE_FUNC_3_ARG(OtfTransmittanceMapOccluder, 
                               Renderer::CPtr, Camera::CPtr, const size_t);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(OtfTransmittanceMapOccluder);

  // From Occluder -------------------------------------------------------------

  virtual Color sample(const OcclusionSampleState &state) const;

protected:

  // Utility methods -----------------------------------------------------------

  size_t offset(const size_t x, const size_t y) const
  { return x + m_resolution.x * y; }
  void updateCoordinate(const Vector &rsP) const;
  void updatePixel(const size_t x, const size_t y) const;

  // Data members --------------------------------------------------------------

  bool                      m_clipBehindCamera;
  Renderer::CPtr            m_renderer;
  Camera::CPtr              m_camera;
  Imath::V2f                m_floatRasterBounds;
  Imath::V2i                m_intRasterBounds;
  Imath::V2i                m_resolution;
  mutable DeepImage         m_transmittanceMap;
  mutable std::vector<char> m_computed;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
