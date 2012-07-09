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

/*! \file PhysicalSampler.h
  Contains the PhysicalSampler class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PHYSICALSAMPLER_H__
#define __INCLUDED_PVR_PHYSICALSAMPLER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/RenderState.h"
#include "pvr/Types.h"
#include "pvr/VolumeAttr.h"
#include "pvr/RaymarchSamplers/RaymarchSampler.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// PhysicalSampler
//----------------------------------------------------------------------------//

/*! \class PhysicalSampler
   \brief Looks for scattering and performs a simplified lighting calculation.
 */

//----------------------------------------------------------------------------//

class PhysicalSampler : public RaymarchSampler
{
public:
  
  // Typedefs ---

  PVR_TYPEDEF_SMART_PTRS(PhysicalSampler);

  // Constructor, factory ---

  //! Default constructor. Sets up volume attribute names.
  PhysicalSampler();

  PVR_DEFINE_CREATE_FUNC(PhysicalSampler);

  // From ParamBase ---

  PVR_DEFINE_TYPENAME(PhysicalSampler);
  
  // From RaymarchSampler ---

  virtual RaymarchSample sample(const VolumeSampleState &state) const;

private:

  // Private data members ---

  //! Used for sampling the scattering attribute
  VolumeAttr m_scatteringAttr;
  //! Used for sampling the absorption attribute
  VolumeAttr m_absorptionAttr;
  //! Used for sampling the emission attribute
  VolumeAttr m_emissionAttr;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
