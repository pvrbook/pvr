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

/*! \file ModelingUtils.h
  Contains the ModelingUtils class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_MODELINGUTILS_H__
#define __INCLUDED_PVR_MODELINGUTILS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <boost/foreach.hpp>

#include <OpenEXR/ImathFun.h>

// Project headers

#include "pvr/Globals.h"
#include "pvr/Math.h"
#include "pvr/Types.h"
#include "pvr/VoxelBuffer.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {

//----------------------------------------------------------------------------//
// Utility functions
//--------------------------------------------------------------------------//

inline double sphereVolume(const double radius)
{
  return (4.0 / 3.0) * M_PI * radius * radius * radius;
}

//----------------------------------------------------------------------------//

inline void writePoint(const Vector &vsP, const Imath::V3f &value, 
                       VoxelBuffer::Ptr buffer)
{
  using namespace std;
  using namespace Imath;
  using namespace Field3D;
  
  int i = contToDisc(vsP.x);
  int j = contToDisc(vsP.y);
  int k = contToDisc(vsP.z);
  
  if (buffer->isInBounds(i, j, k)) {
    buffer->lvalue(i, j, k) += value;
  }
}

//--------------------------------------------------------------------------//

inline void writeAntialiasedPoint(const Vector &vsP, const Imath::V3f &value, 
                                  VoxelBuffer::Ptr buffer)
{
  using namespace std;
  using namespace Imath;
  
  // Offset the voxel-space position relative to voxel centers
  // The rest of the calculations will be done in this space
  Vector p(vsP.x - 0.5, vsP.y - 0.5, vsP.z - 0.5);
  // Find the lower-left corner of the cube of 8 voxels that
  // we need to access
  V3i corner(static_cast<int>(floor(p.x)), 
             static_cast<int>(floor(p.y)), 
             static_cast<int>(floor(p.z)));
  // Calculate P's fractional distance between voxels
  // We start out with weight = fraction, then each step in the loop
  // inverts the value.
  Vector fraction(static_cast<Vector>(corner + V3i(1)) - p);
  // Loop over the 8 voxels and distribute the value
  for (int k = 0; k < 2; k++) {
    for (int j = 0; j < 2; j++) {
      for (int i = 0; i < 2; i++) {
        double weight = fraction[0] * fraction[1] * fraction[2];
        if (buffer->isInBounds(corner.x + i, corner.y + j, corner.z + k)) {
          buffer->lvalue(corner.x + i, 
                         corner.y + j, 
                         corner.z + k) += value * weight;
        }
        fraction[0] = 1.0 - fraction[0];
      }
      fraction[1] = 1.0 - fraction[1];
    }
    fraction[2] = 1.0 - fraction[2];
  }
}

//----------------------------------------------------------------------------//

template <bool Antialiased_T>
inline void writeLine(const Vector &vsStart, const Vector &vsEnd,
                      const Imath::V3f &value, VoxelBuffer::Ptr buffer)
{
  using namespace std;
  using namespace Imath;
  using namespace Field3D;
  
  // Construct a line in voxel space
  Vector vsLine = (vsEnd - vsStart);
  // Determine number of splats based on length in voxel space
  size_t numSamples = static_cast<size_t>(std::ceil(vsLine.length()));
  numSamples = max(static_cast<size_t>(2), numSamples);

  // Splat each sample
  Imath::V3f sampleValue = value / static_cast<double>(numSamples);
  for (size_t i = 0; i < numSamples; ++i) {
    // Find current point on line
    double fraction = static_cast<double>(i) / 
      static_cast<double>(numSamples - 1);
    Vector vsP = Imath::lerp(vsStart, vsEnd, fraction);
    // Write antialiased or non-antialiased point based on template argument
    if (Antialiased_T) {
      writeAntialiasedPoint(vsP, sampleValue, buffer);
    } else {
      writePoint(vsP, sampleValue, buffer);
    }
  }
}

//----------------------------------------------------------------------------//

inline BBox vsSphereBounds(Field3D::FieldMapping::Ptr mapping, 
                           const Vector &wsCenter, const float wsRadius) 
{
  BBox vsBounds;
  // Construct world space bounds from center and radius
  BBox wsBounds(wsCenter - Vector(wsRadius), wsCenter + Vector(wsRadius));
  // Get the corner points of the bounding box
  std::vector<Vector> wsCornerPoints = Math::cornerPoints(wsBounds);
  // Transform each corner point to voxel space
  BOOST_FOREACH (const Vector &wsP, wsCornerPoints) {
    Vector vsP;
    mapping->worldToVoxel(wsP, vsP);
    vsBounds.extendBy(vsP);
  }
  // Return the constructed voxel space bounds
  return vsBounds;
}

//----------------------------------------------------------------------------//

inline Vector wsVoxelSize(Field3D::FieldMapping::Ptr mapping, const Vector &vsP)
{
  Imath::V3i vsIndex = Field3D::contToDisc(vsP);
  return mapping->wsVoxelSize(vsIndex.x, vsIndex.y, vsIndex.z);
}

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
