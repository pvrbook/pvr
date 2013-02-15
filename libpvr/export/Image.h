//-*-c++-*--------------------------------------------------------------------//

/*
    This file is part of PVR. Copyright (C) 2012 Magnus Wrenninge

    PVR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PVR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//----------------------------------------------------------------------------//

/*! \file Image.h
  Contains the Image class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_IMAGE_H__
#define __INCLUDED_PVR_IMAGE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

#include <OpenImageIO/imagebuf.h>

// Project headers

#include "pvr/export.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

/*! \class Image
  \brief Wraps a very simple image class.

  Because PVR doesn't need any fancy image operations, this class is only used
  to store a simple RGBA images, and is not focused on efficiency. Although
  the internal representation of the image is RGBA, it can be written to disk
  as RGB by passing Channels::RGB to the write() method.
*/

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC Image
{
public:
  
  PVR_TYPEDEF_SMART_PTRS(Image);

  // Enums ---------------------------------------------------------------------

  //! Refers to the number of channels in an image. Primarily used to 
  //! determine which channels of the internal RGBA representation are written
  //! to the output file in write().
  enum Channels {
    RGB,
    RGBA
  };

  // Structs -------------------------------------------------------------------

  struct pixel_iterator;
  
  // Constructor, destructor, factory ------------------------------------------

  //! Factory method. Use this whenever the lifespan of the image needs
  //! to be managed
  static Ptr create();

  //! Clones the instance. Will clear all the non-const data members but keeps
  //! pointers to const data members
  Ptr        clone();

  // Main methods --------------------------------------------------------------

  //! Sets the size/resolution of the image
  void       setSize(const size_t width, const size_t height);

  //! Sets the value of a given pixel
  void       setPixel(const size_t x, const size_t y, const Color &value);
  //! Sets the alpha value of a given pixel
  void       setPixelAlpha(const size_t x, const size_t y, const float value);

  //! Returns the size of the image
  Imath::V2i size() const;

  //! Returns the value of a given pixel.
  Color      pixel(const size_t x, const size_t y) const;
  //! Returns the alpha value of a given pixel
  float      pixelAlpha(const size_t x, const size_t y) const;

  //! Writes the image to disk. The filename extension may be any format
  //! supported by OpenImageIO.
  void       write(const std::string &filename, Channels channels) const;

  // Iteration -----------------------------------------------------------------

  //! Returns iterator to first pixel
  pixel_iterator begin();
  //! Returns iterator to one element past last pixel
  pixel_iterator end();

private:

  // Private data members ------------------------------------------------------

  //! Internal representation of RGBA image.
  OpenImageIO::ImageBuf m_buf;

};

//----------------------------------------------------------------------------//

//! Used to conveniently iterate over all the pixels in an image
//! \note Not a proper STL iterator
struct LIBPVR_PUBLIC Image::pixel_iterator
{
  // Public methods
  pixel_iterator(Image &image, const size_t xPos, const size_t yPos);
  const pixel_iterator& operator ++ ();
  bool operator != (const pixel_iterator &rhs) const;
  void setPixel(const Color &color);
  void setPixelAlpha(const float alpha);
  float progress() const;
  float rsX() const;
  float rsY() const;
  // Public data members
  size_t x, y;
private:
  Image& m_image;
  Imath::Vec2<size_t> m_size;
};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
