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

/*! \file PointLight.h
  Contains the PointLight class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_POINTLIGHT_H__
#define __INCLUDED_PVR_POINTLIGHT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/export.h"
#include "pvr/Lights/Light.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// PointLight
//----------------------------------------------------------------------------//

/*! \class PointLight
  \brief Implements a simple point light

 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC PointLight : public Light
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(PointLight);

  // Ctor, factory -------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(PointLight);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(PointLight);

  // From Light ----------------------------------------------------------------

  virtual LightSample sample(const LightSampleState &state) const;

  // Main methods --------------------------------------------------------------

  //! Sets the position of the point light
  void                setPosition(const Vector &wsP);
  //! Returns the position of the point light
  Vector              position() const;

private:

  // Private data members ------------------------------------------------------

  //! Position of point light
  Vector m_wsP;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
