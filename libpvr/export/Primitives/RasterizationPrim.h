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

/*! \file RasterizationPrimitive.h
  Contains the RasterizationPrimitive class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RASTERIZATIONPRIMITIVE_H__
#define __INCLUDED_PVR_RASTERIZATIONPRIMITIVE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <boost/foreach.hpp>

// Project headers

#include "pvr/AttrUtil.h"
#include "pvr/Geometry.h"
#include "pvr/Math.h"
#include "pvr/Primitives/Primitive.h"
#include "pvr/Types.h"
#include "pvr/VoxelBuffer.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// RasterizationSample
//----------------------------------------------------------------------------//

struct RasterizationSample
{
  // Data members ---

  Imath::V3f value;
  Vector wsVelocity;
  
  // Constructor ---

  RasterizationSample()
    : value(Imath::V3f(0.0)), wsVelocity(Vector(0.0))
  { }
};

//----------------------------------------------------------------------------//
// RasterizationState
//----------------------------------------------------------------------------//

struct RasterizationState
{
  // Data members ---

  Vector wsP;
  Vector wsVoxelSize;

  // Constructor ---

  RasterizationState()
    : wsP(Vector(0.0)), wsVoxelSize(Vector(0.0))
  { }
};

//----------------------------------------------------------------------------//
// RasterizationPrim
//----------------------------------------------------------------------------//

/*! \class RasterizationPrim
  \brief Base class for rasterization primitives, as described in chapter XX.

  See chapter XX for details.
*/

//----------------------------------------------------------------------------//

class RasterizationPrim : public Primitive
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(RasterizationPrim);

  // To be implemented by subclasses -------------------------------------------

  //! Lets the primitive write to the voxel buffer
  virtual void execute(Geo::Geometry::CPtr geometry, 
                       VoxelBuffer::Ptr buffer) const = 0;

protected:

  // To be called from subclasses ----------------------------------------------

  //! Rasterizes the domain in vsBounds, making a call to wsSample() at each 
  //! voxel.
  void rasterize(const BBox &vsBounds, VoxelBuffer::Ptr buffer) const;

  // To be implemented by subclasses -------------------------------------------

  //! Samples the rasterization primitive at a given world-space position
  //! This is called from rasterize(), and assumes that the internal state
  //! of the subclass is configured to know which underlying primitive is 
  //! being rasterized.
  virtual void getSample(const RasterizationState &state,
                         RasterizationSample &sample) const = 0;

};

//----------------------------------------------------------------------------//
// PointBase
//----------------------------------------------------------------------------//

/*! \class PointBase
  \brief Base class for point rasterization primitives, as described in 
  chapter XX.

  See chapter XX for details.

  \todo Move to separate .h file
*/

//----------------------------------------------------------------------------//

class PointBase : public RasterizationPrim
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(PointBase);

  // From Primitive ------------------------------------------------------------

  //! Returns a world-space bounding box for all points in primitive
  //! \note Calls pointWsBounds().
  virtual BBox wsBounds(Geo::Geometry::CPtr geometry) const;

protected:

  // To be implemented by subclasses -------------------------------------------

  //! Returns a world-space bounding box for single point in primitive
  //! \todo THIS SHOULD NOT TAKE V INTO ACCOUNT, SINCE RASTERIZATION HAPPENS
  //! AT TIME=0.0
  virtual BBox pointWsBounds(const Geo::AttrVisitor::const_iterator &i) const = 0;

};

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
