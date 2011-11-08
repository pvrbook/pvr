//-*-c++-*--------------------------------------------------------------------//

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
      : Exception(std::string(baseErr) + " " + what)            \
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
