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
  PVR_TYPEDEF_SMART_PTRS(NoiseFunction);
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
  PVR_TYPEDEF_SMART_PTRS(PerlinNoise);
  PVR_DEFINE_CREATE_FUNC(PerlinNoise);
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
  PVR_TYPEDEF_SMART_PTRS(AbsPerlinNoise);
  PVR_DEFINE_CREATE_FUNC(AbsPerlinNoise);
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
  PVR_TYPEDEF_SMART_PTRS(Fractal);
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
  PVR_TYPEDEF_SMART_PTRS(fBm);
  fBm(NoiseFunction::CPtr noise,
      float scale, float octaves, float octaveGain, float lacunarity);
  static Ptr create(NoiseFunction::CPtr noise,
                    float scale, float octaves, float octaveGain,
                    float lacunarity);
  virtual float eval(const Imath::V3f &p) const;
  virtual Imath::V3f evalVec(const Imath::V3f &p) const;
  virtual Range range() const;
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
