//-*-c++-*--------------------------------------------------------------------//

/*! \file Time.h
  Contains the tine-related classes and functions
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_TIME_H__
#define __INCLUDED_PVR_TIME_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Time
//----------------------------------------------------------------------------//

/*! \class Time
  \brief Holds a time in seconds.

  This class typefies time to ensure that parametric time is not accidentally
  passed to functions and/or data structures that expect a time measurement
  in seconds, and vice versa.
 */

//----------------------------------------------------------------------------//

class Time
{
public:
  explicit Time(const float t)
    : m_value(t)
  { }
  operator float() const
  { return m_value; }
  float value() const
  { return m_value; }
private:
  float m_value;
};

//----------------------------------------------------------------------------//
// PTime
//----------------------------------------------------------------------------//

/*! \class PTime
  \brief Holds a time in the parametric shutter open/close [0,1] time frame.

  See Time for an explanation of why this is needed.
 */

//----------------------------------------------------------------------------//

class PTime
{
public:
  explicit PTime(const float t)
    : m_value(t)
  { }
  operator float() const
  { return m_value; }
  float value() const
  { return m_value; }
private:
  float m_value;
};

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
