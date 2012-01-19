//----------------------------------------------------------------------------//

/*! \file Noise.cpp
  Contains implementations of 
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Noise/Noise.h"

// System includes



//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Noise {

//----------------------------------------------------------------------------//
// fBm
//----------------------------------------------------------------------------//

fBm::fBm(NoiseFunction::CPtr noise,
         float scale, float octaves, float octaveGain, float lacunarity)
  : m_noise(noise), m_scale(scale), m_octaves(octaves), 
    m_octaveGain(octaveGain), m_lacunarity(lacunarity)
{
  
}

//----------------------------------------------------------------------------//

fBm::Ptr fBm::create(NoiseFunction::CPtr noise,
                     float scale, float octaves, float octaveGain,
                     float lacunarity)
{ 
  return Ptr(new fBm(noise, scale, octaves, octaveGain, lacunarity)); 
}

//----------------------------------------------------------------------------//

float fBm::eval(const Imath::V3f &p) const
{
  // Scale the lookup point
  Imath::V3f noiseP(p / m_scale);
  // Initialize iteration variables
  float result = 0.0f;
  float octaveContribution = 1.0f;
  float octaves = m_octaves;
  // Loop over octaves
  for (; octaves > 1.0f; octaves -= 1.0f) {
    // Add in noise function
    result += m_noise->eval(noiseP) * octaveContribution;
    // Scale amplitude of next octave
    octaveContribution *= m_octaveGain;
    // Scale lookup points of next octave
    noiseP *= m_lacunarity;
  }
  // If there is a partial octave left, apply fraction of last octave
  if (octaves > 0.0f) {
    result += m_noise->eval(noiseP) * octaveContribution * octaves;
  }
  return result ;
}

//----------------------------------------------------------------------------//

Imath::V3f fBm::evalVec(const Imath::V3f &p) const
{
  // Scale the lookup point
  Imath::V3f noiseP(p / m_scale);
  // Initialize iteration variables
  Imath::V3f result(0.0f);
  float octaveContribution = 1.0f;
  float octaves = m_octaves;
  // Loop over octaves
  for (; octaves > 1.0f; octaves -= 1.0f) {
    // Add in noise function
    result += m_noise->evalVec(noiseP) * octaveContribution;
    // Scale amplitude of next octave
    octaveContribution *= m_octaveGain;
    // Scale lookup points of next octave
    noiseP *= m_lacunarity;
  }
  // If there is a partial octave left, apply fraction of last octave
  if (octaves > 0.0f) {
    result += m_noise->evalVec(noiseP) * octaveContribution * octaves;
  }
  return result ;
}

//----------------------------------------------------------------------------//

Fractal::Range fBm::range() const
{
  Range noiseRange = m_noise->range(), range = std::make_pair(0.0f, 0.0f);
  float octaveContribution = 1.0f;
  float octaves = m_octaves;
  // Loop over octaves
  for (; octaves > 1.0f; octaves -= 1.0f) {
    range.first += noiseRange.first * octaveContribution;
    range.second += noiseRange.second * octaveContribution;
    octaveContribution *= m_octaveGain;
  }
  if (octaves > 0.0f) {
    range.first += noiseRange.first * octaveContribution * octaves;
    range.second += noiseRange.second * octaveContribution * octaves;
  }
  return range;
}

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

float pyroclastic(const float distance, const float noise,
                  const float filterWidth)
{
  float width     = filterWidth * 0.5;
  float pyroValue = Math::fit(distance - noise, -width, width, 1.0, 0.0);
  return Imath::clamp(pyroValue, 0.0f, 1.0f);
}

//----------------------------------------------------------------------------//

} // namespace Noise
} // namespace pvr

//----------------------------------------------------------------------------//
