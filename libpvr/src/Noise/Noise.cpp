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
// NoiseFunction
//----------------------------------------------------------------------------//

float NoiseFunction::eval(const Imath::V3f &p) const
{
  return eval(p.x, p.y, p.z);
}

Imath::V3f NoiseFunction::evalVec(const Imath::V3f &p) const
{
  return evalVec(p.x, p.y, p.z);
}

//----------------------------------------------------------------------------//
// PerlinNoise
//----------------------------------------------------------------------------//
  
float PerlinNoise::eval(const float x) const
{
  float result;
  m_noise(result, x);
  return result;
}

//----------------------------------------------------------------------------//
  
float PerlinNoise::eval(const float x, const float y) const
{
  float result;
  m_noise(result, x, y);
  return result;
}

//----------------------------------------------------------------------------//
  
float PerlinNoise::eval(const float x, const float y, const float z) const
{
  float result;
  m_noise(result, Imath::V3f(x, y, z));
  return result;
}

//----------------------------------------------------------------------------//
  
Imath::V3f PerlinNoise::evalVec(const float x) const
{
  Imath::V3f result;
  m_noise(result, x);
  return result;
}

//----------------------------------------------------------------------------//
  
Imath::V3f PerlinNoise::evalVec(const float x, const float y) const
{
  Imath::V3f result;
  m_noise(result, x, y);
  return result;
}

//----------------------------------------------------------------------------//
  
Imath::V3f PerlinNoise::evalVec(const float x, const float y, 
                                const float z) const
{
  Imath::V3f result;
  m_noise(result, Imath::V3f(x, y, z));
  return result;
}

//----------------------------------------------------------------------------//
  
NoiseFunction::Range PerlinNoise::range() const
{
  return std::make_pair(-1.0, 1.0f);
}

//----------------------------------------------------------------------------//
// AbsPerlinNoise
//----------------------------------------------------------------------------//

float AbsPerlinNoise::eval(const float x) const
{
  float result;
  m_noise(result, x);
  return std::abs(result);
}

//----------------------------------------------------------------------------//

float AbsPerlinNoise::eval(const float x, const float y) const
{
  float result;
  m_noise(result, x, y);
  return std::abs(result);
}

//----------------------------------------------------------------------------//

float AbsPerlinNoise::eval(const float x, const float y, const float z) const
{
  float result;
  m_noise(result, Imath::V3f(x, y, z));
  return std::abs(result);
}

//----------------------------------------------------------------------------//

Imath::V3f AbsPerlinNoise::evalVec(const float x) const
{
  Imath::V3f result;
  m_noise(result, x);
  return Math::abs(result);
}

//----------------------------------------------------------------------------//

Imath::V3f AbsPerlinNoise::evalVec(const float x, const float y) const
{
  Imath::V3f result;
  m_noise(result, x, y);
  return Math::abs(result);
}

//----------------------------------------------------------------------------//

Imath::V3f AbsPerlinNoise::evalVec(const float x, const float y, const float z) const
{
  Imath::V3f result;
  m_noise(result, Imath::V3f(x, y, z));
  return Math::abs(result);
}

//----------------------------------------------------------------------------//

NoiseFunction::Range AbsPerlinNoise::range() const
{
  return std::make_pair(0.0f, 1.0f);
}

//----------------------------------------------------------------------------//
// Fractal
//----------------------------------------------------------------------------//

float Fractal::eval(const float x) const
{ 
  return eval(Imath::V3f(x, 0.0, 0.0)); 
}

//----------------------------------------------------------------------------//

float Fractal::eval(const float x, const float y) const
{ 
  return eval(Imath::V3f(x, y, 0.0)); 
}

//----------------------------------------------------------------------------//

float Fractal::eval(const float x, const float y, const float z) const
{ 
  return eval(Imath::V3f(x, y, z)); 
}

//----------------------------------------------------------------------------//

Imath::V3f Fractal::evalVec(const float x) const
{ 
  return evalVec(Imath::V3f(x, 0.0, 0.0)); 
}

//----------------------------------------------------------------------------//

Imath::V3f Fractal::evalVec(const float x, const float y) const
{ 
  return evalVec(Imath::V3f(x, y, 0.0)); 
}

//----------------------------------------------------------------------------//

Imath::V3f Fractal::evalVec(const float x, const float y, const float z) const
{ 
  return evalVec(Imath::V3f(x, y, z)); 
}

//----------------------------------------------------------------------------//
// fBm
//----------------------------------------------------------------------------//

fBm::fBm(NoiseFunction::CPtr noise, float scale, float octaves, 
         float octaveGain, float lacunarity)
  : m_noise(noise), m_scale(scale), m_octaves(octaves), 
    m_octaveGain(octaveGain), m_lacunarity(lacunarity)
{
  
}

//----------------------------------------------------------------------------//

fBm::Ptr fBm::create(NoiseFunction::CPtr noise, float scale, float octaves, 
                     float octaveGain, float lacunarity)
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
