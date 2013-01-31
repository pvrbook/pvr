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

/*! \file Sphere.h
  Contains the Inst::Sphere class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_INSTANTIATION_SPHERE_H__
#define __INCLUDED_PVR_INSTANTIATION_SPHERE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

// Project headers

#include "pvr/export.h"
#include "pvr/AttrUtil.h"
#include "pvr/ModelerInput.h"
#include "pvr/Primitives/InstantiationPrim.h"
#include "pvr/Types.h"
#include "pvr/Noise/Noise.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Inst {

//----------------------------------------------------------------------------//
// Sphere
//----------------------------------------------------------------------------//

/*! \class Sphere
  \brief Implements the sphere-based instantiation primitive.

  Instantiation-based primitives are covered in chapter XX.
 */

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC Sphere : public InstantiationPrim
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Sphere);

  // Factory -------------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(Sphere);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(SphereInstancer);

  // From Primitive ------------------------------------------------------------

  virtual BBox wsBounds(Geo::Geometry::CPtr geometry) const;

  // From InstantiationPrimitive -----------------------------------------------

  virtual ModelerInput::Ptr execute(const Geo::Geometry::CPtr geo) const;

protected:

  // Structs -------------------------------------------------------------------

  //! Contains the information about a single point that is being rasterized.
  struct AttrState
  {
    // Constructor ---
    
    AttrState()
      : wsCenter      ("P"), 
        wsVelocity    ("v"), 
        radius        ("radius",                         1.0f), 
        instanceRadius("instance_radius",                0.01f), 
        density       ("density",                        Imath::V3f(1.0f)),
        numPoints     ("num_points",                     1000), 
        doFill        ("fill",                           0), 
        seed          ("seed",                           1), 
        densScale     ("density_noise_scale",            1.0f), 
        densOctaves   ("density_noise_octaves",          8.0f), 
        densOctaveGain("density_noise_octave_gain",      0.5f), 
        densLacunarity("density_noise_lacunarity",       1.92f), 
        dispScale     ("displacement_noise_scale",       1.0f), 
        dispOctaves   ("displacement_noise_octaves",     8.0f), 
        dispOctaveGain("displacement_noise_octave_gain", 0.5f), 
        dispLacunarity("displacement_noise_lacunarity",  1.92f), 
        dispAmplitude ("displacement_noise_amplitude",   1.0f), 
        doDensNoise   ("density_noise",                  0), 
        doDispNoise   ("displacement_noise",             0)
    { }

    // Main methods ---

    //! Updates the point attributes
    void update(const Geo::AttrVisitor::const_iterator &i);

    // Data members ---

    Geo::Attr<Imath::V3f> wsCenter;
    Geo::Attr<Imath::V3f> wsVelocity;
    Geo::Attr<float>      radius;
    Geo::Attr<float>      instanceRadius;
    Geo::Attr<Imath::V3f> density;
    Geo::Attr<int>        numPoints;
    Geo::Attr<int>        doFill;    
    Geo::Attr<int>        seed;
    Geo::Attr<float>      densScale;
    Geo::Attr<float>      densOctaves;
    Geo::Attr<float>      densOctaveGain;
    Geo::Attr<float>      densLacunarity;
    Geo::Attr<float>      dispScale;
    Geo::Attr<float>      dispOctaves;
    Geo::Attr<float>      dispOctaveGain;
    Geo::Attr<float>      dispLacunarity;
    Geo::Attr<float>      dispAmplitude;
    Geo::Attr<int>        doDensNoise;    
    Geo::Attr<int>        doDispNoise;
    Noise::Fractal::CPtr  densFractal;
    Noise::Fractal::CPtr  dispFractal;
  };

  // Utility functions ---------------------------------------------------------

  //! Counts the number of points that will be created for the given 
  //! input geometry
  size_t numOutputPoints(const Geo::Geometry::CPtr geo) const;

  // Data members --------------------------------------------------------------

  //! Holds the Attr instances that describe a single point.
  //! Gets set up in execute() and is used in getSample().
  mutable AttrState m_attrs;

};

//----------------------------------------------------------------------------//

} // namespace Inst
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
