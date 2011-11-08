//-*-c++-*--------------------------------------------------------------------//

/*! \file Types.h
  Contains the Types class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_TYPES_H__
#define __INCLUDED_PVR_TYPES_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <stdexcept>

// Library includes

#include <boost/shared_ptr.hpp>

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathColor.h>
#include <OpenEXR/ImathEuler.h>
#include <OpenEXR/ImathLine.h>
#include <OpenEXR/ImathMatrix.h>
#include <OpenEXR/ImathPlane.h>
#include <OpenEXR/ImathQuat.h>
#include <OpenEXR/ImathVec.h>

#include <Field3D/Field.h>

//----------------------------------------------------------------------------//
// Namespaces for SPI
//----------------------------------------------------------------------------//

#ifdef FIELD3D_NS 
namespace Field3D = SPI::Field3D;
#endif

//----------------------------------------------------------------------------//
// Namespaces 
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Smart pointers
//----------------------------------------------------------------------------//
 
#define DECLARE_SMART_PTRS(name)                \
  typedef boost::shared_ptr<name>       Ptr;    \
  typedef boost::shared_ptr<const name> CPtr;   \

#define DECLARE_CREATE_FUNC(name)               \
  static Ptr create()                           \
  { return Ptr(new name); }                     \

//----------------------------------------------------------------------------//

namespace Util {

  //! Traits class, assists in creating a shared pointer to a given type.
  template <typename T>
  struct SPtr
  {
    //! Access to type
    typedef boost::shared_ptr<T> type;
  };

}

//----------------------------------------------------------------------------//
// Common PVR types
//----------------------------------------------------------------------------//

//typedef std::pair<double, double> Interval;

struct Interval
{
  double t0;
  double t1;
  double stepLength;
  Interval(double start, double end, double step)
    : t0(start), t1(end), stepLength(step) 
  { /* Empty */ }
};

typedef std::vector<Interval> IntervalVec;

typedef Imath::Box3d   BBox;
typedef Imath::Box3i   DiscreteBBox;
typedef Imath::C3f     Color;
typedef Imath::Eulerd  Euler;
typedef Imath::Line3d  Ray;
typedef Imath::M44d    Matrix;
typedef Imath::Plane3d Plane;
typedef Imath::Quatd   Quat;
typedef Imath::V3d     Vector;

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
