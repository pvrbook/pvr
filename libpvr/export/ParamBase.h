//-*-c++-*--------------------------------------------------------------------//

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
