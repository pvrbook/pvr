//-*-c++-*--------------------------------------------------------------------//

/*! \file LinearInterp.h
  Contains the LinearInterp class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_LINEARINTERP_H__
#define __INCLUDED_PVR_LINEARINTERP_H__

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
Data_T linearInterp(Coord_T x, Data_T p[2])
{
  return (1.0 - x) * p[0] + x * p[1];
}

//----------------------------------------------------------------------------//

template <class Data_T, class Coord_T>
Data_T bilinearInterp(Coord_T x, Coord_T y, Data_T p[2][2]) 
{
  Data_T yInterps[2];
  yInterps[0] = linearInterp(y, p[0]);
  yInterps[1] = linearInterp(y, p[1]);
  return linearInterp(x, yInterps);
}

//----------------------------------------------------------------------------//

template <class Data_T, class Coord_T>
Data_T trilinearInterp(Coord_T x, Coord_T y, Coord_T z, Data_T p[2][2][2]) 
{
  Data_T yzInterps[2];
  yzInterps[0] = bilinearInterp(y, z, p[0]);
  yzInterps[1] = bilinearInterp(y, z, p[1]);
  return linearInterp(x, yzInterps);
}

//----------------------------------------------------------------------------//
// TriLinearFieldInterp
//----------------------------------------------------------------------------//

template <class Data_T>
class TriLinearFieldInterp : public Field3D::FieldInterp<Data_T>
{
 public:
  
  // Typedefs ------------------------------------------------------------------

  typedef boost::intrusive_ptr<TriLinearFieldInterp> Ptr;

  // RTTI replacement ----------------------------------------------------------

  typedef TriLinearFieldInterp class_type;
  DEFINE_FIELD_RTTI_CONCRETE_CLASS;

  static const char *staticClassName()
  {
    return "TriLinearFieldInterp";
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
    V3i c(static_cast<int>(floor(p.x)), 
          static_cast<int>(floor(p.y)), 
          static_cast<int>(floor(p.z)));

    // Interpolation coords
    V3f x(p.x - floor(p.x), p.y - floor(p.y), p.z - floor(p.z));
    
    using namespace std;

    Data_T values[2][2][2];
    for (int k = c.z, ki = 0; k < c.z + 2; k++, ki++) {
      for (int j = c.y, ji = 0; j < c.y + 2; j++, ji++) {
        for (int i = c.x, ii = 0; i < c.x + 2; i++, ii++) {
          int iIdx = std::min(std::max(i, dw.min.x), dw.max.x);
          int jIdx = std::min(std::max(j, dw.min.y), dw.max.y);
          int kIdx = std::min(std::max(k, dw.min.z), dw.max.z);
          values[ii][ji][ki] = data.value(iIdx, jIdx, kIdx);
        }
      }
    }

    return trilinearInterp(x.x, x.y, x.z, values);
  }

private:

  // Static data members -------------------------------------------------------

  static TemplatedFieldType<TriLinearFieldInterp<Data_T> > ms_classType;

  // Typedefs ------------------------------------------------------------------

  //! Convenience typedef for referring to base class
  typedef RefBase base;

};

//----------------------------------------------------------------------------//
// Static data member instantiation
//----------------------------------------------------------------------------//

FIELD3D_CLASSTYPE_TEMPL_INSTANTIATION(TriLinearFieldInterp);

//----------------------------------------------------------------------------//

} // namespace Field3D

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
