//----------------------------------------------------------------------------//

/*! \file DeepImage.cpp
  Contains implementations of DeepImage class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/DeepImage.h"

// Library includes

#include <OpenEXR/ImathFun.h>

// Project includes

#include "pvr/Log.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// DeepImage
//----------------------------------------------------------------------------//

DeepImage::DeepImage()
  : m_numSamples(32)
{
  setSize(2, 2);
}

//----------------------------------------------------------------------------//

DeepImage::Ptr DeepImage::create()
{ 
  return Ptr(new DeepImage); 
}

//----------------------------------------------------------------------------//

DeepImage::Ptr DeepImage::clone() const
{ 
  return Ptr(new DeepImage(*this)); 
}

//----------------------------------------------------------------------------//

void DeepImage::setSize(const size_t width, const size_t height)
{
  m_width = width;
  m_height = height;
  swapClear(m_pixels);
  m_pixels.resize(width * height);
}

//----------------------------------------------------------------------------//

Imath::V2i DeepImage::size() const
{ 
  return Imath::V2i(m_width, m_height); 
}

//----------------------------------------------------------------------------//

void DeepImage::setNumSamples(const size_t numSamples)
{
  m_numSamples = numSamples;
}

//----------------------------------------------------------------------------//

size_t DeepImage::numSamples() const
{
  return m_numSamples;
}


//----------------------------------------------------------------------------//

void DeepImage::setPixel(const size_t x, const size_t y, 
                         const Util::ColorCurve::CPtr func)
{
  assert(x < m_width && "Pixel x coordinate out of bounds");
  assert(y < m_height && "Pixel y coordinate out of bounds");
  assert(func != NULL && "Got null pointer for pixel function");
  Util::ColorCurve::Ptr fixedSampleCurve = makeFixedSample(func,
                                                           m_numSamples);
  pixel(x, y) = *fixedSampleCurve;
}
  
//----------------------------------------------------------------------------//

void DeepImage::setPixel(const size_t x, const size_t y, 
                         const Color& value)
{
  assert(x < m_width && "Pixel x coordinate out of bounds");
  assert(y < m_height && "Pixel y coordinate out of bounds");
  pixel(x, y) = *makeFixedSample(ColorCurve::Ptr(new ColorCurve(value)),
                                 m_numSamples);
}
  
//----------------------------------------------------------------------------//

Util::ColorCurve::Ptr 
DeepImage::pixelFunction(const size_t x, const size_t y) const
{
  assert(x < m_width && "Pixel x coordinate out of bounds");
  assert(y < m_height && "Pixel y coordinate out of bounds");
  return Util::ColorCurve::Ptr
    (new Util::ColorCurve(m_pixels[x + y * m_width]));
}


//----------------------------------------------------------------------------//

Color DeepImage::lerp(const float rsX, const float rsY, const float z) const
{
  const size_t zero = 0;
  size_t xMin = std::floor(rsX);
  size_t xMax = std::ceil(rsX);
  size_t yMin = std::floor(rsY);
  size_t yMax = std::ceil(rsY);
  xMin = Imath::clamp(xMin, zero, m_width - 1);
  xMax = Imath::clamp(xMax, zero, m_width - 1);
  yMin = Imath::clamp(yMin, zero, m_height - 1);
  yMax = Imath::clamp(yMax, zero, m_height - 1);
  return Util::lerp2D(rsX - static_cast<float>(xMin), 
                      rsY - static_cast<float>(yMin), 
                      pixel(xMin, yMin).interpolate(z),
                      pixel(xMax, yMin).interpolate(z),
                      pixel(xMin, yMax).interpolate(z),
                      pixel(xMax, yMax).interpolate(z));
}

//----------------------------------------------------------------------------//

void DeepImage::printStats() const 
{
  using namespace Util;
  
  Log::print("Deep image stats:");

  // Count samples
  size_t numSamples = 0;
  BOOST_FOREACH (const Util::ColorCurve &p, m_pixels) {
    numSamples += p.numSamples();
  }

  // Average samples/pixel
  const size_t numPixels = m_width * m_height;
  const float avg = static_cast<float>(numSamples) / numPixels;
  Log::print("  Average # samples per pixel: " + str(avg));

  // Memory use
  size_t bytesUsed = numSamples * (sizeof(float) + sizeof(Color));
  float mbUsed = static_cast<float>(bytesUsed) / (1024.0f * 1024.0f);
  Log::print("  Approximate memory use: " + str(mbUsed) + " MB");
}

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

Util::ColorCurve::Ptr makeFixedSample(Util::ColorCurve::CPtr curve,
                                      const size_t numSamples) 
{
  using namespace Math;

  typedef ColorCurve::SampleVec SampleVec;
  typedef SampleVec::const_iterator CIter;
  
  const ColorCurve::SampleVec &samples = curve->samples();

  // Handle case of no samples
  if (samples.size() == 0) {
    return ColorCurve::Ptr(new ColorCurve(numSamples, Color(0.0)));
  }

  // Handle case of zero samples
  if (samples.size() == 1) {
    return ColorCurve::Ptr(new ColorCurve(numSamples, 
                                          samples[0].second));
  }

  Color first = samples.front().second;
  Color last  = samples.back().second;
  int   sign  = first.x >= last.x ? 1 : -1;

  // Check that function is monotonic
  bool  monotonic = true;
  Color lastVal   = samples[0].second;
  for (size_t i = 1; i < samples.size(); ++i) {
    Color value = samples[i].second;
    if (value.x > sign * lastVal.x || 
        value.y > sign * lastVal.y || 
        value.z > sign * lastVal.z) {
      Log::warning("Non-monotonic curve in DeepImage::makeFixedSample()");
      return makeFixedSample(ColorCurve::Ptr(new ColorCurve(Color(0.0))),
                             numSamples);
    }
    last = value;
  }

  // If function is monotonic, divide equally over the range of transmittance
  // If the function is non-monotonic, divide equally in depth.
  if (monotonic) {
    ColorCurve::Ptr result(new ColorCurve);
    result->addSample(samples[0].first, samples[0].second);
    lastVal = samples[0].second;
    size_t p = 0;
    for (size_t i = 1; i < numSamples; ++i) {
      float t = static_cast<float>(i) / static_cast<float>(numSamples - 1);
      Color value = fit01(t, first, last);
      while (avg(samples[p].second) > avg(value) * sign) {
        lastVal = samples[p].second;
        p++;
      }
      float factor = Imath::lerpfactor(avg(value), avg(lastVal), 
                                       avg(samples[p].second));
      Color interp = fit01(factor, lastVal, samples[p].second);
      float position = fit01(factor, samples[p-1].first, samples[p].first);
      result->addSample(position, interp);
    }
    return result;
  } else {
    //! \todo Finish
    return ColorCurve::Ptr(new ColorCurve(*curve));;
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
