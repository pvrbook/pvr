//-*-c++-*--------------------------------------------------------------------//

/*! \file Line.h
  Contains the Inst::Line class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INSTANTIATION_LINE_H__
#define __INCLUDED_PVR_INSTANTIATION_LINE_H__

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
//! \param t In range [0, nPoints - 1]
#define LINE_INST_INTERP(variable, t)                                   \
  Math::fit01(t - std::floor(t),                                        \
    m_pointAttrs[static_cast<int>(std::floor(t))].variable.value(),     \
    m_pointAttrs[static_cast<int>(std::ceil(t))].variable.value())  \
  
//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Inst {

//----------------------------------------------------------------------------//
// Line
//----------------------------------------------------------------------------//

/*! \class Line
  \brief Implements the line-based instantiation primitive.

  Instantiation-based primitives are covered in chapter XX.
 */

//----------------------------------------------------------------------------//

class Line : public InstantiationPrim
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Line);

  // Factory -------------------------------------------------------------------

  DECLARE_CREATE_FUNC(Line);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "LineInstancer"; }

  // From Primitive ------------------------------------------------------------

  virtual BBox wsBounds(Geo::Geometry::CPtr geometry) const;

  // From InstantiationPrimitive -----------------------------------------------

  virtual ModelerInput::Ptr execute(const Geo::Geometry::CPtr geo) const;

protected:

  // Structs -------------------------------------------------------------------

  //! Contains the per-line attributes
  struct PolyAttrState
  {
    PolyAttrState()
      : instanceRadius("instance_radius",                0.01f), 
        numPoints     ("num_points",                     1000), 
        doFill        ("fill",                           0), 
        densScale     ("density_noise_scale",            Imath::V3f(1.0f)), 
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

    Geo::Attr<float>      instanceRadius;
    Geo::Attr<int>        numPoints;
    Geo::Attr<int>        doFill;    
    Geo::Attr<Imath::V3f> densScale;
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
        wsTangent     ("T"), 
        wsVelocity    ("v"), 
        radius        ("radius",  1.0f), 
        density       ("density", Imath::V3f(1.0f))
    { }

    // Main methods ---

    //! Updates the point attributes
    void update(const Geo::AttrVisitor::const_iterator &i);

    // Data members ---

    Geo::Attr<Imath::V3f> wsP;
    Geo::Attr<Imath::V3f> wsNormal;
    Geo::Attr<Imath::V3f> wsTangent;
    Geo::Attr<Imath::V3f> wsVelocity;
    Geo::Attr<float>      radius;
    Geo::Attr<Imath::V3f> density;
  };

  //! Contains information about the current batch state. 
  struct BatchState
  {
    Geo::Geometry::CPtr geo;
    size_t lastPointIdx;
  };

  // Utility functions ---------------------------------------------------------

  //! Counts the number of points that will be created for the given 
  //! input geometry
  size_t numOutputPoints(const Geo::Geometry::CPtr geo) const;

  virtual void updatePolyAttrs(Geo::AttrVisitor::const_iterator i) const;
  virtual void updatePointAttrs(Geo::AttrVisitor::const_iterator i, 
                                const size_t numPoints) const;

  // Data members --------------------------------------------------------------

  //! Holds the Attr instances that describe a single primitive
  //! Gets set up in execute() and is used in getSample().
  mutable PolyAttrState m_polyAttrs;

  //! Holds the Attr instances that describe each point on the line prim
  mutable std::vector<PointAttrState> m_pointAttrs;

  //! Holds the current batch state
  mutable BatchState m_batch;
};

//----------------------------------------------------------------------------//

} // namespace Inst
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
