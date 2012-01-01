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
  
#define DECLARE_CREATE_FUNC_1_ARG(name, argType)        \
  static Ptr create(argType arg)                        \
  { return Ptr(new name(arg)); }                        \

#define DECLARE_CREATE_FUNC_2_ARG(name, argType1, argType2)      \
  static Ptr create(argType1 arg1, argType2 arg2)                \
  { return Ptr(new name(arg1, arg2)); }                          \

#define DECLARE_CREATE_FUNC_3_ARG(name, argType1, argType2, argType3)   \
  static Ptr create(argType1 arg1, argType2 arg2, argType3 arg3)        \
  { return Ptr(new name(arg1, arg2, arg3)); }                           \

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
// Interval
//----------------------------------------------------------------------------//

//! Represents a single integration interval. This communicates to a raymarcher
//! what parts of a ray should be integrated, and is normally only created
//! by the Volume::intersect() routines.
//! The interval is assumed to be inclusive, i.e. [t0,t1].
struct Interval
{
  // Constructor ---------------------------------------------------------------

  //! Default constructor
  Interval(double start, double end, double step)
    : t0(start), t1(end), stepLength(step) 
  { }

  // Public data members -------------------------------------------------------

  //! The start of the interval (inclusive)
  double t0;
  //! The end of the interval (inclusive)
  double t1;
  //! The world space step length that is reasonable to use for the given 
  //! interval.
  double stepLength;
};

typedef std::vector<Interval> IntervalVec;

//----------------------------------------------------------------------------//
// Common PVR types
//----------------------------------------------------------------------------//

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
