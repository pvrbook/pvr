//----------------------------------------------------------------------------//

/*! \file Math.cpp
  Contains implementations of math utility functions
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Math.h"

// System includes

// Library includes

// Project headers

#include "pvr/Strings.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Math {

//----------------------------------------------------------------------------//

DiscreteBBox discreteBounds(const BBox &bbox)
{
  DiscreteBBox result;
  result.min.x = static_cast<int>(std::floor(bbox.min.x));
  result.min.y = static_cast<int>(std::floor(bbox.min.y));  
  result.min.z = static_cast<int>(std::floor(bbox.min.z));
  result.max.x = static_cast<int>(std::ceil(bbox.max.x));
  result.max.y = static_cast<int>(std::ceil(bbox.max.y));  
  result.max.z = static_cast<int>(std::ceil(bbox.max.z));
  return result;
}

//----------------------------------------------------------------------------//

DiscreteBBox clipBounds(const DiscreteBBox &bbox, const DiscreteBBox &bounds)
{
  DiscreteBBox result;
  result.min.x = std::max(bbox.min.x, bounds.min.x);
  result.min.y = std::max(bbox.min.y, bounds.min.y);
  result.min.z = std::max(bbox.min.z, bounds.min.z);
  result.max.x = std::min(bbox.max.x, bounds.max.x);
  result.max.y = std::min(bbox.max.y, bounds.max.y);
  result.max.z = std::min(bbox.max.z, bounds.max.z);
  return result;
}

//----------------------------------------------------------------------------//
  
// Adapted from Geometric Tools, p. 628
bool intersect(const Ray &ray, const BBox &box, double &outT0, double &outT1)
{
  double tNear = -std::numeric_limits<double>::max();
  double tFar = std::numeric_limits<double>::max();
  const double epsilon = 1.0e-6;
  
  for (size_t dim = 0; dim < 3; ++dim) {
    double t0, t1;
    if (std::abs(ray.dir[dim]) < epsilon) {
      // Ray is parallel, check if inside slab
      if (ray.pos[dim] < box.min[dim] || ray.pos[dim] > box.max[dim]) {
        return false;
      }
    }
    t0 = (box.min[dim] - ray.pos[dim]) / ray.dir[dim];
    t1 = (box.max[dim] - ray.pos[dim]) / ray.dir[dim];
    if (t0 > t1) {
      std::swap(t0, t1);
    }
    tNear = std::max(tNear, t0);
    tFar = std::min(tFar, t1);
    if (tNear > tFar) {
      return false;
    }
    if (tFar < 0.0) {
      return false;
    }
  }
  outT0 = tNear;
  outT1 = tFar;
  return true;
}

//----------------------------------------------------------------------------//

} // namespace Math
} // namespace pvr

//----------------------------------------------------------------------------//
