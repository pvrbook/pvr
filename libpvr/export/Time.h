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
