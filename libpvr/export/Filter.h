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

/*! \file Filter.h
  Contains the Filter class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_FILTER_H__
#define __INCLUDED_PVR_FILTER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <cmath>

#include <boost/shared_ptr.hpp>

// Library headers

// Project headers

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Filter {

//----------------------------------------------------------------------------//
// Filter functions
//----------------------------------------------------------------------------//

template <typename Data_T, typename Filt_T>
Data_T filter1D(const float x, const Data_T values[Filt_T::width], 
                const Filt_T &filter)
{
  Data_T result(0.0);
  float weight(0.0);
  for (int i = 0; i < Filt_T::width; i++) {
    const float t = x + (Filt_T::width / 2) - 1;
    const float filterValue = filter(std::abs(i - t));
    result += filterValue * values[i];
    weight += filterValue;
  }
  return result / weight;
}

//----------------------------------------------------------------------------//

template <typename Data_T, typename Filt_T>
Data_T filter2D(const float x, const float y, 
                const Data_T values[Filt_T::width][Filt_T::width], 
                const Filt_T &filter)
{
  Data_T yVals[Filt_T::width];
  for (int i = 0; i < Filt_T::width; i++) {
    yVals[i] = filter1D<Data_T,Filt_T>(y, values[i], filter);
  }
  return filter1D<Data_T,Filt_T>(x, yVals, filter);
}

//----------------------------------------------------------------------------//

template <typename Data_T, typename Filt_T>
Data_T filter3D(const float x, const float y, const float z, 
                const Data_T values[Filt_T::width][Filt_T::width][Filt_T::width], 
                const Filt_T &filter)
{
  Data_T yzVals[Filt_T::width];
  for (int i = 0; i < Filt_T::width; i++) {
    yzVals[i] = filter2D<Data_T,Filt_T>(y, z, values[i], filter);
  }
  return filter1D<Data_T,Filt_T>(x, yzVals, filter);
}

//----------------------------------------------------------------------------//
// MitchellNetravali
//----------------------------------------------------------------------------//

struct MitchellNetravali
{
  MitchellNetravali(const float B = 1.0 / 3.0, const float C = 1.0 / 3.0)
      : m_B(B), m_C(C)
  { /* Empty */ }
  float operator()(const float x) const
  {
    const float ax = std::abs(x);
    if (ax < 1) {
      return ((12 - 9 * m_B - 6 * m_C) * ax * ax * ax +
              (-18 + 12 * m_B + 6 * m_C) * ax * ax + (6 - 2 * m_B)) / 6;
    } else if ((ax >= 1) && (ax < 2)) {
      return ((-m_B - 6 * m_C) * ax * ax * ax +
              (6 * m_B + 30 * m_C) * ax * ax + (-12 * m_B - 48 * m_C) *
              ax + (8 * m_B + 24 * m_C)) / 6;
    } else {
      return 0;
    }
  }
  static const int width = 4;
private:
  const float m_B, m_C;
};

//----------------------------------------------------------------------------//
// Gaussian
//----------------------------------------------------------------------------//

struct Gaussian
{
  Gaussian(const float alpha = 2.0, const float width = 2.0)
    : m_alpha(alpha), 
      m_exp(std::exp(-alpha * width * width))
  { /* Empty */ }
  float operator()(const float x) const
  {
    return std::max(0.0f, std::exp(-m_alpha * x * x) - m_exp);
  }
  static const int width = 4;
private:
  const float m_alpha, m_exp;
};

//----------------------------------------------------------------------------//

} // namespace Filter
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
