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

/*! \file Acceleration.h
  Contains acceleration data structures
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_ACCELERATION_H__
#define __INCLUDED_PVR_ACCELERATION_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <vector>

// Library headers

#include <Field3D/DenseField.h>

// Project headers

#include "pvr/Math.h"
#include "pvr/Strings.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Accel {

//----------------------------------------------------------------------------//
// UniformGrid
//----------------------------------------------------------------------------//

template <class T>
class UniformGrid
{
public:

  // Typedefs ------------------------------------------------------------------

  typedef std::vector<T> HashVec;
  typedef Field3D::DenseField<HashVec> HashGrid;

  // Ctors ---------------------------------------------------------------------

  //! Defaults constructor. Creates a 32^3 grid with cell size 1 at the origin.
  UniformGrid();

  // Main methods --------------------------------------------------------------

  //! Resets the 
  void clear(const float cellSize, const size_t res, const pvr::Vector origin);
  //! Returns the vector of hits given a point in space
  const HashVec& get(const pvr::Vector &p) const;
  //! Adds a point (with radius) to the grid. 
  void addPoint(pvr::Vector p, const float radius, const T value);
  //! Adds a line with radius to the grid
  void addLine(pvr::Vector p0, pvr::Vector p1, const float radius,
               const T value);
  
private:

  // Utility methods -----------------------------------------------------------

  //! Computes an integer 3D coordinate given a point in space.
  Imath::V3i hash(pvr::Vector p) const;

  // Data members --------------------------------------------------------------

  //! Resolution of hash
  size_t m_size;
  //! Cell size of hash
  float m_cellSize;
  //! Origin (lower left corner) of hash
  pvr::Vector m_origin;

  //! 3D Array of vector<T>
  HashGrid m_hash;

};

//----------------------------------------------------------------------------//
// Template implementations
//----------------------------------------------------------------------------//

template <class T>
UniformGrid<T>::UniformGrid()
  : m_size(32), m_cellSize(1.0), m_origin(0.0)
{ 
  m_hash.setSize(Imath::V3i(m_size));
}
  
//----------------------------------------------------------------------------//

template <class T>
void UniformGrid<T>::clear(const float cellSize, const size_t res, 
                           const Vector origin)
{
  m_cellSize = cellSize;
  m_size = res;
  m_origin = origin;
  m_hash.setSize(Imath::V3i(m_size));
}
  
//----------------------------------------------------------------------------//

template <class T>
const typename UniformGrid<T>::HashVec& 
UniformGrid<T>::get(const pvr::Vector &p) const
{
  Imath::V3i idx = hash(p - m_origin);
  return m_hash.fastValue(idx.x, idx.y, idx.z);
}
  
//----------------------------------------------------------------------------//

template <class T>
void UniformGrid<T>::addPoint(pvr::Vector p, const float radius, const T value)
{
  p -= m_origin;
  for (typename HashGrid::iterator i = m_hash.begin(), end = m_hash.end();
       i != end; ++i) {
    pvr::Vector cellPos(i.x * m_cellSize, i.y * m_cellSize, i.z * m_cellSize);
    if ((p - cellPos).length() < radius + m_cellSize) {
      i->push_back(value);
    }
  }
}

//----------------------------------------------------------------------------//

template <class T>
void UniformGrid<T>::addLine(pvr::Vector p0, pvr::Vector p1, const float radius,
                          const T value)
{
  using namespace pvr;
  double t;
  p0 -= m_origin;
  p1 -= m_origin;
  for (typename HashGrid::iterator i = m_hash.begin(), end = m_hash.end();
       i != end; ++i) {
    Vector cellPos(i.x * m_cellSize, i.y * m_cellSize, i.z * m_cellSize);
    Vector pOnLine =  Math::closestPointOnLineSegment(p0, p1, cellPos, t);
    if ((pOnLine - cellPos).length() < radius + m_cellSize) {
      i->push_back(value);
    }
  }
}
  
//----------------------------------------------------------------------------//

template <class T>
Imath::V3i UniformGrid<T>::hash(pvr::Vector p) const
{
  p.x = std::max(0.0, p.x);
  p.y = std::max(0.0, p.y);
  p.z = std::max(0.0, p.z);
  size_t x = static_cast<size_t>(std::floor(p.x / m_cellSize));
  size_t y = static_cast<size_t>(std::floor(p.y / m_cellSize));
  size_t z = static_cast<size_t>(std::floor(p.z / m_cellSize));
  x = std::min(x, m_size - 1);
  y = std::min(y, m_size - 1);
  z = std::min(z, m_size - 1);
  return Imath::V3i(x, y, z);
}

//----------------------------------------------------------------------------//

} // namespace Accel
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
