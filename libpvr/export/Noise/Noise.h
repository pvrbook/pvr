//-*-c++-*--------------------------------------------------------------------//

/*! \file Noise.h
  Contains the Noise base class
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_NOISE_H__
#define __INCLUDED_PVR_NOISE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Math.h"
#include "pvr/Types.h"
#include "pvr/Noise/NoiseImpl.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Noise {
 
//----------------------------------------------------------------------------//

//! Base class for noise functions
class NoiseFunction
{
public:
  DECLARE_SMART_PTRS(NoiseFunction);
  typedef std::pair<float, float> Range;
  virtual float eval(const float x) const = 0;
  virtual float eval(const float x, const float y) const = 0;
  virtual float eval(const float x, const float y, const float z) const = 0;
  virtual Imath::V3f evalVec(const float x) const = 0;
  virtual Imath::V3f evalVec(const float x, const float y) const = 0;
  virtual Imath::V3f evalVec(const float x, const float y, const float z) const = 0;
  float eval(const Imath::V3f &p) const
  {
    return eval(p.x, p.y, p.z);
  }
  Imath::V3f evalVec(const Imath::V3f &p) const
  {
    return evalVec(p.x, p.y, p.z);
  }
  virtual Range range() const = 0;
};

//----------------------------------------------------------------------------//

class PerlinNoise : public NoiseFunction
{
public:
  DECLARE_SMART_PTRS(PerlinNoise);
  DECLARE_CREATE_FUNC(PerlinNoise);
  virtual float eval(const float x) const
  {
    float result;
    m_noise(result, x);
    return result;
  }
  virtual float eval(const float x, const float y) const
  {
    float result;
    m_noise(result, x, y);
    return result;
  }
  virtual float eval(const float x, const float y, const float z) const
  {
    float result;
    m_noise(result, Imath::V3f(x, y, z));
    return result;
  }
  virtual Imath::V3f evalVec(const float x) const
  {
    Imath::V3f result;
    m_noise(result, x);
    return result;
  }
  virtual Imath::V3f evalVec(const float x, const float y) const
  {
    Imath::V3f result;
    m_noise(result, x, y);
    return result;
  }
  virtual Imath::V3f evalVec(const float x, const float y, const float z) const
  {
    Imath::V3f result;
    m_noise(result, Imath::V3f(x, y, z));
    return result;
  }
  virtual Range range() const
  {
    return std::make_pair(-1.0f, 1.0f);
  }
private:
  SNoise m_noise;
};

//----------------------------------------------------------------------------//

class AbsPerlinNoise : public NoiseFunction
{
public:
  DECLARE_SMART_PTRS(AbsPerlinNoise);
  DECLARE_CREATE_FUNC(AbsPerlinNoise);
  virtual float eval(const float x) const
  {
    float result;
    m_noise(result, x);
    return std::abs(result);
  }
  virtual float eval(const float x, const float y) const
  {
    float result;
    m_noise(result, x, y);
    return std::abs(result);
  }
  virtual float eval(const float x, const float y, const float z) const
  {
    float result;
    m_noise(result, Imath::V3f(x, y, z));
    return std::abs(result);
  }
  virtual Imath::V3f evalVec(const float x) const
  {
    Imath::V3f result;
    m_noise(result, x);
    return Math::abs(result);
  }
  virtual Imath::V3f evalVec(const float x, const float y) const
  {
    Imath::V3f result;
    m_noise(result, x, y);
    return Math::abs(result);
  }
  virtual Imath::V3f evalVec(const float x, const float y, const float z) const
  {
    Imath::V3f result;
    m_noise(result, Imath::V3f(x, y, z));
    return Math::abs(result);
  }
  virtual Range range() const
  {
    return std::make_pair(0.0f, 1.0f);
  }
private:
  SNoise m_noise;
};

//----------------------------------------------------------------------------//
// Fractal
//----------------------------------------------------------------------------//

class Fractal
{
public:
  DECLARE_SMART_PTRS(Fractal);
  typedef std::pair<float, float> Range;
  float eval(const float x) const
  { return eval(Imath::V3f(x, 0.0, 0.0)); }
  float eval(const float x, const float y) const
  { return eval(Imath::V3f(x, y, 0.0)); }
  float eval(const float x, const float y, const float z) const
  { return eval(Imath::V3f(x, y, z)); }
  Imath::V3f evalVec(const float x) const
  { return evalVec(Imath::V3f(x, 0.0, 0.0)); }
  Imath::V3f evalVec(const float x, const float y) const
  { return evalVec(Imath::V3f(x, y, 0.0)); }
  Imath::V3f evalVec(const float x, const float y, const float z) const
  { return evalVec(Imath::V3f(x, y, z)); }
  virtual float eval(const Imath::V3f &p) const = 0;
  virtual Imath::V3f evalVec(const Imath::V3f &p) const = 0;
  virtual Range range() const = 0;
};

//----------------------------------------------------------------------------//

//! \todo Change scale to V3f
class fBm : public Fractal
{
public:
  DECLARE_SMART_PTRS(fBm);
  fBm(NoiseFunction::CPtr noise,
      float scale, float octaves, float octaveGain, float lacunarity)
    : m_noise(noise), m_scale(scale), m_octaves(octaves), 
      m_octaveGain(octaveGain), m_lacunarity(lacunarity)
  {
    
  }
  static Ptr create(NoiseFunction::CPtr noise,
                    float scale, float octaves, float octaveGain,
                    float lacunarity)
  { return Ptr(new fBm(noise, scale, octaves, octaveGain, lacunarity)); }
  virtual float eval(const Imath::V3f &p) const
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
  virtual Imath::V3f evalVec(const Imath::V3f &p) const
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
  virtual Range range() const
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
private:
  NoiseFunction::CPtr m_noise;
  float m_scale;
  float m_octaves;
  float m_octaveGain;
  float m_lacunarity;
};

//----------------------------------------------------------------------------//

} // namespace Noise
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
