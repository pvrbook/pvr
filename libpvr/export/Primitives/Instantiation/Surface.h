//-*-c++-*--------------------------------------------------------------------//

/*! \file Surface.h
  Contains the Inst::Surface class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INSTANTIATION_SURFACE_H__
#define __INCLUDED_PVR_INSTANTIATION_SURFACE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

// Project headers

#include "pvr/AttrUtil.h"
#include "pvr/ModelerInput.h"
#include "pvr/Primitives/InstantiationPrim.h"
#include "pvr/Types.h"
#include "pvr/Noise/Noise.h"

//----------------------------------------------------------------------------//
// Defines
//----------------------------------------------------------------------------//

//! Interpolates an attribute in m_pointAttrs.
//! \param variable Name of variable
//! \param s In range [0, numCols - 1]
//! \param t In range [0, numRows - 1]
#define SURFACE_INST_INTERP(variable, s, t)                                \
  Math::linear2D(s - std::floor(s), t - std::floor(t),                     \
    m_pointAttrs[pointIdx(s, t, numCols, numRows, 0, 0)].variable.value(), \
    m_pointAttrs[pointIdx(s, t, numCols, numRows, 1, 0)].variable.value(), \
    m_pointAttrs[pointIdx(s, t, numCols, numRows, 0, 1)].variable.value(), \
    m_pointAttrs[pointIdx(s, t, numCols, numRows, 1, 1)].variable.value()) \
  
//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Inst {

//----------------------------------------------------------------------------//
// Surface
//----------------------------------------------------------------------------//

/*! \class Surface
  \brief Implements the surface-based instantiation primitive.

  Instantiation-based primitives are covered in chapter XX.
 */

//----------------------------------------------------------------------------//

class Surface : public InstantiationPrim
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Surface);

  // Factory -------------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(Surface);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(SurfaceInstancer);

  // From Primitive ------------------------------------------------------------

  virtual BBox wsBounds(Geo::Geometry::CPtr geometry) const;

  // From InstantiationPrimitive -----------------------------------------------

  virtual ModelerInput::Ptr execute(const Geo::Geometry::CPtr geo) const;

protected:

  // Structs -------------------------------------------------------------------

  //! Contains the per-surface attributes
  struct SurfAttrState
  {
    SurfAttrState()
      : seed          ("seed",                           1),
        instanceRadius("instance_radius",                0.01f), 
        numPoints     ("num_points",                     1000), 
        doFill        ("fill",                           1), 
        densScale     ("density_noise_scale",            Imath::V3f(1.0f)), 
        densFade      ("density_noise_fade",             Imath::V3f(0.0)), 
        densOctaves   ("density_noise_octaves",          8.0f), 
        densOctaveGain("density_noise_octave_gain",      0.5f), 
        densLacunarity("density_noise_lacunarity",       1.92f), 
        dispScale     ("displacement_noise_scale",       Imath::V3f(1.0f)), 
        dispOctaves   ("displacement_noise_octaves",     8.0f), 
        dispOctaveGain("displacement_noise_octave_gain", 0.5f), 
        dispLacunarity("displacement_noise_lacunarity",  1.92f), 
        dispAmplitude ("displacement_noise_amplitude",   1.0f), 
        doDensNoise   ("density_noise",                  0), 
        doDispNoise   ("displacement_noise",             0)
    { }
    
    void update(const Geo::AttrVisitor::const_iterator &i);

    Geo::Attr<int>        seed;
    Geo::Attr<float>      instanceRadius;
    Geo::Attr<int>        numPoints;
    Geo::Attr<int>        doFill;    
    Geo::Attr<Imath::V3f> densScale;
    Geo::Attr<Imath::V3f> densFade;
    Geo::Attr<float>      densOctaves;
    Geo::Attr<float>      densOctaveGain;
    Geo::Attr<float>      densLacunarity;
    Geo::Attr<Imath::V3f> dispScale;
    Geo::Attr<float>      dispOctaves;
    Geo::Attr<float>      dispOctaveGain;
    Geo::Attr<float>      dispLacunarity;
    Geo::Attr<float>      dispAmplitude;
    Geo::Attr<int>        doDensNoise;    
    Geo::Attr<int>        doDispNoise;

    Noise::Fractal::CPtr  densFractal;
    Noise::Fractal::CPtr  dispFractal;
  };

  //! Contains the per-point attributes
  struct PointAttrState
  {
    // Constructor ---
    
    PointAttrState()
      : wsP           ("P"), 
        wsNormal      ("N"),
        wsDPds        ("dPds"),
        wsDPdt        ("dPdt"),
        wsVelocity    ("v"), 
        thickness     ("thickness", 1.0f), 
        density       ("density",   Imath::V3f(1.0f))
    { }

    // Main methods ---

    //! Updates the point attributes
    void update(const Geo::AttrVisitor::const_iterator &i);

    // Data members ---

    Geo::Attr<Imath::V3f> wsP;
    Geo::Attr<Imath::V3f> wsNormal;
    Geo::Attr<Imath::V3f> wsDPds;
    Geo::Attr<Imath::V3f> wsDPdt;
    Geo::Attr<Imath::V3f> wsVelocity;
    Geo::Attr<float>      thickness;
    Geo::Attr<Imath::V3f> density;
  };

  // Utility functions ---------------------------------------------------------

  //! Returns the index of the point for the given parametric coordinates
  //! \param s Coordinate in range [0, numCols-1]
  //! \param t Coordinate in range [0, numRows-1]
  //! \param numCols Size of current mesh in X direction
  //! \param numRows Size of current mesh in Y direction
  //! \param xOff Offset in X direction
  //! \param yOff Offset in Y direction
  size_t pointIdx(const float s, const float t, 
                  const size_t numCols, const size_t numRows, 
                  const int xOff, const int yOff) const;

  //! Computes a [-1,1] offset based on the position on the primitive
  //! \param x Position in [0,1] range
  //! \param y Position in [0,1] range
  //! \param z Position in [0,1] range
  //! \note Assumes that m_pointAttrs is updated.
  float edgeFade(float x, float y, float z) const;

  //! Counts the number of points that will be created for the given 
  //! input geometry
  size_t numOutputPoints(const Geo::Geometry::CPtr geo) const;

  virtual void updateSurfAttrs(Geo::AttrVisitor::const_iterator i) const;
  virtual void updatePointAttrs(Geo::AttrVisitor::const_iterator i, 
                                const size_t numPoints) const;

  // Data members --------------------------------------------------------------

  //! Holds the Attr instances that describe a single primitive
  //! Gets set up in execute() and is used in getSample().
  mutable SurfAttrState m_surfAttrs;

  //! Holds the Attr instances that describe each point on the surface prim
  mutable std::vector<PointAttrState> m_pointAttrs;

};

//----------------------------------------------------------------------------//

} // namespace Inst
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
