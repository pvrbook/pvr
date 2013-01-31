//-*-c++-*--------------------------------------------------------------------//

/*
    This file is part of PVR. Copyright (C) 2012 Magnus Wrenninge

    PVR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PVR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//----------------------------------------------------------------------------//

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

#include "pvr/export.h"
#include "pvr/Math.h"
#include "pvr/Types.h"
#include "pvr/Noise/NoiseImpl.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Noise {
 
//----------------------------------------------------------------------------//
// NoiseFunction
//----------------------------------------------------------------------------//

//! Base class for noise functions
class LIBPVR_PUBLIC NoiseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(NoiseFunction);
  typedef std::pair<float, float> Range;

  // To be implemented by subclasses -------------------------------------------

  //! Returns a scalar noise value
  virtual float      eval(const float x) const = 0;
  virtual float      eval(const float x, const float y) const = 0;
  virtual float      eval(const float x, const float y, 
                          const float z) const = 0;
  //! Returns a vector noise value
  virtual Imath::V3f evalVec(const float x) const = 0;
  virtual Imath::V3f evalVec(const float x, const float y) const = 0;
  virtual Imath::V3f evalVec(const float x, const float y, 
                             const float z) const = 0;
  //! Returns the range (min and max) of the noise function.
  virtual Range      range() const = 0;
  
  // Utility member functions --------------------------------------------------

  //! Evaluates scalar noise for a vector coordinate
  float              eval(const Imath::V3f &p) const;
  //! Evaluates vector noise for a vector coordinate
  Imath::V3f         evalVec(const Imath::V3f &p) const;

};

//----------------------------------------------------------------------------//
// PerlinNoise
//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC PerlinNoise : public NoiseFunction
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(PerlinNoise);

  // Ctor, factory function ----------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(PerlinNoise);

  // From NoiseFunction --------------------------------------------------------

  virtual float      eval(const float x) const;
  virtual float      eval(const float x, const float y) const;
  virtual float      eval(const float x, const float y, const float z) const;
  virtual Imath::V3f evalVec(const float x) const;
  virtual Imath::V3f evalVec(const float x, const float y) const;
  virtual Imath::V3f evalVec(const float x, const float y, const float z) const;
  virtual Range      range() const;

private:

  // Private data members ------------------------------------------------------

  //! SNoise instance to use for sampling noise.
  SNoise m_noise;

};

//----------------------------------------------------------------------------//
// AbsPerlinNoise
//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC AbsPerlinNoise : public NoiseFunction
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(AbsPerlinNoise);

  // Ctor, factory function ----------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(AbsPerlinNoise);

  // From NoiseFunction --------------------------------------------------------

  virtual float      eval(const float x) const;
  virtual float      eval(const float x, const float y) const;
  virtual float      eval(const float x, const float y, const float z) const;
  virtual Imath::V3f evalVec(const float x) const;
  virtual Imath::V3f evalVec(const float x, const float y) const;
  virtual Imath::V3f evalVec(const float x, const float y, const float z) const;
  virtual Range      range() const;

private:

  // Private data members ------------------------------------------------------

  //! SNoise instance to use for sampling noise.
  SNoise m_noise;
};

//----------------------------------------------------------------------------//
// Fractal
//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC Fractal
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Fractal);
  typedef std::pair<float, float> Range;

  // To be implemented by subclasses -------------------------------------------

  virtual float      eval(const Imath::V3f &p) const = 0;
  virtual Imath::V3f evalVec(const Imath::V3f &p) const = 0;
  virtual Range      range() const = 0;

  // Utility member functions --------------------------------------------------

  float      eval(const float x) const;
  float      eval(const float x, const float y) const;
  float      eval(const float x, const float y, const float z) const;
  Imath::V3f evalVec(const float x) const;
  Imath::V3f evalVec(const float x, const float y) const;
  Imath::V3f evalVec(const float x, const float y, const float z) const;

};

//----------------------------------------------------------------------------//
// fBm
//----------------------------------------------------------------------------//

//! \todo Change scale to V3f
class LIBPVR_PUBLIC fBm : public Fractal
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(fBm);

  // Ctor, factory function ----------------------------------------------------

  fBm(NoiseFunction::CPtr noise, float scale, float octaves, 
      float octaveGain, float lacunarity);
  static Ptr create(NoiseFunction::CPtr noise, float scale, float octaves, 
                    float octaveGain, float lacunarity);

  // From Fractal --------------------------------------------------------------

  virtual float      eval(const Imath::V3f &p) const;
  virtual Imath::V3f evalVec(const Imath::V3f &p) const;
  virtual Range      range() const;

private:

  // Private data members ------------------------------------------------------

  NoiseFunction::CPtr m_noise;
  float               m_scale;
  float               m_octaves;
  float               m_octaveGain;
  float               m_lacunarity;

};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

//! Performs antialiased pyroclastic noise function, given a distance function,
//! a noise value and a filter width. The filter width and distance function
//! should be in the same space.
float pyroclastic(const float distance, const float noise,
                  const float filterWidth);

//----------------------------------------------------------------------------//

} // namespace Noise
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
