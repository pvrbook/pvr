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

/*! \file CubicInterp.h
  Contains the CubicInterp class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_CUBICINTERP_H__
#define __INCLUDED_PVR_CUBICINTERP_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

#include <Field3D/FieldInterp.h>

// Project headers

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace Field3D {

//----------------------------------------------------------------------------//
// Interpolation functions
//----------------------------------------------------------------------------//

template <class Data_T, class Coord_T>
Data_T cubicInterp(Coord_T x, Data_T p[4])
{
  return p[1] + 0.5 * 
    x * (p[2] - p[0] + 
         x * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2] - p[3] + 
              x * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
}

//----------------------------------------------------------------------------//

template <class Data_T, class Coord_T>
Data_T bicubicInterp(Coord_T x, Coord_T y, Data_T p[4][4]) 
{
  Data_T yInterps[4];
  yInterps[0] = cubicInterp(y, p[0]);
  yInterps[1] = cubicInterp(y, p[1]);
  yInterps[2] = cubicInterp(y, p[2]);
  yInterps[3] = cubicInterp(y, p[3]);
  return cubicInterp(x, yInterps);
}

//----------------------------------------------------------------------------//

template <class Data_T, class Coord_T>
Data_T tricubicInterp(Coord_T x, Coord_T y, Coord_T z, Data_T p[4][4][4]) 
{
  Data_T yzInterps[4];
  yzInterps[0] = bicubicInterp(y, z, p[0]);
  yzInterps[1] = bicubicInterp(y, z, p[1]);
  yzInterps[2] = bicubicInterp(y, z, p[2]);
  yzInterps[3] = bicubicInterp(y, z, p[3]);
  return cubicInterp(x, yzInterps);
}

//----------------------------------------------------------------------------//
// TriCubicFieldInterp
//----------------------------------------------------------------------------//

template <class Data_T>
class TriCubicFieldInterp : public Field3D::FieldInterp<Data_T>
{
 public:
  
  // Typedefs ------------------------------------------------------------------

  typedef boost::intrusive_ptr<TriCubicFieldInterp> Ptr;

  // RTTI replacement ----------------------------------------------------------

  typedef TriCubicFieldInterp class_type;
  DEFINE_FIELD_RTTI_CONCRETE_CLASS;

  static const char *staticClassName()
  {
    return "TriCubicFieldInterp";
  }

  static const char* classType()
  {
    return class_type::ms_classType.name();
  }
  
  // From FieldInterp ----------------------------------------------------------

  virtual Data_T sample(const Field<Data_T> &data, const V3d &vsP) const
  {
    // Voxel centers are at .5 coordinates
    // NOTE: Don't use contToDisc for this, we're looking for sample
    // point locations, not coordinate shifts.
    FIELD3D_VEC3_T<double> p(vsP - FIELD3D_VEC3_T<double>(0.5));
    
    const Box3i &dw = data.dataWindow();
  
    // Lower left corner
    V3i c(static_cast<int>(floor(p.x)) - 1, 
          static_cast<int>(floor(p.y)) - 1, 
          static_cast<int>(floor(p.z)) - 1);

    // Interpolation coords
    V3f x(p.x - floor(p.x), p.y - floor(p.y), p.z - floor(p.z));
    
    Data_T values[4][4][4];
    for (int k = c.z, ki = 0; k < c.z + 4; k++, ki++) {
      for (int j = c.y, ji = 0; j < c.y + 4; j++, ji++) {
        for (int i = c.x, ii = 0; i < c.x + 4; i++, ii++) {
          int iIdx = std::min(std::max(i, dw.min.x), dw.max.x);
          int jIdx = std::min(std::max(j, dw.min.y), dw.max.y);
          int kIdx = std::min(std::max(k, dw.min.z), dw.max.z);
          values[ii][ji][ki] = data.value(iIdx, jIdx, kIdx);
        }
      }
    }

    return tricubicInterp(x.x, x.y, x.z, values);
  }

private:

  // Static data members -------------------------------------------------------

  static TemplatedFieldType<TriCubicFieldInterp<Data_T> > ms_classType;

  // Typedefs ------------------------------------------------------------------

  //! Convenience typedef for referring to base class
  typedef RefBase base;

};

//----------------------------------------------------------------------------//
// Static data member instantiation
//----------------------------------------------------------------------------//

FIELD3D_CLASSTYPE_TEMPL_INSTANTIATION(TriCubicFieldInterp);

//----------------------------------------------------------------------------//

} // namespace Field3D

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
