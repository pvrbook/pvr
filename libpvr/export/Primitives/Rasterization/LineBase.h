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

/*! \file LineBase.h
  Contains the LineBase class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RASTERIZATION_LINEBASE_H__
#define __INCLUDED_PVR_RASTERIZATION_LINEBASE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <vector>

// Library headers

// Project headers

#include "pvr/export.h"
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

#define LINE_INTERP(variable, info)                                   \
  Math::fit01(info.t, m_basePointAttrs[info.index].variable.value(),  \
              m_basePointAttrs[info.index + 1].variable.value())      \

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// LineBase
//----------------------------------------------------------------------------//

/*! \class LineBase
  \brief Implements the rasterization line primitive described in chapter XX.

  See chapter XX for details.
*/

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC LineBase : public RasterizationPrim
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(LineBase);

  // From Primitive ------------------------------------------------------------

  //! Returns a world-space bounding box for the primitive
  virtual BBox wsBounds(Geo::Geometry::CPtr geometry) const;

  // From RasterizationPrim ----------------------------------------------------

  //! Lets the primitive write its data to the voxel buffer
  virtual void execute(Geo::Geometry::CPtr geometry, 
                       VoxelBuffer::Ptr buffer) const;

protected:

  // Structs -------------------------------------------------------------------

  //! Contains the per-point attributes of the Line
  //! Subclasses of LineRasterizationPrimitive should use their own 
  //! PointAttrState to hold per-point attributes.
  struct PointAttrState
  {
    PointAttrState()
      : wsCenter   ("P"), 
        wsVelocity ("v"), 
        density    ("density", Imath::V3f(1.0f)), 
        radius     ("radius",  0.1f)
    { }

    void update(const Geo::AttrVisitor::const_iterator &i);

    Geo::Attr<Imath::V3f> wsCenter;
    Geo::Attr<Imath::V3f> wsVelocity;
    Geo::Attr<Imath::V3f> density;
    Geo::Attr<float>      radius;
  };

  //! Contains the per-primitive properties for each polygon line
  //! Subclasses of LineRasterizationPrimitive should use their own
  //! PolyAttrState to hold per-line/poly attributes.
  struct PolyAttrState
  {
    PolyAttrState()
      : antialiased("antialiased", 1)
    { }
    void update(const Geo::AttrVisitor::const_iterator &i);
    Geo::Attr<int> antialiased;
  };

  //! Struct containing the results of findClosestSegment()
  struct SegmentInfo
  {
    //! Default constructor
    SegmentInfo()
      : index(0), t(0.0), distance(std::numeric_limits<double>::max()),
        radius(0.0)
    { }
    //! Index of segment
    size_t index;
    //! Parametric coordinate on segment
    double t;
    //! World space distance from sample point to closest point on line
    double distance;
    //! Radius of curve at closest point
    double radius;
  };

  // To be implemented by subclasses -------------------------------------------

  //! Updates all per-poly attributes. 
  virtual void updatePolyAttrs(Geo::AttrVisitor::const_iterator i) const;
  //! Updates all per-point attributes. Assumes that all vertices of the polygon
  //! are contiguous in the AttrTable.
  virtual void updatePointAttrs(Geo::AttrVisitor::const_iterator i, 
                                const size_t numPoints) const;
  //! Returns the displacement bounds at the given point.
  //! \returns The relative displacement. I.e. if radius = 2m and displ = 1m,
  //! the function should return 0.5.
  virtual float displacementBounds(const size_t) const
  { return 0.0f; }

  // Utility methods -----------------------------------------------------------

  //! Updates the acceleration structure. Uses the current state of 
  //! m_pointAttrs and m_polyAttrs.
  void updateAccelStruct() const;
  //! Finds the closest line segment on the current polygon described by
  //! m_pointAttrs.
  //! \returns Whether a segment was found for which distance < radius.
  bool findClosestSegment(const RasterizationState &state, 
                          SegmentInfo &info) const;

  // Data members --------------------------------------------------------------

  //! Holds the vector of per-point Attr instances 
  mutable std::vector<PointAttrState> m_basePointAttrs;
  //! Holds the current per-poly Attr instances 
  mutable PolyAttrState m_basePolyAttrs;
  //! Acceleration structure for finding line segments quickly.
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
