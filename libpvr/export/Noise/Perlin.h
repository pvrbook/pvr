//-*-c++-*--------------------------------------------------------------------//

/*! \file Perlin.h
  Contains a C implementation of Ken Perlin's 'Improved' noise function.

  http://mrl.nyu.edu/~perlin/noise/
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PERLIN_H__
#define __INCLUDED_PVR_PERLIN_H__

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
namespace Perlin {
 
//----------------------------------------------------------------------------//

//! \brief Improved Perlin noise
double noise(double x, double y, double z);
  
//! \brief Improved Perlin noise, overloaded to take Vector
double noise(const Vector &p);

//----------------------------------------------------------------------------//
// Implementations of inline functions
//----------------------------------------------------------------------------//

inline double noise(const Vector &p)
{
  return noise(p.x, p.y, p.z);
}

//----------------------------------------------------------------------------//

} // namespace Perlin
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
