//-*-c++-*--------------------------------------------------------------------//

/*! \file Curve.h
  Contains the Curve class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_CURVE_H__
#define __INCLUDED_PVR_CURVE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <cmath>
#include <vector>

// Library headers

#include <boost/lexical_cast.hpp>

#include <OpenEXR/ImathQuat.h>
#include <OpenEXR/ImathVec.h>

// Project headers

#include "pvr/AttrTable.h"
#include "pvr/Exception.h"
#include "pvr/Math.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Util {

//----------------------------------------------------------------------------//
// Curve
//----------------------------------------------------------------------------//

/*! \brief A simple function curve class. 

  The Curve class is most commonly used to pass time-varying parameters. 
  The class itself puts no restriction on the time values, but when feeding
  attributes that vary over the course of a single frame, the time used
  is usually [0.0, 1.0], corresponding to the shutter open and close times.

  The class can also be used for generic lookup curves.
 */

//----------------------------------------------------------------------------//

template <typename T>
class Curve
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Curve);

  typedef std::pair<float, T> Sample;
  typedef std::vector<Sample> SampleVec;

  // Constructor, destructor, factory ------------------------------------------

  Curve()
  { }
  Curve(const T &initialValue)
  { addSample(0.0f, initialValue); }
  Curve(const size_t numSamples, const T &initialValue);

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  static Ptr create()
  { return Ptr(new Curve); }

  // Main methods --------------------------------------------------------------

  //! Adds a sample point to the curve.
  //! \param t Sample position
  //! \param value Sample value
  void               addSample(const float t, const T &value);
  //! Interpolates a value from the curve.
  //! \param t Position along curve
  T                  interpolate(const float t) const;
  //! Returns number of samples in curve
  size_t             numSamples() const;
  //! Returns a const reference to the samples in the curve.
  const SampleVec&   samples() const;
  //! Returns the sample locations
  std::vector<float> samplePoints() const;
  //! Returns the sample values
  std::vector<T>     sampleValues() const;
  //! Removes duplicated values
  void               removeDuplicates();
  //! Average a set of Curves into a single one.
  static CPtr        average(const std::vector<CPtr> &curves);

private:
  
  // Structs -------------------------------------------------------------------

  //! Used when looking up values in the m_samples vector.
  struct CheckTGreaterThan : 
    public std::unary_function<std::pair<float, T>, bool>
  {
    CheckTGreaterThan(float match)
      : m_match(match)
    { }
    bool operator()(const std::pair<float, T> &test)
    {
      return test.first > m_match;
    }
    float m_match;
  };

  // Utility methods -----------------------------------------------------------

  //! The default return value is used when no sample points are available.
  //! This defaults to zero, but for some types (for example Quaternion), 
  //! We need more arguments to the constructor. In these cases the method
  //! is specialized for the given T type.
  static T defaultReturnValue() 
  { return T(0); }

  //! The default implementation for linear interpolation. Works for all classes
  //! for which Imath::lerp is implemented (i.e float/double, V2f, V3f).
  //! For other types this method needs to be specialized.
  static T lerp(const Sample &lower, const Sample &upper, const float t) 
  { return Imath::lerp(lower.second, upper.second, t); }

  // Private data members ------------------------------------------------------

  //! Stores the samples that define the curve.
  SampleVec m_samples;

};

//----------------------------------------------------------------------------//

typedef Curve<float>  FloatCurve;
typedef Curve<Color>  ColorCurve;
typedef Curve<Vector> VectorCurve;
typedef Curve<Quat>   QuatCurve;
typedef Curve<Matrix> MatrixCurve;

//----------------------------------------------------------------------------//
// Template implementations
//----------------------------------------------------------------------------//

template <typename T>
Curve<T>::Curve(const size_t numSamples, const T &initialValue)
{
  for (size_t i = 0; i < numSamples; i++) {
    addSample(static_cast<float>(i), initialValue);
  }
}

//----------------------------------------------------------------------------//

template <typename T>
void Curve<T>::addSample(const float t, const T &value)
{
  using namespace std;
  // Find the first sample location that is greater than the interpolation
  // position
  typename SampleVec::iterator i = 
    find_if(m_samples.begin(), m_samples.end(), CheckTGreaterThan(t));
  // If we get something other than end() back then we insert the new
  // sample before that. If there wasn't a larger value we add this sample
  // to the end of the vector.
  if (i != m_samples.end()) {
    m_samples.insert(i, make_pair(t, value));
  } else {
    m_samples.push_back(make_pair(t, value));
  }
}

//----------------------------------------------------------------------------//

template <typename T>
T Curve<T>::interpolate(const float t) const
{
  using namespace std;
  // If there are no samples, return zero
  if (m_samples.size() == 0) {
    return defaultReturnValue();
  }
  // Find the first sample location that is greater than the interpolation
  // position
  typename SampleVec::const_iterator i = 
    find_if(m_samples.begin(), m_samples.end(), CheckTGreaterThan(t));
  // If we get end() back then there was no sample larger, so we return the
  // last value. If we got the first value then there is only one value and
  // we return that.
  if (i == m_samples.end()) {
    return m_samples.back().second;
  } else if (i == m_samples.begin()) {
    return m_samples.front().second;
  } 
  // Interpolate between the nearest two samples.
  const Sample &upper = *i;
  const Sample &lower = *(--i);
  const float interpT = Imath::lerpfactor(t, lower.first, upper.first);
  return lerp(lower, upper, interpT);
}

