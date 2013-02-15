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

/*! \file Exception.h
  Contains the Exception base class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_EXCEPTION_H__
#define __INCLUDED_PVR_EXCEPTION_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include <exception>
#include <stdexcept>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Sys {

//----------------------------------------------------------------------------//
// Exception base class
//----------------------------------------------------------------------------//
 
//! Base class for PVR exceptions.
class Exception : public std::runtime_error
{
public:
  explicit Exception(const std::string &what) throw()
    : std::runtime_error(what)
  { }
  ~Exception() throw()
  { }
};

//----------------------------------------------------------------------------//
// Exception macro
//----------------------------------------------------------------------------//

//! Used to conveniently define a new exception class
//! \todo Rename this PVR_DECLARE_RT_EXC
#define DECLARE_PVR_RT_EXC(name, baseErr)                       \
  class name : public Sys::Exception                            \
  {                                                             \
  public:                                                       \
    explicit name(const std::string &what = "") throw()         \
      : Exception(std::string(baseErr).append(" ").append(what))\
    { }                                                         \
    ~name() throw()                                             \
      { }                                                       \
  };                                                            \
  
//----------------------------------------------------------------------------//
// Common PVR exceptions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//

} // namespace Sys
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
