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

/*! \file ParamBase.h
  Contains the ParamBase class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PARAMBASE_H__
#define __INCLUDED_PVR_PARAMBASE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

// Project headers

#include "pvr/ParamMap.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//

namespace Util {
  
//! Base class that provides a convenient entry point for control
//! via ParamMap input. Also provides the typeName() necessary for
//! runtime identification (mostly logging)
class ParamBase
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(ParamBase);

  // Ctor, dtor ----------------------------------------------------------------

  //! Virtual destructor
  virtual ~ParamBase()
  { }

  // To be implemented by subclasses -------------------------------------------

  //! Returns the class' type name.
  virtual std::string typeName() const = 0;    
  //! Subclasses implement this to decide what they want to do with
  //! default attribute values. ParamBase itself does nothing.
  virtual void setParams(const ParamMap &params)
  { 
    // Default implementation does nothing.
  }
};
  
//----------------------------------------------------------------------------//

} // namespace Util
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
