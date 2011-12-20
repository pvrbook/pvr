//-*-c++-*--------------------------------------------------------------------//

/*! \file RenderGlobals.h
  Contains the RenderGlobals class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RENDERGLOBALS_H__
#define __INCLUDED_PVR_RENDERGLOBALS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

#include "pvr/Exception.h"

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// RenderGlobals
//----------------------------------------------------------------------------//

class RenderGlobals 
{
public:
  
  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(BadFpsException, "Bad frames per second value:");
  DECLARE_PVR_RT_EXC(BadShutterException, "Bad shutter value:");

  // Main methods --------------------------------------------------------------

  //! Sets the motion blur settings
  //! \param fps Frames per second. Should be 24 for film
  //! \param shutter Fraction of frame length that shutter is open. A value 
  //! of 0.5 corresponsonds to a 180 degree shutter angle.
  static void setupMotionBlur(const float fps, const float shutter);

  // Accessors -----------------------------------------------------------------

  //! Returns the number of frames per second
  static float fps()
  { return ms_fps; }

  //! Returns the shutter length
  static float shutter() 
  { return ms_shutter; }

  //! Returns length (in seconds) of open shutter time.
  static float dt()
  { return ms_dt; }

private:

  // Data members --------------------------------------------------------------

  static float ms_fps;
  static float ms_shutter;
  static float ms_dt;

};

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
