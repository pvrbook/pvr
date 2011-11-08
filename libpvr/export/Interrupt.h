//-*-c++-*--------------------------------------------------------------------//

/*! \file Interrupt.h
  Contains the Interrupt base class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INTERRUPT_H__
#define __INCLUDED_PVR_INTERRUPT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Exception.h"
#include "Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Sys {

//----------------------------------------------------------------------------//
// Exceptions
//----------------------------------------------------------------------------//

DECLARE_PVR_RT_EXC(UserInterruptException, "User interrupt from python");

//----------------------------------------------------------------------------//
// Interrupt base class
//----------------------------------------------------------------------------//

class Interrupt
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Interrupt);

  // Main methods --------------------------------------------------------------

  virtual bool abort() const = 0;

  // Static methods ------------------------------------------------------------

  //! Sets the global interrupt handler
  static void setGlobalInterrupt(Interrupt::Ptr interrupt)
  { ms_interrupt = interrupt; }
  //! Checks whether to abort, returns true/false
  static bool checkAbort()
  { return ms_interrupt ? ms_interrupt->abort() : false; }
  //! Checks whether to abort, throws an exception if true.
  static void throwOnAbort()
  { 
    if (ms_interrupt && ms_interrupt->abort()) {
      throw UserInterruptException();
    }
  }

private:

  //! Pointer to global interrupt handler
  static Ptr ms_interrupt;

};

//----------------------------------------------------------------------------//

} // namespace Sys
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
