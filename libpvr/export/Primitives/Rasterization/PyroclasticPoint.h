//-*-c++-*--------------------------------------------------------------------//

/*! \file PyroclasticPoint.h
  Contains the PyroclasticPoint class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RASTERIZATION_PYROCLASTICPOINT_H__
#define __INCLUDED_PVR_RASTERIZATION_PYROCLASTICPOINT_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/AttrUtil.h"
#include "pvr/Strings.h"
#include "pvr/Types.h"
#include "pvr/VoxelBuffer.h"
#include "pvr/Noise/Noise.h"
#include "pvr/Primitives/RasterizationPrim.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// PyroclasticPoint
//----------------------------------------------------------------------------//

/*! \class PyroclasticPoint
  \brief Implements the pyroclastic point primitive described in chapter XX.

  See chapter XX for details.
*/

//----------------------------------------------------------------------------//

class PyroclasticPoint : public PointBase
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(PyroclasticPoint);

  // Factory -------------------------------------------------------------------

  //! Factory function
  static Ptr create();

  // From ParamBase ------------------------------------------------------------

  //! Returns the type name of the primitive
  virtual std::string typeName() const
  { return "PyroclasticPoint"; }

  // From RasterizationPrim ----------------------------------------------------

  //! Lets the primitive write its data to the voxel buffer
  virtual void execute(Geo::Geometry::CPtr geometry, 
                       VoxelBuffer::Ptr buffer) const;

protected:

  // From RasterizationPrimitive -----------------------------------------------

  virtual void getSample(const RasterizationState &state,
                         RasterizationSample &sample) const;

  // From PointRasterizationPrimitive ------------------------------------------

  virtual BBox pointWsBounds(const Geo::AttrVisitor::const_iterator &i) const;

  // Structs -------------------------------------------------------------------

  //! Contains the information about a single point that is being rasterized.
  struct AttrState
  {
    // Constructor ---

    //! Initializes attribute names and defaults
    AttrState()
      : wsCenter   ("P"), 
        wsVelocity ("v"), 
        orientation("orientation"), 
        radius     ("radius",         1.0f), 
        density    ("density",        Imath::V3f(1.0f)),
        seed       ("seed",           1), 
        scale      ("scale",          1.0f), 
        octaves    ("octaves",        8.0f), 
        octaveGain ("octave_gain",    0.5f), 
        lacunarity ("lacunarity",     1.92f), 
        amplitude  ("amplitude",      1.0f), 
        gamma      ("gamma",          1.0f), 
        pyroclastic("pyroclastic",    1), 
        pyro2D     ("pyroclastic_2d", 1), 
        absNoise   ("absolute_noise", 1),
        antialiased("antialiased", 1)
    { }
     
    // Main methods ---

    //! Updates the point attributes
    void update(const Geo::AttrVisitor::const_iterator &i);

    // Data members ---

    Geo::Attr<Imath::V3f> wsCenter;
    Geo::Attr<Imath::V3f> wsVelocity;
    Geo::Attr<Imath::V3f> orientation;
    Geo::Attr<float>      radius;
    Geo::Attr<Imath::V3f> density;
    Geo::Attr<int>        seed;
    Geo::Attr<float>      scale;
    Geo::Attr<float>      octaves;
    Geo::Attr<float>      octaveGain;
    Geo::Attr<float>      lacunarity;
    Geo::Attr<float>      amplitude;
    Geo::Attr<float>      gamma;
    Geo::Attr<int>        pyroclastic;
    Geo::Attr<int>        pyro2D;
    Geo::Attr<int>        absNoise;
    Geo::Attr<int>        antialiased;
    Matrix                rotation;
    Noise::Fractal::CPtr  fractal;
  };

  // Data members --------------------------------------------------------------

  //! Holds the Attr instances that describe a single point.
  //! Gets set up in execute() and is used in getSample().
  mutable AttrState m_attrs;

};

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
