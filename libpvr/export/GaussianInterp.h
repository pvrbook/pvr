//-*-c++-*--------------------------------------------------------------------//

/*! \file GaussInterp.h
  Contains the GaussInterp class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_GAUSSINTERP_H__
#define __INCLUDED_PVR_GAUSSINTERP_H__

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
// GaussianFieldInterp
//----------------------------------------------------------------------------//

template <class Data_T>
class GaussianFieldInterp : public Field3D::FieldInterp<Data_T>
{
 public:
  typedef boost::intrusive_ptr<GaussianFieldInterp> Ptr;
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
    
    Gaussian gaussian(2.0, 2.0);
    
    Data_T value(0.0f);
    float normalization = 0.0f;
    for (int k = c.z; k < c.z + 4; ++k) {
      for (int j = c.y; j < c.y + 4; ++j) {
        for (int i = c.x; i < c.x + 4; ++i) {
          float weight = gaussian.eval(discToCont(i) - clampedVsP.x,
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
  struct Gaussian
  {
    Gaussian(float alpha, float width)
      : m_alpha(alpha), m_width(width),
        m_exp(std::exp(-alpha * width * width))
    { /* Empty */ }
    float eval(float x)
    {
      return std::max(0.0f, std::exp(-m_alpha * x * x) - m_exp);
    }
    float eval(float x, float y, float z)
    {
      return eval(x) * eval(y) * eval(z);
    }
  private:
    float m_alpha, m_width, m_exp;
  };
};

//----------------------------------------------------------------------------//

} // namespace Field3D

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
