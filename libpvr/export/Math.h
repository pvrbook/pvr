//-*-c++-*--------------------------------------------------------------------//

/*! \file Math.h
  Contains various math utility functions
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_MATH_H__
#define __INCLUDED_PVR_MATH_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

// Project includes

#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {
namespace Math {

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

//! Returns a discrete-valued bounding box. Guaranteed to contain the 
//! real-valued bounding box.
DiscreteBBox discreteBounds(const BBox &bbox);

//----------------------------------------------------------------------------//

//! Returns a discrete-valued bounding box. Guaranteed to contain the 
//! real-valued bounding box.
DiscreteBBox clipBounds(const DiscreteBBox &bbox, const DiscreteBBox &bounds);

//----------------------------------------------------------------------------//

template <typename S, typename T, typename U>
Imath::Box<Imath::Vec3<S> > 
extendBounds(const Imath::Box<Imath::Vec3<S> > &bounds, const Imath::Vec3<T> &p,
             const U radius);

//----------------------------------------------------------------------------//

//! Returns a vector containing the corner points of the given box.
template <typename T>
std::vector<Imath::Vec3<T> > 
cornerPoints(const Imath::Box<Imath::Vec3<T> > &box);

//----------------------------------------------------------------------------//

template <typename T>
Imath::Vec3<T> normalizeXY(const Imath::Vec3<T> &v);

//----------------------------------------------------------------------------//

//! Abs of each component from Vec3<T>
template <typename T>
Imath::Vec3<T> abs(const Imath::Vec3<T> &vec);

//----------------------------------------------------------------------------//

//! Max component from Vec3<T>
template <typename T>
T max(const Imath::Vec3<T> &vec);

//----------------------------------------------------------------------------//

//! Min component from Vec3<T>
template <typename T>
T min(const Imath::Vec3<T> &vec);

//----------------------------------------------------------------------------//

//! Fits a value from range 1 to range 2
//! \param t Value to fit
//! \param min Old range min
//! \param max Old range max
//! \param newMin New range min
//! \param newMax New range max
template <typename S, typename T>
S fit(const T &t, const T &min, const T &max, const S &newMin, const S &newMax);

//----------------------------------------------------------------------------//

//! Fits a value in the range [0,1] to a new range
//! \param t Value to fit
//! \param newMin New range min
//! \param newMax New range max
template <typename S, typename T>
S fit01(const T &t, const S &newMin, const S &newMax);

//----------------------------------------------------------------------------//

//! Linear interpolation in 2D array
//! \param Coordinate in X. Range [0, 1].
//! \param Coordinate in Y. Range [0, 1].
//! \param x0y0 Value at x=0, y=0.
//! \param x1y0 Value at x=1, y=0.
//! \param x0y1 Value at x=0, y=1.
//! \param x1y1 Value at x=1, y=1.
template <typename S, typename T>
S linear2D(const T s, const T t, 
           const S &x0y0, const S &x1y0, const S &x0y1, const S &x1y1);

//----------------------------------------------------------------------------//

template <typename S, typename T>
S baryInterp(const Imath::Vec3<T> &bary, const S &p0, const S &p1, const S &p2);

//----------------------------------------------------------------------------//

template <typename T>
T smoothStep(const T x, const T min, const T max);

//----------------------------------------------------------------------------//

template <typename S, typename T>
T gamma(const T x, const S gamma);

//----------------------------------------------------------------------------//
  
// Adapted from Geometric Tools, p. 628
bool intersect(const Ray &ray, const BBox &box, double &outT0, double &outT1);

//----------------------------------------------------------------------------//

//! Finds barycentric coordinates for a point in triangle
template <typename T>
Imath::Vec3<T> barycentricCoords(const Imath::Vec3<T> p1, 
                                 const Imath::Vec3<T> p2, 
                                 const Imath::Vec3<T> p3, 
                                 const Imath::Vec3<T> p);

//----------------------------------------------------------------------------//

//! Closest point on line segment.
template <class Vec_T>
Vec_T closestPointOnLineSegment(const Vec_T &p0, const Vec_T &p1, 
                                const Vec_T &p, typename Vec_T::BaseType &t);

//----------------------------------------------------------------------------//

//! Closest point on triangle.
template <class Vec_T>
Vec_T closestPointOnTriangle(const Vec_T &p0, const Vec_T &p1, const Vec_T &p1, 
                             const Vec_T &p, Vec_T &bary, 
                             typename Vec_T::BaseType &distance);

//----------------------------------------------------------------------------//

//! Coordinate system from basis vectors
template <typename T>
Imath::Matrix44<T> coordinateSystem(const Imath::Vec3<T> &e1,
                                    const Imath::Vec3<T> &e2,
                                    const Imath::Vec3<T> &e3,
                                    const Imath::Vec3<T> &origin);

//----------------------------------------------------------------------------//
// Template implementations
//----------------------------------------------------------------------------//

template <typename S, typename T, typename U>
Imath::Box<Imath::Vec3<S> > 
extendBounds(const Imath::Box<Imath::Vec3<S> > &bounds, const Imath::Vec3<T> &p,
             const U radius)
{
  Imath::Box<Imath::Vec3<S> > newBounds = bounds;
  newBounds.extendBy(p - Imath::Vec3<T>(radius));
  newBounds.extendBy(p + Imath::Vec3<T>(radius));
  return newBounds;
}

//----------------------------------------------------------------------------//

template <typename T>
std::vector<Imath::Vec3<T> > 
cornerPoints(const Imath::Box<Imath::Vec3<T> > &box)
{
  std::vector<Imath::Vec3<T> > result;
  result.push_back(Imath::Vec3<T>(box.min.x, box.min.y, box.min.z));
  result.push_back(Imath::Vec3<T>(box.max.x, box.min.y, box.min.z));
  result.push_back(Imath::Vec3<T>(box.min.x, box.max.y, box.min.z));
  result.push_back(Imath::Vec3<T>(box.max.x, box.max.y, box.min.z));
  result.push_back(Imath::Vec3<T>(box.min.x, box.min.y, box.max.z));
  result.push_back(Imath::Vec3<T>(box.max.x, box.min.y, box.max.z));
  result.push_back(Imath::Vec3<T>(box.min.x, box.max.y, box.max.z));
  result.push_back(Imath::Vec3<T>(box.max.x, box.max.y, box.max.z));
  return result;
}

//----------------------------------------------------------------------------//

template <typename T>
Imath::Vec3<T> normalizeXY(const Imath::Vec3<T> &v)
{
  T xyLen = std::sqrt(v.x * v.x + v.y * v.y);
  return Imath::Vec3<T>(v.x / xyLen, v.y / xyLen, v.z);
}

//----------------------------------------------------------------------------//

template <typename T>
Imath::Vec3<T> abs(const Imath::Vec3<T> &vec)
{
  return Imath::Vec3<T>(std::abs(vec.x), std::abs(vec.y), std::abs(vec.z));
}

//----------------------------------------------------------------------------//

template <typename T>
T max(const Imath::Vec3<T> &vec)
{
  return std::max(std::max(vec.x, vec.y), vec.z);
}

//----------------------------------------------------------------------------//

template <typename T>
T min(const Imath::Vec3<T> &vec)
{
  return std::min(std::min(vec.x, vec.y), vec.z);
}

//----------------------------------------------------------------------------//

template <typename S, typename T>
S fit(const T &t, const T &min, const T &max, const S &newMin, const S &newMax)
{
  T interpT = Imath::lerpfactor(t, min, max);
  return Imath::lerp(newMin, newMax, interpT);
}

//----------------------------------------------------------------------------//

template <typename S, typename T>
S fit01(const T &t, const S &newMin, const S &newMax)
{
  T interpT = Imath::lerpfactor(t, T(0.0), T(1.0));
  return Imath::lerp(newMin, newMax, interpT);
}

//----------------------------------------------------------------------------//

template <typename S, typename T>
S linear2D(const T s, const T t, 
           const S &x0y0, const S &x1y0, const S &x0y1, const S &x1y1)
{
  S y0 = Math::fit01(s, x0y0, x1y0);
  S y1 = Math::fit01(s, x0y1, x1y1);
  return Math::fit01(t, y0, y1);
}

//----------------------------------------------------------------------------//

template <typename S, typename T>
S baryInterp(const Imath::Vec3<T> &bary, const S &p0, const S &p1, const S &p2)
{
  return bary.x * p0 + bary.y * p1 + bary.z * p2;
}

//----------------------------------------------------------------------------//

template <typename T>
T smoothStep(const T xArg, const T min, const T max)
{
  T x = Imath::clamp((xArg - min) / (max - min), 0.0, 1.0);
  return x * x * (3 - 2 * x);
}

//----------------------------------------------------------------------------//

template <typename S, typename T>
T gamma(const T x, const S gamma)
{
  if (x > static_cast<T>(0.0)) {
    return std::pow(x, static_cast<T>(1.0) / static_cast<T>(gamma));
  } else if (x < static_cast<T>(0.0)) {
    return -std::pow(-x, static_cast<T>(1.0) / static_cast<T>(gamma));
  } else {
    return static_cast<T>(0.0);
  }
}

//----------------------------------------------------------------------------//

template <typename T>
Imath::Vec3<T> barycentricCoords(const Imath::Vec3<T> p0, 
                                 const Imath::Vec3<T> p1, 
                                 const Imath::Vec3<T> p2, 
                                 const Imath::Vec3<T> p)
{
  // Compute the normal of the triangle
  // Vector N = Normalize(Cross(B-A,C-A));
  Imath::Vec3<T> N = (p1 - p0).cross(p2 - p0).normalized();
  // Compute twice area of triangle ABC
  // float AreaABC = Dot(N,Cross(B-A,C-A));
  T area012 = N.dot((p1 - p0).cross(p2 - p0));
  // Compute a
  // float AreaPBC = Dot(N,Cross(B-P,C-P));
  // float a = AreaPBC / AreaABC;
  T areaP12 = N.dot((p1 - p).cross(p2 - p));
  T a = areaP12 / area012;
  // Compute b
  // float AreaPCA = Dot(N,Cross(C-P,A-P));
  // float b = AreaPCA / AreaABC;
  T areaP20 = N.dot((p2 - p).cross(p0 - p));
  T b = areaP20 / area012;
  // Compute c
  T c = static_cast<T>(1.0) - a - b;
}

//----------------------------------------------------------------------------//

template <class Vec_T>
Vec_T closestPointOnLineSegment(const Vec_T &p0, const Vec_T &p1, 
                                const Vec_T &p, typename Vec_T::BaseType &t) 
{
  typedef typename Vec_T::BaseType T;

  Vec_T d = p1 - p0;
  T l2 = d.length2();

  if (l2 == 0) {
    t = static_cast<T>(0);
    return p0;
  }

  t = Imath::clamp((p - p0).dot(d) / l2, static_cast<T>(0), static_cast<T>(1));
  return p0 + d * t;
}

//----------------------------------------------------------------------------//

template <class Vec_T>
Vec_T closestPointOnTriangle(const Vec_T &p0, const Vec_T &p1, const Vec_T &p2, 
                             const Vec_T &p, Vec_T &bary, 
                             typename Vec_T::BaseType &distance)
{
  typedef typename Vec_T::BaseType T;
  Imath::Plane3<T> plane(p0, p1, p2);
  distance = plane.distanceTo(p);
  Vec_T pointOnTriangle = p - plane.normal * distance;
  bary = barycentricCoords(p0, p1, p2, pointOnTriangle);
  return pointOnTriangle;
}

//----------------------------------------------------------------------------//

//! Coordinate system from basis vectors
template <typename T>
Imath::Matrix44<T> coordinateSystem(const Imath::Vec3<T> &e1,
                                    const Imath::Vec3<T> &e2,
                                    const Imath::Vec3<T> &e3,
                                    const Imath::Vec3<T> &origin)
{
  Imath::Matrix44<T> m;
  m[0][0] = e1.x;
  m[0][1] = e1.y;
  m[0][2] = e1.z;
  m[1][0] = e2.x;
  m[1][1] = e2.y;
  m[1][2] = e2.z;
  m[2][0] = e3.x;
  m[2][1] = e3.y;
  m[2][2] = e3.z;
  m[3][0] = origin.x;
  m[3][1] = origin.y;
  m[3][2] = origin.z;
  return m;
}

//----------------------------------------------------------------------------//

} // namespace Math
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
