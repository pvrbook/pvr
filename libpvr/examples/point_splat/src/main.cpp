//-*-c++-*--------------------------------------------------------------------//

/* 
 * 
 * 
 *
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include <iostream>

#include <boost/timer.hpp>

#include <OpenEXR/ImathRandom.h>

#include <Field3D/DenseField.h>
#include <Field3D/DenseField.h>

#include <pvr/Image.h>
#include <pvr/VoxelBuffer.h>
#include <pvr/ModelingUtils.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace Field3D;
using namespace pvr;
using namespace pvr::Model;
using namespace pvr::Render;

//----------------------------------------------------------------------------//

void writeSliceAsImage(DenseBuffer::Ptr buffer, int res, int slice, 
                       const std::string &filename)
{
  Image::Ptr image = Image::create();
  image->setSize(res, res);
  for (Image::pixel_iterator i = image->begin(), end = image->end();
       i != end; ++i) {
    i.setPixel(buffer->value(i.x, i.y, slice));
  }
  image->write(filename, Image::RGB);
}

//----------------------------------------------------------------------------//

int main()
{
  int res = 200;
  int numPoints = 500; 

  Imath::Rand32 rng(0);

  DenseBuffer::Ptr buffer(new DenseBuffer);
  buffer->setSize(V3i(res));
  buffer->clear(V3f(0.0));

  for (int i = 0; i < numPoints; i++) {
    float x = rng.nextf() * res;
    float y = rng.nextf() * res;
    float z = 0.5;
    writePoint(Vector(x, y, z), V3f(1.0), buffer);
  }
  writeSliceAsImage(buffer, res, 0, "nonantialiased.png");
  
  buffer->clear(V3f(0.0));
  rng.init(0);

  for (int i = 0; i < numPoints; i++) {
    float x = rng.nextf() * res;
    float y = rng.nextf() * res;
    float z = 0.5;
    writeAntialiasedPoint(Vector(x, y, z), V3f(1.0), buffer);
  }
  writeSliceAsImage(buffer, res, 0, "antialiased.png");
  
  buffer->clear(V3f(0.0));
  rng.init(0);

  for (int i = 0; i < numPoints; i++) {
    float x = rng.nextf() * res;
    float y = rng.nextf() * res;
    float z = 0.5;
    float xe = x + (rng.nextf() - 0.5) * 0.1 * res;
    float ye = y + (rng.nextf() - 0.5) * 0.1 * res;
    writeLine<true>(Vector(x, y, z), Vector(xe, ye, z), V3f(1.0), buffer);
  }
  writeSliceAsImage(buffer, res, 0, "lines.png");
  
  // High point count

  numPoints = numPoints * 1000; 
  float density = 1.0 / 100;

   buffer->clear(V3f(0.0));

  for (int i = 0; i < numPoints; i++) {
    float x = rng.nextf() * res;
    float y = rng.nextf() * res;
    float z = 0.5;
    writePoint(Vector(x, y, z), V3f(density), buffer);
  }
  writeSliceAsImage(buffer, res, 0, "nonantialiased-hi.png");

  buffer->clear(V3f(0.0));
  rng.init(0);

  for (int i = 0; i < numPoints; i++) {
    float x = rng.nextf() * res;
    float y = rng.nextf() * res;
    float z = 0.5;
    writeAntialiasedPoint(Vector(x, y, z), V3f(density), buffer);
  }
  writeSliceAsImage(buffer, res, 0, "antialiased-hi.png"); 
}

//----------------------------------------------------------------------------//
