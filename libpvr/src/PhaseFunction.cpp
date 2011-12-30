//----------------------------------------------------------------------------//

/*! \file PhaseFunction.cpp
  Contains implementations of PhaseFunction classes
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/PhaseFunction.h"

// System includes

// Library includes

// Project headers

#include "pvr/Strings.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {
namespace Phase {

//----------------------------------------------------------------------------//
// Isotropic
//----------------------------------------------------------------------------//

float Isotropic::probability(const Vector &in, const Vector &out) const
{
  return k_isotropic;
}

//----------------------------------------------------------------------------//
// HenyeyGreenstein
//----------------------------------------------------------------------------//

float HenyeyGreenstein::probability(const Vector &in, const Vector &out) const
{
  return k_isotropic;
}

//----------------------------------------------------------------------------//
// Schlick
//----------------------------------------------------------------------------//

float Schlick::probability(const Vector &in, const Vector &out) const
{
  return k_isotropic;
}

//----------------------------------------------------------------------------//
// Rayleigh
//----------------------------------------------------------------------------//

float Rayleigh::probability(const Vector &in, const Vector &out) const
{
  return k_isotropic;
}

//----------------------------------------------------------------------------//
// Mie
//----------------------------------------------------------------------------//

float Mie::probability(const Vector &in, const Vector &out) const
{
  return k_isotropic;
}

//----------------------------------------------------------------------------//

} // namespace Phase
} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
