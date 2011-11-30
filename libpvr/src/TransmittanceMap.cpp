//----------------------------------------------------------------------------//

/*! \file TransmittanceMap.cpp
  Contains implementations of TransmittanceMap class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/TransmittanceMap.h"

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
// TransmittanceMap
//----------------------------------------------------------------------------//

TransmittanceMap::Ptr TransmittanceMap::create()
{ 
  return Ptr(new TransmittanceMap); 
}

//----------------------------------------------------------------------------//

TransmittanceMap::Ptr TransmittanceMap::clone() const
{ 
  return Ptr(new TransmittanceMap(*this)); 
}

//----------------------------------------------------------------------------//

void TransmittanceMap::setSize(const size_t width, const size_t height)
{
  m_width = width;
  m_height = height;
  swapClear(m_pixels);
  m_pixels.resize(width * height);
}

//----------------------------------------------------------------------------//

Imath::V2i TransmittanceMap::size() const
{ 
  return Imath::V2i(m_width, m_height); 
}

//----------------------------------------------------------------------------//

void TransmittanceMap::setPixel(const size_t x, const size_t y, 
                                const Util::TransmittanceFunction::CPtr func)
{
  assert(x < m_width && "Pixel x coordinate out of bounds");
  assert(y < m_height && "Pixel y coordinate out of bounds");
  pixel(x, y) = *func;
}
  
//----------------------------------------------------------------------------//

Util::TransmittanceFunction::Ptr 
TransmittanceMap::pixelFunction(const size_t x, const size_t y) const
{
  assert(x < m_width && "Pixel x coordinate out of bounds");
  assert(y < m_height && "Pixel y coordinate out of bounds");
  return Util::TransmittanceFunction::Ptr
    (new Util::TransmittanceFunction(m_pixels[x + y * m_width]));
}


//----------------------------------------------------------------------------//

Color TransmittanceMap::lerp(const float rsX, const float rsY, const float z) const
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

void TransmittanceMap::printStats() const 
{
  using namespace Util;
  
  Log::print("Transmittance map stats:");

  // Count samples
  size_t numSamples = 0;
  BOOST_FOREACH (const Util::TransmittanceFunction &p, m_pixels) {
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

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
