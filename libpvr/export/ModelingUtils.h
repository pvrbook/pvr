//-*-c++-*--------------------------------------------------------------------//

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
  return (4.0 / 3.0) * 3.1416 * radius * radius * radius;
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
  // We start out with (1.0 - fraction) since each step of the loop
  // will invert the value
  Vector fraction(Vector(1.0f) - (static_cast<Vector>(corner + V3i(1)) - p));
  // Loop over the 8 voxels and distribute the value
  for (int k = 0; k < 2; k++) {
    fraction[2] = 1.0 - fraction[2];
    for (int j = 0; j < 2; j++) {
      fraction[1] = 1.0 - fraction[1];
      for (int i = 0; i < 2; i++) {
        fraction[0] = 1.0 - fraction[0];
        double weight = fraction[0] * fraction[1] * fraction[2];
        if (buffer->isInBounds(corner.x + i, corner.y + j, corner.z + k)) {
          buffer->lvalue(corner.x + i, 
                         corner.y + j, 
                         corner.z + k) += value * weight;
        }
      }
    }
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
  
  Vector vsLine = (vsEnd - vsStart);
  size_t numSamples = static_cast<size_t>(std::ceil(vsLine.length()));
  numSamples = max(static_cast<size_t>(2), numSamples);

  for (size_t i = 0; i < numSamples; ++i) {
    double fraction = static_cast<double>(i) / 
      static_cast<double>(numSamples - 1);
    Imath::V3f sampleValue = value / static_cast<double>(numSamples);
    Vector vsP = Imath::lerp(vsStart, vsEnd, fraction);
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
  BBox wsBounds(wsCenter - Vector(wsRadius), wsCenter + Vector(wsRadius));
  std::vector<Vector> wsCornerPoints = Math::cornerPoints(wsBounds);
  BOOST_FOREACH (const Vector &wsP, wsCornerPoints) {
    Vector vsP;
    mapping->worldToVoxel(wsP, vsP);
    vsBounds.extendBy(vsP);
  }
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
