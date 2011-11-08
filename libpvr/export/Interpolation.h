//-*-c++-*--------------------------------------------------------------------//

/*! \file Interpolation.h
  Contains various interpolation functions
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INTERPOLATION_H__
#define __INCLUDED_PVR_INTERPOLATION_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {
namespace Util {

//----------------------------------------------------------------------------//
// Interpolation functions
//----------------------------------------------------------------------------//

//! Linear interpolation in two dimensions
template <typename T>
T lerp2D(const float tX, const float tY, const T &x0y0, const T &x1y0,
         const T &x0y1, const T &x1y1);

//----------------------------------------------------------------------------//
// Template implementations
//----------------------------------------------------------------------------//

template <typename T>
T lerp2D(const float tX, const float tY, const T &x0y0, const T &x1y0,
         const T &x0y1, const T &x1y1)
{
  return ((1.0f - tX) * ((1.0f - tY) * x0y0 + tY * x0y1) + 
                  tX  * ((1.0f - tY) * x1y0 + tY * x1y1));
}

//----------------------------------------------------------------------------//

} // namespace Util
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
