//-*-c++-*--------------------------------------------------------------------//

/*
Copyright (c) 2009-2010 Sony Pictures Imageworks Inc., et al.
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of Sony Pictures Imageworks nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//----------------------------------------------------------------------------//

/*! \file NoiseImpl.h
  Contains noise functions from OSL library.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_NOISEIMPL_H__
#define __INCLUDED_PVR_NOISEIMPL_H__

//----------------------------------------------------------------------------//

#include <limits>

#include "pvr/Types.h"

//----------------------------------------------------------------------------//

namespace pvr {
namespace Noise {

namespace {

  //! return the greatest integer <= x
  inline int quick_floor (float x) {
    return (int) x - ((x < 0) ? 1 : 0);
  }

  //! convert a 32 bit integer into a floating point number in [0,1]
  inline float bits_to_01 (unsigned int bits) {
    // divide by 2^32-1
    return bits * (1.0f / std::numeric_limits<unsigned int>::max());
  }

  //! hash an array of N 32 bit values into a pseudo-random value
  //! based on my favorite hash: http://burtleburtle.net/bob/c/lookup3.c
  //! templated so that the compiler can unroll the loops for us
  template <int N>
  inline unsigned int
  inthash (const unsigned int k[N]) {
    // define some handy macros
#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))
#define mix(a,b,c)                              \
    {                                           \
      a -= c;  a ^= rot(c, 4);  c += b;         \
      b -= a;  b ^= rot(a, 6);  a += c;         \
      c -= b;  c ^= rot(b, 8);  b += a;         \
      a -= c;  a ^= rot(c,16);  c += b;         \
      b -= a;  b ^= rot(a,19);  a += c;         \
      c -= b;  c ^= rot(b, 4);  b += a;         \
    }
#define final(a,b,c)                            \
    {                                           \
      c ^= b; c -= rot(b,14);                   \
      a ^= c; a -= rot(c,11);                   \
      b ^= a; b -= rot(a,25);                   \
      c ^= b; c -= rot(b,16);                   \
      a ^= c; a -= rot(c,4);                    \
      b ^= a; b -= rot(a,14);                   \
      c ^= b; c -= rot(b,24);                   \
    }
    // now hash the data!
    unsigned int a, b, c, len = N;
    a = b = c = 0xdeadbeef + (len << 2) + 13;
    while (len > 3) {
      a += k[0];
      b += k[1];
      c += k[2];
      mix(a, b, c);
      len -= 3;
      k += 3;
    }
    switch (len) {
    case 3 : c += k[2];
    case 2 : b += k[1];
    case 1 : a += k[0];
      final(a, b, c);
    case 0:
      break;
    }
    return c;
    // macros not needed anymore
#undef rot
#undef mix
#undef final
  }

} // Anonymous namespace
  
struct CellNoise 
{
  
  CellNoise () { }
    
  inline void operator() (float &result, float x) {
    unsigned int iv[1];
    iv[0] = quick_floor (x);
    hash1<1> (result, iv);
  }
    
  inline void operator() (float &result, float x, float y) {
    unsigned int iv[2];
    iv[0] = quick_floor (x);
    iv[1] = quick_floor (y);
    hash1<2> (result, iv);
  }
    
  inline void operator() (float &result, const Imath::V3f &p) {
    unsigned int iv[3];
    iv[0] = quick_floor (p.x);
    iv[1] = quick_floor (p.y);
    iv[2] = quick_floor (p.z);
    hash1<3> (result, iv);
  }
    
  inline void operator() (float &result, const Imath::V3f &p, float t) {
    unsigned int iv[4];
    iv[0] = quick_floor (p.x);
    iv[1] = quick_floor (p.y);
    iv[2] = quick_floor (p.z);
    iv[3] = quick_floor (t);
    hash1<4> (result, iv);
  }
    
  inline void operator() (Imath::V3f &result, float x) {
    unsigned int iv[2];
    iv[0] = quick_floor (x);
    hash3<2> (result, iv);
  }
    
  inline void operator() (Imath::V3f &result, float x, float y) {
    unsigned int iv[3];
    iv[0] = quick_floor (x);
    iv[1] = quick_floor (y);
    hash3<3> (result, iv);
  }
    
  inline void operator() (Imath::V3f &result, const Imath::V3f &p) {
    unsigned int iv[4];
    iv[0] = quick_floor (p.x);
    iv[1] = quick_floor (p.y);
    iv[2] = quick_floor (p.z);
    hash3<4> (result, iv);
  }
    
  inline void operator() (Imath::V3f &result, const Imath::V3f &p, float t) {
    unsigned int iv[5];
    iv[0] = quick_floor (p.x);
    iv[1] = quick_floor (p.y);
    iv[2] = quick_floor (p.z);
    iv[3] = quick_floor (t);
    hash3<5> (result, iv);
  }
    
private:
  template <int N>
  inline void hash1 (float &result, const unsigned int k[N]) {
    result = bits_to_01(inthash<N>(k));
  }

  template <int N>
  inline void hash3 (Imath::V3f &result, unsigned int k[N]) {
    k[N-1] = 0; result.x = bits_to_01 (inthash<N> (k));
    k[N-1] = 1; result.y = bits_to_01 (inthash<N> (k));
    k[N-1] = 2; result.z = bits_to_01 (inthash<N> (k));
  }
};

namespace {

  // helper functions for perlin noise 

  // always return a value inside [0,b) - even for negative numbers
  inline int imod(int a, int b) {
    a %= b;
    return a < 0 ? a + b : a;
  }

  // floorfrac return quick_floor as well as the fractional remainder
  inline float floorfrac(float x, int* i) {
    *i = quick_floor(x);
    return x - *i;
  }

  template <typename T>
  inline T fade (const T &t) { 
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f); 
  }


  // lerp implementation, a few obstacles prevent us from using Imath::lerp():
  //   * Original perlin code was written assuming lerp(t,a,b) which is different
  //     from Imath::lerp(a,b,t)
  //   * Imath version uses 1-t internally which doesn't work for duals. We need
  //     to use (1.0f-t) for the templated operator-() to get chosen
  //   * This call recieves some arguments which are not handled trivially by
  //     templated overloads such as lerp(Dual2<float>, Imath::V3f, Imath::V3f)

  float lerp(float t, float a, float b) {
    return (1.0f - t) * a + t * b;
  }

  Imath::V3f lerp(float t, Imath::V3f a, Imath::V3f b) {
    return (1.0f - t) * a + t * b;
  }

  // 1,2,3 and 4 dimensional gradient functions - perform a dot product against a
  // randomly chosen vector. Note that the gradient vector is not normalized, but
  // this only affects the overall "scale" of the result, so we simply account for
  // the scale by multiplying in the corresponding "perlin" function.
  // These factors were experimentally calculated to be:
  //    1D:   0.188
  //    2D:   0.507
  //    3D:   0.936
  //    4D:   0.870

  template <typename T>
  inline T grad (int hash, const T &x) {
    int h = hash & 15;
    float g = 1 + (h & 7);  // 1, 2, .., 8
    if (h&8) g = -g;        // random sign
    return g * x;           // dot-product
  }

  template <typename T>
  inline T grad (int hash, const T &x, const T &y) {
    // 8 possible directions (+-1,+-2) and (+-2,+-1)
    int h = hash & 7;
    T u = h<4 ? x : y;
    T v = h<4 ? y : x;
    // compute the dot product with (x,y).
    return ((h&1) ? -u : u) + ((h&2) ? -2.0f * v : 2.0f * v);
  }

  template <typename T>
  inline T grad (int hash, const T &x, const T &y, const T &z) {
    // use vectors pointing to the edges of the cube
    int h = hash & 15;
    T u = h<8 ? x : y;
    T v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) ? -u : u) + ((h&2) ? -v : v);
  }

  template <typename T>
  inline T grad (int hash, const T &x, const T &y, const T &z, const T &w) {
    // use vectors pointing to the edges of the hypercube
    int h = hash & 31;
    T u = h<24 ? x : y;
    T v = h<16 ? y : z;
    T s = h<8  ? z : w;
    return ((h&1)? -u : u) + ((h&2)? -v : v) + ((h&4)? -s : s);
  }

  inline Imath::V3f grad (const Imath::V3i &hash, float x) {
    return Imath::V3f (grad (hash.x, x),
                 grad (hash.y, x),
                 grad (hash.z, x));
  }

  inline Imath::V3f grad (const Imath::V3i &hash, float x, float y) {
    return Imath::V3f (grad (hash.x, x, y),
                 grad (hash.y, x, y),
                 grad (hash.z, x, y));
  }

  inline Imath::V3f grad (const Imath::V3i &hash, float x, float y, float z) {
    return Imath::V3f (grad (hash.x, x, y, z),
                 grad (hash.y, x, y, z),
                 grad (hash.z, x, y, z));
  }

  inline Imath::V3f grad (const Imath::V3i &hash, float x, float y, float z, float w) {
    return Imath::V3f (grad (hash.x, x, y, z, w),
                 grad (hash.y, x, y, z, w),
                 grad (hash.z, x, y, z, w));
  }

  template <typename T>
  inline T scale1 (const T &result) { return 0.2500f * result; }
  template <typename T>
  inline T scale2 (const T &result) { return 0.6616f * result; }
  template <typename T>
  inline T scale3 (const T &result) { return 0.9820f * result; }
  template <typename T>
  inline T scale4 (const T &result) { return 0.8344f * result; }

} // Anonymous namespace

template <typename V, typename H, typename T>
inline void perlin (V& result, H& hash, const T &x) {
  int X; T fx = floorfrac(x, &X);
  T u = fade(fx);
  
  result = lerp (u, grad (hash (X  ), fx     ),
                 grad (hash (X+1), fx-1.0f));
  result = scale1 (result);
}
  
template <typename V, typename H, typename T>
inline void perlin (V &result, const H &hash, const T &x, const T &y) {
  int X; T fx = floorfrac(x, &X);
  int Y; T fy = floorfrac(y, &Y);
  
  T u = fade(fx);
  T v = fade(fy);
  
  result = lerp (v, lerp (u, grad (hash (X  , Y  ), fx     , fy     ),
                          grad (hash (X+1, Y  ), fx-1.0f, fy     )),
                 lerp (u, grad (hash (X  , Y+1), fx     , fy-1.0f),
                       grad (hash (X+1, Y+1), fx-1.0f, fy-1.0f)));
  result = scale2 (result);
}
  
template <typename V, typename H, typename T>
inline void perlin (V &result, const H &hash, const T &x, const T &y, const T &z) {
  int X; T fx = floorfrac(x, &X);
  int Y; T fy = floorfrac(y, &Y);
  int Z; T fz = floorfrac(z, &Z);
  
  T u = fade(fx);
  T v = fade(fy);
  T w = fade(fz);
  
  result = lerp (w, lerp (v, lerp (u, grad (hash (X  , Y  , Z  ), fx     , fy     , fz      ),
                                   grad (hash (X+1, Y  , Z  ), fx-1.0f, fy     , fz      )),
                          lerp (u, grad (hash (X  , Y+1, Z  ), fx     , fy-1.0f, fz      ),
                                grad (hash (X+1, Y+1, Z  ), fx-1.0f, fy-1.0f, fz      ))),
                 lerp (v, lerp (u, grad (hash (X  , Y  , Z+1), fx     , fy     , fz-1.0f ),
                                grad (hash (X+1, Y  , Z+1), fx-1.0f, fy     , fz-1.0f )),
                       lerp (u, grad (hash (X  , Y+1, Z+1), fx     , fy-1.0f, fz-1.0f ),
                             grad (hash (X+1, Y+1, Z+1), fx-1.0f, fy-1.0f, fz-1.0f ))));
  result = scale3 (result);
}
  
template <typename V, typename H, typename T>
inline void perlin (V &result, const H &hash, 
                    const T &x, const T &y, const T &z, 
                    const T &w) {
  int X; T fx = floorfrac(x, &X);
  int Y; T fy = floorfrac(y, &Y);
  int Z; T fz = floorfrac(z, &Z);
  int W; T fw = floorfrac(w, &W);

  T u = fade(fx);
  T v = fade(fy);
  T t = fade(fz);
  T s = fade(fw);

  result = lerp (s, lerp (t, lerp (v, lerp (u, grad (hash (X  , Y  , Z  , W  ), fx     , fy     , fz     , fw     ),
                                            grad (hash (X+1, Y  , Z  , W  ), fx-1.0f, fy     , fz     , fw     )),
                                   lerp (u, grad (hash (X  , Y+1, Z  , W  ), fx     , fy-1.0f, fz     , fw     ),
                                         grad (hash (X+1, Y+1, Z  , W  ), fx-1.0f, fy-1.0f, fz     , fw     ))),
                          lerp (v, lerp (u, grad (hash (X  , Y  , Z+1, W  ), fx     , fy     , fz-1.0f, fw     ),
                                         grad (hash (X+1, Y  , Z+1, W  ), fx-1.0f, fy     , fz-1.0f, fw     )),
                                lerp (u, grad (hash (X  , Y+1, Z+1, W  ), fx     , fy-1.0f, fz-1.0f, fw     ),
                                      grad (hash (X+1, Y+1, Z+1, W  ), fx-1.0f, fy-1.0f, fz-1.0f, fw     )))),
                 lerp (t, lerp (v, lerp (u, grad (hash (X  , Y  , Z  , W+1), fx     , fy     , fz     , fw-1.0f),
                                         grad (hash (X+1, Y  , Z  , W+1), fx-1.0f, fy     , fz     , fw-1.0f)),
                                lerp (u, grad (hash (X  , Y+1, Z  , W+1), fx     , fy-1.0f, fz     , fw-1.0f),
                                      grad (hash (X+1, Y+1, Z  , W+1), fx-1.0f, fy-1.0f, fz     , fw-1.0f))),
                       lerp (v, lerp (u, grad (hash (X  , Y  , Z+1, W+1), fx     , fy     , fz-1.0f, fw-1.0f),
                                      grad (hash (X+1, Y  , Z+1, W+1), fx-1.0f, fy     , fz-1.0f, fw-1.0f)),
                             lerp (u, grad (hash (X  , Y+1, Z+1, W+1), fx     , fy-1.0f, fz-1.0f, fw-1.0f),
                                   grad (hash (X+1, Y+1, Z+1, W+1), fx-1.0f, fy-1.0f, fz-1.0f, fw-1.0f)))));
  result = scale4 (result);
}

namespace {

  struct HashScalar 
  {
    int operator() (int x) const {
      unsigned int iv[1];
      iv[0] = x;
      return inthash<1> (iv);
    }

    int operator() (int x, int y) const {
      unsigned int iv[2];
      iv[0] = x;
      iv[1] = y;
      return inthash<2> (iv);
    }

    int operator() (int x, int y, int z) const {
      unsigned int iv[3];
      iv[0] = x;
      iv[1] = y;
      iv[2] = z;
      return inthash<3> (iv);
    }

    int operator() (int x, int y, int z, int w) const {
      unsigned int iv[4];
      iv[0] = x;
      iv[1] = y;
      iv[2] = z;
      iv[3] = w;
      return inthash<4> (iv);
    }
  };

  struct HashVector 
  {
    Imath::V3i operator() (int x) const {
      unsigned int iv[1];
      iv[0] = x;
      return hash3<1> (iv);
    }

    Imath::V3i operator() (int x, int y) const {
      unsigned int iv[2];
      iv[0] = x;
      iv[1] = y;
      return hash3<2> (iv);
    }

    Imath::V3i operator() (int x, int y, int z) const {
      unsigned int iv[3];
      iv[0] = x;
      iv[1] = y;
      iv[2] = z;
      return hash3<3> (iv);
    }

    Imath::V3i operator() (int x, int y, int z, int w) const {
      unsigned int iv[4];
      iv[0] = x;
      iv[1] = y;
      iv[2] = z;
      iv[3] = w;
      return hash3<4> (iv);
    }

    template <int N>
    Imath::V3i hash3 (unsigned int k[N]) const {
      Imath::V3i result;
      unsigned int h = inthash<N> (k);
      // we only need the low-order bits to be random, so split out
      // the 32 bit result into 3 parts for each channel
      result.x = (h      ) & 0xFF;  
      result.y = (h >> 8 ) & 0xFF;
      result.z = (h >> 16) & 0xFF;
      return result;
    }
  };

  struct HashScalarPeriodic 
  {
    HashScalarPeriodic (float px) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
    }
    HashScalarPeriodic (float px, float py) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
      m_py = quick_floor(py); if (m_py < 1) m_py = 1;
    }
    HashScalarPeriodic (float px, float py, float pz) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
      m_py = quick_floor(py); if (m_py < 1) m_py = 1;
      m_pz = quick_floor(pz); if (m_pz < 1) m_pz = 1;
    }
    HashScalarPeriodic (float px, float py, float pz, float pw) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
      m_py = quick_floor(py); if (m_py < 1) m_py = 1;
      m_pz = quick_floor(pz); if (m_pz < 1) m_pz = 1;
      m_pw = quick_floor(pw); if (m_pw < 1) m_pw = 1;
    }

    int m_px, m_py, m_pz, m_pw;

    int operator() (int x) const {
      unsigned int iv[1];
      iv[0] = imod (x, m_px);
      return inthash<1> (iv);
    }

    int operator() (int x, int y) const {
      unsigned int iv[2];
      iv[0] = imod (x, m_px);
      iv[1] = imod (y, m_py);
      return inthash<2> (iv);
    }

    int operator() (int x, int y, int z) const {
      unsigned int iv[3];
      iv[0] = imod (x, m_px);
      iv[1] = imod (y, m_py);
      iv[2] = imod (z, m_pz);
      return inthash<3> (iv);
    }

    int operator() (int x, int y, int z, int w) const {
      unsigned int iv[4];
      iv[0] = imod (x, m_px);
      iv[1] = imod (y, m_py);
      iv[2] = imod (z, m_pz);
      iv[3] = imod (w, m_pw);
      return inthash<4> (iv);
    }
  };

  struct HashVectorPeriodic 
  {
    HashVectorPeriodic (float px) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
    }
    HashVectorPeriodic (float px, float py) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
      m_py = quick_floor(py); if (m_py < 1) m_py = 1;
    }
    HashVectorPeriodic (float px, float py, float pz) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
      m_py = quick_floor(py); if (m_py < 1) m_py = 1;
      m_pz = quick_floor(pz); if (m_pz < 1) m_pz = 1;
    }
    HashVectorPeriodic (float px, float py, float pz, float pw) {
      m_px = quick_floor(px); if (m_px < 1) m_px = 1;
      m_py = quick_floor(py); if (m_py < 1) m_py = 1;
      m_pz = quick_floor(pz); if (m_pz < 1) m_pz = 1;
      m_pw = quick_floor(pw); if (m_pw < 1) m_pw = 1;
    }

    int m_px, m_py, m_pz, m_pw;

    Imath::V3i operator() (int x) const {
      unsigned int iv[1];
      iv[0] = imod (x, m_px);
      return hash3<1> (iv);
    }

    Imath::V3i operator() (int x, int y) const {
      unsigned int iv[2];
      iv[0] = imod (x, m_px);
      iv[1] = imod (y, m_py);
      return hash3<2> (iv);
    }

    Imath::V3i operator() (int x, int y, int z) const {
      unsigned int iv[3];
      iv[0] = imod (x, m_px);
      iv[1] = imod (y, m_py);
      iv[2] = imod (z, m_pz);
      return hash3<3> (iv);

    }

    Imath::V3i operator() (int x, int y, int z, int w) const {
      unsigned int iv[4];
      iv[0] = imod (x, m_px);
      iv[1] = imod (y, m_py);
      iv[2] = imod (z, m_pz);
      iv[3] = imod (w, m_pw);
      return hash3<4> (iv);
    }

    template <int N>
    Imath::V3i hash3 (unsigned int k[N]) const {
      Imath::V3i result;
      unsigned int h = inthash<N> (k);
      // we only need the low-order bits to be random, so split out
      // the 32 bit result into 3 parts for each channel
      result.x = (h      ) & 0xFF;  
      result.y = (h >> 8 ) & 0xFF;
      result.z = (h >> 16) & 0xFF;
      return result;
    }
  };

} // Anonymous namespace

struct Noise 
{
  Noise () { }
  
  inline void operator() (float &result, float x) const {
    HashScalar h;
    perlin(result, h, x);
    result = 0.5f * (result + 1);
  }

  inline void operator() (float &result, float x, float y) const {
    HashScalar h;
    perlin(result, h, x, y);
    result = 0.5f * (result + 1);
  }

  inline void operator() (float &result, const Imath::V3f &p) const {
    HashScalar h;
    perlin(result, h, p.x, p.y, p.z);
    result = 0.5f * (result + 1);
  }

  inline void operator() (float &result, const Imath::V3f &p, float t) const {
    HashScalar h;
    perlin(result, h, p.x, p.y, p.z, t);
    result = 0.5f * (result + 1);
  }

  inline void operator() (Imath::V3f &result, float x) const {
    HashVector h;
    perlin(result, h, x);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

  inline void operator() (Imath::V3f &result, float x, float y) const {
    HashVector h;
    perlin(result, h, x, y);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p) const {
    HashVector h;
    perlin(result, h, p.x, p.y, p.z);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p, float t) const {
    HashVector h;
    perlin(result, h, p.x, p.y, p.z, t);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

};

struct SNoise 
{
  
  SNoise () { }

  inline void operator() (float &result, float x) const {
    HashScalar h;
    perlin(result, h, x);
  }

  inline void operator() (float &result, float x, float y) const {
    HashScalar h;
    perlin(result, h, x, y);
  }

  inline void operator() (float &result, const Imath::V3f &p) const {
    HashScalar h;
    perlin(result, h, p.x, p.y, p.z);
  }

  inline void operator() (float &result, const Imath::V3f &p, float t) const {
    HashScalar h;
    perlin(result, h, p.x, p.y, p.z, t);
  }

  inline void operator() (Imath::V3f &result, float x) const {
    HashVector h;
    perlin(result, h, x);
  }

  inline void operator() (Imath::V3f &result, float x, float y) const {
    HashVector h;
    perlin(result, h, x, y);
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p) const {
    HashVector h;
    perlin(result, h, p.x, p.y, p.z);
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p, float t) const {
    HashVector h;
    perlin(result, h, p.x, p.y, p.z, t);
  }

};

struct PeriodicNoise 
{

  PeriodicNoise () { }

  inline void operator() (float &result, float x, float px) const {
    HashScalarPeriodic h(px);
    perlin(result, h, x);
    result = 0.5f * (result + 1);
  }

  inline void operator() (float &result, float x, float y, float px, float py) const {
    HashScalarPeriodic h(px, py);
    perlin(result, h, x, y);
    result = 0.5f * (result + 1);
  }

  inline void operator() (float &result, const Imath::V3f &p, const Imath::V3f &pp) const {
    HashScalarPeriodic h(pp.x, pp.y, pp.z);
    perlin(result, h, p.x, p.y, p.z);
    result = 0.5f * (result + 1);
  }

  inline void operator() (float &result, const Imath::V3f &p, float t, const Imath::V3f &pp, float pt) const {
    HashScalarPeriodic h(pp.x, pp.y, pp.z, pt);
    perlin(result, h, p.x, p.y, p.z, t);
    result = 0.5f * (result + 1);
  }

  inline void operator() (Imath::V3f &result, float x, float px) const {
    HashVectorPeriodic h(px);
    perlin(result, h, x);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

  inline void operator() (Imath::V3f &result, float x, float y, float px, float py) const {
    HashVectorPeriodic h(px, py);
    perlin(result, h, x, y);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p, const Imath::V3f &pp) const {
    HashVectorPeriodic h(pp.x, pp.y, pp.z);
    perlin(result, h, p.x, p.y, p.z);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p, float t, const Imath::V3f &pp, float pt) const {
    HashVectorPeriodic h(pp.x, pp.y, pp.z, pt);
    perlin(result, h, p.x, p.y, p.z, t);
    result = 0.5f * (result + Imath::V3f(1, 1, 1));
  }

};

struct PeriodicSNoise 
{

  PeriodicSNoise () { }

  inline void operator() (float &result, float x, float px) const {
    HashScalarPeriodic h(px);
    perlin(result, h, x);
  }

  inline void operator() (float &result, float x, float y, float px, float py) const {
    HashScalarPeriodic h(px, py);
    perlin(result, h, x, y);
  }

  inline void operator() (float &result, const Imath::V3f &p, const Imath::V3f &pp) const {
    HashScalarPeriodic h(pp.x, pp.y, pp.z);
    perlin(result, h, p.x, p.y, p.z);
  }

  inline void operator() (float &result, const Imath::V3f &p, float t, const Imath::V3f &pp, float pt) const {
    HashScalarPeriodic h(pp.x, pp.y, pp.z, pt);
    perlin(result, h, p.x, p.y, p.z, t);
  }

  inline void operator() (Imath::V3f &result, float x, float px) const {
    HashVectorPeriodic h(px);
    perlin(result, h, x);
  }

  inline void operator() (Imath::V3f &result, float x, float y, float px, float py) const {
    HashVectorPeriodic h(px, py);
    perlin(result, h, x, y);
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p, const Imath::V3f &pp) const {
    HashVectorPeriodic h(pp.x, pp.y, pp.z);
    perlin(result, h, p.x, p.y, p.z);
  }

  inline void operator() (Imath::V3f &result, const Imath::V3f &p, float t, const Imath::V3f &pp, float pt) const {
    HashVectorPeriodic h(pp.x, pp.y, pp.z, pt);
    perlin(result, h, p.x, p.y, p.z, t);
  }

};

}; // namespace Noise
}; // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