//----------------------------------------------------------------------------//

template <typename T>
std::vector<float> Curve<T>::samplePoints() const
{
  std::vector<float> result;
  for (typename SampleVec::const_iterator i = m_samples.begin(), 
         end = m_samples.end(); i != end; ++i) {
    result.push_back(i->first);
  }
  return result;
}

//----------------------------------------------------------------------------//

template <typename T>
std::vector<T> Curve<T>::sampleValues() const
{
  std::vector<T> result;
  for (typename SampleVec::const_iterator i = m_samples.begin(), 
         end = m_samples.end(); i != end; ++i) {
    result.push_back(i->second);
  }
  return result;
}

//----------------------------------------------------------------------------//

template <typename T>
void Curve<T>::removeDuplicates()
{
  // New sample vector
  SampleVec newSamples;
  // Vector size
  size_t size = m_samples.size();
  if (size == 1) {
    return;
  }
  // First point
  if (m_samples[0] != m_samples[1]) {
    newSamples.push_back(m_samples[0]);
  }
  // Interior points
  for (size_t i = 1; i < size - 1; i++) {
    if (m_samples[i].second != m_samples[i - 1].second || 
        m_samples[i].second != m_samples[i + 1].second) {
      newSamples.push_back(m_samples[i]);
    } 
  }
  // Last points
  if (m_samples[size - 1] != m_samples[size - 2]) {
    newSamples.push_back(m_samples[size - 1]);
  }
  // Swap contents of m_samples with new sample vector.
  m_samples.swap(newSamples);
}

//----------------------------------------------------------------------------//

template <typename T>
typename Curve<T>::CPtr 
Curve<T>::average(const std::vector<Curve<T>::CPtr> &curves)
{
  typename Curve<T>::Ptr result(new Curve<T>);

  if (curves.size() == 0) {
    return result;
  }

  if (curves.size() == 1) {
    return curves[0];
  }

  // Find first and last sample in all curves
  float first = std::numeric_limits<float>::max();
  float last = std::numeric_limits<float>::min();
  size_t numSamples = 0;
  for (size_t c = 0, numCurves = curves.size(); c < numCurves; ++c) {
    first = std::min(first, curves[c]->samplePoints().front());
    last = std::max(last, curves[c]->samplePoints().back());
    numSamples = std::max(numSamples, curves[c]->samplePoints().size());
  }

  // Average curves
  for (size_t i = 0; i < numSamples; ++i) {
    T value = defaultReturnValue();
    float t = Math::fit(static_cast<float>(i), 0.0f, 
                        static_cast<float>(numSamples - 1), first, last);
    for (size_t c = 0, numCurves = curves.size(); c < numCurves; ++c) {
      value += curves[c]->interpolate(t);
    }
    value *= 1.0 / curves.size();
    result->addSample(t, value);
  }

  return result;
}

//----------------------------------------------------------------------------//

template <typename T>
size_t Curve<T>::numSamples() const
{ 
  return m_samples.size(); 
}

//----------------------------------------------------------------------------//

template <typename T>
const typename Curve<T>::SampleVec& Curve<T>::samples() const
{ 
  return m_samples; 
}

//----------------------------------------------------------------------------//
// Template specializations
//----------------------------------------------------------------------------//

//! Template specialization for matrices. 
//! The default constructor gives us the appropriate identity matrix
template <>
inline Imath::Matrix44<float> 
Curve<Imath::Matrix44<float> >::defaultReturnValue() 
{ 
  Imath::Matrix44<float> identity;
  identity.makeIdentity();
  return identity;
}

//----------------------------------------------------------------------------//

//! Template specialization for quaternionsmatrices. 
//! The default constructor gives us the appropriate identity matrix
template <>
inline Imath::Matrix44<double> 
Curve<Imath::Matrix44<double> >::defaultReturnValue() 
{ 
  Imath::Matrix44<double> identity;
  identity.makeIdentity();
  return identity;
}

//----------------------------------------------------------------------------//

//! Template specialization for quaternions. The default constructor gives
//! us the appropriate default orientation.
template <>
inline Quat Curve<Quat>::defaultReturnValue() 
{
  return Quat();
}

//----------------------------------------------------------------------------//

//! Template specialization for quaternions. We use slerp instead of lerp.
template <>
inline Quat Curve<Quat>::lerp(const Curve<Quat>::Sample &lower, 
                              const Curve<Quat>::Sample &upper, 
                              const float t) 
{
  return Imath::slerp(lower.second, upper.second, static_cast<double>(t));
}

//----------------------------------------------------------------------------//

} // namespace Util
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
