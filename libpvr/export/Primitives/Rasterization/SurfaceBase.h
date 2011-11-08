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

/*! \file SurfaceBase.h
  Contains the Surface class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_SURFACEBASE_H__
#define __INCLUDED_PVR_SURFACEBASE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <vector>

// Library headers

// #include <Field3D/DenseField.h>

// Project headers

#include "pvr/AttrUtil.h"
#include "pvr/Math.h"
#include "pvr/Strings.h"
#include "pvr/Types.h"
#include "pvr/VoxelBuffer.h"
#include "pvr/Primitives/RasterizationPrim.h"
#include "pvr/Acceleration.h"

// Debug
#include "pvr/Log.h"

//----------------------------------------------------------------------------//
// Defines
//----------------------------------------------------------------------------//

#define SURFACE_INTERP(variable, info)                                  \
  Math::baryInterp(info.bary,                                           \
                   m_basePointAttrs[info.index].variable.value(),       \
                   m_basePointAttrs[info.index + 1].variable.value(),   \
                   m_basePointAttrs[info.index + 2].variable.value())   \
  
//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// SurfaceBase
//----------------------------------------------------------------------------//

/*! \class SurfaceBase
  \brief Implements the rasterization surface primitive described in chapter XX.

  See chapter XX for details.
*/

//----------------------------------------------------------------------------//

class SurfaceBase : public RasterizationPrim
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(SurfaceBase);

  // From Primitive ------------------------------------------------------------

  //! Returns a world-space bounding box for the primitive
  virtual BBox wsBounds(Geo::Geometry::CPtr geometry) const;

  // From RasterizationPrim ----------------------------------------------------

  //! Lets the primitive write its data to the voxel buffer
  virtual void execute(Geo::Geometry::CPtr geometry, 
                       VoxelBuffer::Ptr buffer) const;

protected:

  // Structs -------------------------------------------------------------------

  //! Contains the per-point attributes of the Surface
  struct PointAttrState
  {
    PointAttrState()
      : wsCenter   ("P"), 
        wsVelocity ("v"), 
        density    ("density", Imath::V3f(1.0f)), 
        thickness  ("thickness",  0.1f)
    { }

    void update(const Geo::AttrVisitor::const_iterator &i);

    Geo::Attr<Imath::V3f> wsCenter;
    Geo::Attr<Imath::V3f> wsVelocity;
    Geo::Attr<Imath::V3f> density;
    Geo::Attr<float>      thickness;
  };

  //! Contains the per-surface properties for each triangle mesh surface
  struct SurfaceAttrState
  {
    SurfaceAttrState()
      : antialiased("antialiased", 1)
    { }

    void update(const Geo::AttrVisitor::const_iterator &i);

    Geo::Attr<int> antialiased;
  };

  //! Struct containing the results of findClosestTri()
  struct TriangleInfo
  {
    //! Index of triangle's first vertex. Assumes triangle vertex indices are
    //! contiguous.
    size_t index;
    //! Barycentric coordinates on triangle
    Vector bary;
    //! World space distance from sample point to closest point on surface
    double distance;
    //! Thickness of surface at closest point
    double thickness;
  };

  // To be implemented by subclasses -------------------------------------------

  //! Updates all per-surface attributes. 
  virtual void updateSurfaceAttrs(Geo::AttrVisitor::const_iterator i) const;
  //! Updates all per-point attributes. Assumes that all vertices of the 
  //! polygons are contiguous in the AttrTable.
  virtual void updatePointAttrs(Geo::AttrVisitor::const_iterator i, 
                                const size_t numPoints) const;
  //! Returns the displacement bounds at the given point.
  //! \returns The relative displacement. I.e. if radius = 2m and displ = 1m,
  //! the function should return 0.5.
  virtual float displacementBounds(const size_t) const
  { return 0.0f; }

  // Utility methods -----------------------------------------------------------

  //! Updates the acceleration structure. Uses the current state of 
  //! m_pointAttrs and m_surfaceAttrs.
  void updateAccelStruct() const;
  //! Finds the closest triangle on the current polygon described by
  //! m_pointAttrs.
  //! \returns Whether a triangle was found for which distance < radius.
  bool findClosestTriangle(const RasterizationState &state, 
                          TriangleInfo &info) const;

  // Data members --------------------------------------------------------------

  //! Holds the vector of per-point Attr instances. These points are assumed
  //! to define a triangle mesh of size = numPoints / 3, according to the 
  //! pattern [0, 1, 2], [3, 4, 5], etc.
  mutable std::vector<PointAttrState> m_basePointAttrs;
  //! Holds the current per-surface Attr instances 
  mutable SurfaceAttrState m_baseSurfaceAttrs;
  //! Acceleration structure for finding triangles quickly.
  mutable pvr::Accel::UniformGrid<size_t> m_gridAccel;
};

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
