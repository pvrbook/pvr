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

#include <stdlib.h>

// Library includes

// Project headers

#include "pvr/Math.h"
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
    p += m_functions[i]->probability(in, out) * m_weights[i];
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
  assert(idx < m_weights.size());
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

HenyeyGreenstein::HenyeyGreenstein(float g)
  : m_g(Imath::clamp(g, -1.0f, 1.0f))
{ 

}

//----------------------------------------------------------------------------//

float HenyeyGreenstein::probability(const Vector &in, const Vector &out) const
{
  const float cosTheta = in.dot(out);
  return k_isotropic * (1.0f - m_g * m_g) / 
    std::pow(1.0f + m_g * m_g - 2.0f * m_g * cosTheta, 1.5f);
}

//----------------------------------------------------------------------------//
// DoubleHenyeyGreenstein
//----------------------------------------------------------------------------//

DoubleHenyeyGreenstein::DoubleHenyeyGreenstein(float g1, float g2, float blend)
  : m_g1(Imath::clamp(g1, -1.0f, 1.0f)),
    m_g2(Imath::clamp(g2, -1.0f, 1.0f)),
    m_blend(Imath::clamp(blend, 0.0f, 1.0f))
{ 

}

//----------------------------------------------------------------------------//

float DoubleHenyeyGreenstein::probability(const Vector &in, 
                                          const Vector &out) const
{
  const float cosTheta = in.dot(out);
  float p1 = k_isotropic * (1.0f - m_g1 * m_g1) / 
    std::pow(1.0f + m_g1 * m_g1 - 2.0f * m_g1 * cosTheta, 1.5f);
  float p2 = k_isotropic * (1.0f - m_g2 * m_g2) / 
    std::pow(1.0f + m_g2 * m_g2 - 2.0f * m_g2 * cosTheta, 1.5f);
  return Math::fit01(m_blend, p2, p1);
}

//----------------------------------------------------------------------------//

} // namespace Phase
} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
