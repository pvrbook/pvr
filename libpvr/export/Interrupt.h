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

/*! \file Interrupt.h
  Contains the Interrupt base class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INTERRUPT_H__
#define __INCLUDED_PVR_INTERRUPT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "pvr/export.h"
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

class LIBPVR_PUBLIC Interrupt
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Interrupt);

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
