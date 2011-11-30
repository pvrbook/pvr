//-*-c++-*--------------------------------------------------------------------//

/*! \file TransmittanceMap.h
  Contains the TransmittanceMap class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_TRANSMITTANCEMAP_H__
#define __INCLUDED_PVR_TRANSMITTANCEMAP_H__

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
// TransmittanceMap
//----------------------------------------------------------------------------//

/*! \class TransmittanceMap 
  \brief Stores a 2d array of transmittance functions (a deep depth map)

 */

//----------------------------------------------------------------------------//

class TransmittanceMap
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(TransmittanceMap);

  // Constructor, destructor, factory ------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  static Ptr create();

  //! Clones the instance. Will clear all the non-const data members but keeps
  //! pointers to const data members
  Ptr clone() const;

  // Main methods --------------------------------------------------------------

  //! Sets the size of the map
  void setSize(const size_t width, const size_t height);

  //! Returns the size of the map
  Imath::V2i size() const;

  //! Sets the transmittance function of a pixel
  void setPixel(const size_t x, const size_t y, 
                const Util::TransmittanceFunction::CPtr func);

  Util::TransmittanceFunction::Ptr 
  pixelFunction(const size_t x, const size_t y) const;

  //! Interpolated transmittance at a given raster coordinate and depth.
  Color lerp(const float rsX, const float rsY, const float z) const;

  //! Prints statistics about the transmittance map
  void printStats() const;

private:
  
  // Typedefs ------------------------------------------------------------------

  typedef std::vector<Util::TransmittanceFunction> PixelsVec;

  // Utility methods -----------------------------------------------------------

  //! Returns a const reference to the given pixel's transmittance function
  const Util::TransmittanceFunction& pixel(const size_t x, 
                                           const size_t y) const;

  //! Returns a reference to the given pixel's transmittance function.
  Util::TransmittanceFunction& pixel(const size_t x, 
                                     const size_t y);

  // Private data members ------------------------------------------------------

  //! Stores the samples that define the TransmittanceMap.
  PixelsVec  m_pixels;
  size_t     m_width;
  size_t     m_height;

};

//----------------------------------------------------------------------------//
// Inline methods
//----------------------------------------------------------------------------//

inline const Util::TransmittanceFunction& 
TransmittanceMap::pixel(const size_t x, const size_t y) const
{
  assert(x >= 0 && "TransmittanceMap::pixel(): x less than zero");
  assert(y >= 0 && "TransmittanceMap::pixel(): y less than zero");
  assert(x < m_width  && "TransmittanceMap::pixel(): x out of range");
  assert(y < m_height && "TransmittanceMap::pixel(): y out of range");
  return m_pixels[x + y * m_width];
}

//----------------------------------------------------------------------------//

inline Util::TransmittanceFunction& 
TransmittanceMap::pixel(const size_t x, const size_t y) 
{
  assert(x >= 0 && "TransmittanceMap::pixel(): x less than zero");
  assert(y >= 0 && "TransmittanceMap::pixel(): y less than zero");
  assert(x < m_width  && "TransmittanceMap::pixel(): x out of range");
  assert(y < m_height && "TransmittanceMap::pixel(): y out of range");
  return m_pixels[x + y * m_width];
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
