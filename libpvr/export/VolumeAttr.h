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

/*! \file VolumeAttr.h
  Contains the VolumeAttr class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_VOLUMEATTR_H__
#define __INCLUDED_PVR_VOLUMEATTR_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Exception.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// VolumeAttr class
//----------------------------------------------------------------------------//

/*! \class VolumeAttr
  \brief Utility class that makes it simple to efficiently reference an 
  attribute in a Volume using a name string.

  A VolumeAttr instance is created with its name, and its index set to
  IndexNotSet. The first time the instance is used to sample a Volume, that
  Volume is responsible for setting the index of the VolumeAttr, so that
  future calls to Volume::sample() efficiently can determine which attribute
  is requested.

  \note A single VolumeAttr can only be used to sample into a single Volume.
  Once the index has been locked in, there is no guarantee that the attribute
  name first specified will have the same index in a second Volume.
  Caveat Emptor.
 */

//----------------------------------------------------------------------------//

class VolumeAttr
{
public:
  
  // Enums ---------------------------------------------------------------------

  static const int IndexNotSet  = -1;
  static const int IndexInvalid = -2;

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(SetIndexException, "Couldn't set index of of VolumeAttr:");

  // Constructors --------------------------------------------------------------

  VolumeAttr(const std::string &name);

  // Main methods --------------------------------------------------------------

  //! Returns the name of the attribute
  const std::string& name() const;
  //! Returns the index of the attribute
  int                index() const;
  //! Sets the index of the attribute.
  //! \note This is const, because the value always needs to be set on first
  //! use, and re-setting is prohibited.
  //! \throws SetIndexException if trying to set the index more than once
  void               setIndex(const int index) const;
  //! Marks the index as invalid. This is set whenever a Volume decides it
  //! can't provide the requested attribute
  void               setIndexInvalid() const;

private:

  // Data members --------------------------------------------------------------

  //! Name of attribute
  std::string m_name;
  //! Index of attribute. This is IndexNotSet by default.
  mutable int m_index;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
