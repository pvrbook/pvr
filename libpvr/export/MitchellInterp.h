//-*-c++-*--------------------------------------------------------------------//

/*! \file MitchellInterp.h
  Contains the MitchellInterp class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_MITCHELLINTERP_H__
#define __INCLUDED_PVR_MITCHELLINTERP_H__

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
// MitchellFieldInterp
//----------------------------------------------------------------------------//

template <class Data_T>
class MitchellFieldInterp : public Field3D::FieldInterp<Data_T>
{
 public:
  
  // Typedefs ------------------------------------------------------------------

  typedef boost::intrusive_ptr<MitchellFieldInterp> Ptr;

  // RTTI replacement ----------------------------------------------------------

  typedef MitchellFieldInterp class_type;
  DEFINE_FIELD_RTTI_CONCRETE_CLASS;

  static const char *staticClassName()
  {
    return "MitchellFieldInterp";
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
    V3d clampedVsP(std::max(0.5, vsP.x),
                   std::max(0.5, vsP.y),
                   std::max(0.5, vsP.z));
    FIELD3D_VEC3_T<double> p(clampedVsP - FIELD3D_VEC3_T<double>(0.5));
    
    const Box3i &dataWindow = data.dataWindow();
  
    // Lower left corner
    V3i c(static_cast<int>(floor(p.x)) - 1, 
          static_cast<int>(floor(p.y)) - 1, 
          static_cast<int>(floor(p.z)) - 1);
    
    MitchellNetravali filter(1.0/3.0, 1.0/3.0);
    
    Data_T value(0.0f);
    float normalization = 0.0f;
    for (int k = c.z; k < c.z + 4; ++k) {
      for (int j = c.y; j < c.y + 4; ++j) {
        for (int i = c.x; i < c.x + 4; ++i) {
          float weight = filter.eval(discToCont(i) - clampedVsP.x,
                                     discToCont(j) - clampedVsP.y,
                                     discToCont(k) - clampedVsP.z);
          int ic = std::max(dataWindow.min.x, std::min(i, dataWindow.max.x));
          int jc = std::max(dataWindow.min.y, std::min(j, dataWindow.max.y));
          int kc = std::max(dataWindow.min.z, std::min(k, dataWindow.max.z));
          value += weight * data.value(ic, jc, kc);
          normalization += weight;
        }
      }
    }

    return value / normalization;
  }

  struct MitchellNetravali
  {
    MitchellNetravali(float B, float C)
      : m_B(B), m_C(C)
    { /* Empty */ }
    float eval(float x)
    {
      float ax = std::abs(x);
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
    float eval(float x, float y, float z)
    {
      return eval(x) * eval(y) * eval(z);
    }
  private:
    float m_B, m_C;
  };
  
private:

  // Static data members -------------------------------------------------------

  static TemplatedFieldType<MitchellFieldInterp<Data_T> > ms_classType;

  // Typedefs ------------------------------------------------------------------

  //! Convenience typedef for referring to base class
  typedef RefBase base;

};

//----------------------------------------------------------------------------//
// Static data member instantiation
//----------------------------------------------------------------------------//

FIELD3D_CLASSTYPE_TEMPL_INSTANTIATION(MitchellFieldInterp);

//----------------------------------------------------------------------------//

} // namespace Field3D

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
