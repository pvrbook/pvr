//-*-c++-*--------------------------------------------------------------------//

/* Copyright (c) 2011 Magnus Wrenninge
 *
 * All rights reserved.
 * For educational use only. All commercial use expressly forbidden.
 *
 * Redistributions of source code must retain the above copyright
 * notice, the list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//----------------------------------------------------------------------------//

/*! \file PyroclasticLine.h
  Contains the PyroclasticLine class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RASTERIZATION_PYROCLASTICLINE_H__
#define __INCLUDED_PVR_RASTERIZATOPM_PYROCLASTICLINE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/Primitives/Rasterization/LineBase.h"

#include "pvr/Noise/Noise.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//

Vector lineWsToNs(const Vector &wsP, const Vector &wsE1, const Vector &wsE2,
                  const Vector &wsE3, const Vector &wsCenter, 
                  const float u, const float radius);

//----------------------------------------------------------------------------//
// PyroclasticLine
//----------------------------------------------------------------------------//

/*! \class PyroclasticLine
  \brief Implements the rasterization line primitive described in chapter XX.

  See chapter XX for details.
*/

//----------------------------------------------------------------------------//

class PyroclasticLine : public LineBase
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(PyroclasticLine);

  // Factory -------------------------------------------------------------------

  //! Factory function
  static Ptr create();

  //! Returns the type name of the primitive
  PVR_DEFINE_TYPENAME(PyroclasticLine);

protected:

  // From RasterizationPrimitive -----------------------------------------------

  virtual void getSample(const RasterizationState &state,
                         RasterizationSample &sample) const;

  // From LineRasterizationPrimitive -------------------------------------------

  virtual void updatePolyAttrs(Geo::AttrVisitor::const_iterator i) const;
  virtual void updatePointAttrs(Geo::AttrVisitor::const_iterator i, 
                                const size_t numPoints) const;
  virtual float displacementBounds(const size_t index) const;

  // Structs -------------------------------------------------------------------

  struct PolyAttrState
  {
    PolyAttrState()
      : scale       ("scale",          Imath::V3f(1.0f)), 
        octaves     ("octaves",        8.0f), 
        octaveGain  ("octave_gain",    0.5f), 
        lacunarity  ("lacunarity",     1.92f), 
        absNoise    ("absolute_noise", 1),
        pyroclastic ("pyroclastic",    1),
        pyro2D      ("pyroclastic_2d", 1)
    { }
    
    void update(const Geo::AttrVisitor::const_iterator &i);

    Geo::Attr<Imath::V3f> scale;
    Geo::Attr<float>      octaves;
    Geo::Attr<float>      octaveGain;
    Geo::Attr<float>      lacunarity;
    Geo::Attr<int>        absNoise;
    Geo::Attr<int>        pyroclastic;
    Geo::Attr<int>        pyro2D;

    Noise::Fractal::CPtr  fractal;
  };

  struct PointAttrState
  {
    PointAttrState()
      : wsNormal  ("N",           Imath::V3f(0.0, 1.0, 0.0)), 
        wsTangent ("T",           Imath::V3f(0.0, 0.0, 1.0)), 
        u         ("u",           0.0f), 
        amplitude ("amplitude",   1.0f), 
        gamma     ("gamma",       1.0f)
    { }
    
    void update(const Geo::AttrVisitor::const_iterator &i);

    Geo::Attr<Imath::V3f> wsNormal;
    Geo::Attr<Imath::V3f> wsTangent;
    Geo::Attr<float>      u;
    Geo::Attr<float>      amplitude;
    Geo::Attr<float>      gamma;
  };

  // Data members --------------------------------------------------------------

  //! Holds the per-point attributes specific to this class
  mutable std::vector<PointAttrState> m_pointAttrs;
  //! Holds the per-poly attributes specific to this class
  mutable PolyAttrState m_polyAttrs;

};

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
