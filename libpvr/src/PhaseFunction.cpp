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
// Composite
//----------------------------------------------------------------------------//

float Composite::probability(const Vector &in, const Vector &out) const
{
  float p = 0.0;
  float weight = 0.0;
  for (size_t i = 0, size = m_functions.size(); i < size; i++) {
    p += m_functions[i]->probability(in, out);
    weight += m_weights[i];
  }
  return p / weight;
}

//----------------------------------------------------------------------------//

void Composite::add(PhaseFunction::CPtr phaseFunction)
{
  m_functions.push_back(phaseFunction);
  m_weights.push_back(0.0f);
}

//----------------------------------------------------------------------------//

void Composite::setWeight(const size_t idx, const float weight)
{
  m_weights[idx] = weight;
}

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
