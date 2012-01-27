//-*-c++-*--------------------------------------------------------------------//

/*! \file DeepImage.h
  Contains the DeepImage class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_DEEPIMAGE_H__
#define __INCLUDED_PVR_DEEPIMAGE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <boost/foreach.hpp>

// Project headers

#include "pvr/Curve.h"
#include "pvr/Interpolation.h"
#include "pvr/Log.h"
#include "pvr/StlUtil.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// DeepImage
//----------------------------------------------------------------------------//

/*! \class DeepImage 
  \brief Stores a 2d array of Curve<Color> (a deep image).
  PVR uses a fixed number of samples per pixel.
 */

//----------------------------------------------------------------------------//

class DeepImage
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(DeepImage);

  // Constructor, destructor, factory ------------------------------------------

  //! Default constructor. Creates a 2x2 image.
  DeepImage();

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  static Ptr create();

  //! Clones the instance. Will clear all the non-const data members but keeps
  //! pointers to const data members
  Ptr clone() const;

  // Main methods --------------------------------------------------------------

  //! Sets the size of the image
  void setSize(const size_t width, const size_t height);

  //! Returns the size of the image
  Imath::V2i size() const;

  //! Sets the number of samples to use per pixel 
  void setNumSamples(const size_t numSamples);
  //! Returns the number of samples per pixel
  size_t numSamples() const;

  //! Sets the transmittance function of a pixel
  void setPixel(const size_t x, const size_t y, 
                const Util::ColorCurve::CPtr func);
  //! Sets the transmittance function of a pixel to a single value
  void setPixel(const size_t x, const size_t y, 
                const Color &value);

  Util::ColorCurve::Ptr 
  pixelFunction(const size_t x, const size_t y) const;

  //! Interpolated transmittance at a given raster coordinate and depth.
  Color lerp(const float rsX, const float rsY, const float z) const;

  //! Prints statistics about the image
  void printStats() const;

private:
  
  // Typedefs ------------------------------------------------------------------

  typedef std::vector<Util::ColorCurve> PixelsVec;

  // Utility methods -----------------------------------------------------------

  //! Returns a const reference to the given pixel's color curve
  const Util::ColorCurve& pixel(const size_t x, const size_t y) const;

  //! Returns a reference to the given pixel's color curve.
  Util::ColorCurve& pixel(const size_t x, const size_t y);

  // Private data members ------------------------------------------------------

  //! Stores the samples that define the DeepImage.
  PixelsVec  m_pixels;
  //! Width of image
  size_t     m_width;
  //! Height of image
  size_t     m_height;
  //! Number of samples per pixel
  size_t     m_numSamples;

};

//----------------------------------------------------------------------------//
// Inline methods
//----------------------------------------------------------------------------//

inline const Util::ColorCurve& 
DeepImage::pixel(const size_t x, const size_t y) const
{
  assert(x >= 0 && "DeepImage::pixel(): x less than zero");
  assert(y >= 0 && "DeepImage::pixel(): y less than zero");
  assert(x < m_width  && "DeepImage::pixel(): x out of range");
  assert(y < m_height && "DeepImage::pixel(): y out of range");
  return m_pixels[x + y * m_width];
}

//----------------------------------------------------------------------------//

inline Util::ColorCurve& 
DeepImage::pixel(const size_t x, const size_t y) 
{
  assert(x >= 0 && "DeepImage::pixel(): x less than zero");
  assert(y >= 0 && "DeepImage::pixel(): y less than zero");
  assert(x < m_width  && "DeepImage::pixel(): x out of range");
  assert(y < m_height && "DeepImage::pixel(): y out of range");
  return m_pixels[x + y * m_width];
}

//----------------------------------------------------------------------------//
// Utility Functions
//----------------------------------------------------------------------------//

//! Converts a ColorCurve to a curve with a fixed number of samples
Util::ColorCurve::Ptr makeFixedSample(Util::ColorCurve::CPtr curve,
                                      const size_t numSamples);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
