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

/*! \file DensitySampler.h
  Contains the DensitySampler class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_DENSITYSAMPLER_H__
#define __INCLUDED_PVR_DENSITYSAMPLER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/RaymarchSamplers/RaymarchSampler.h"
#include "pvr/VolumeAttr.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// DensitySampler
//----------------------------------------------------------------------------//

/*! \class DensitySampler
   \brief Interprets density as both luminance and attenuation.
 */

//----------------------------------------------------------------------------//

class DensitySampler : public RaymarchSampler
{
public:
  // Typedefs ---
  PVR_TYPEDEF_SMART_PTRS(DensitySampler);
  // Ctor, factory ---
  DensitySampler();
  PVR_DEFINE_CREATE_FUNC(DensitySampler);
  // From ParamBase ---
  PVR_DEFINE_TYPENAME(DensitySampler);
  // From RaymarchSampler ---
  virtual RaymarchSample sample(const VolumeSampleState &state) const;
private:
  // Private data members ---
  VolumeAttr m_densityAttr;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
