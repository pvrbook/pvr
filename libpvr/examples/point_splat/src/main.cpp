//-*-c++-*--------------------------------------------------------------------//

/* This example shows some use of the AttrTable and related classes.
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

//----------------------------------------------------------------------------//

int main()
{
  DenseBuffer::Ptr buffer(new DenseBuffer);
  buffer->setSize(V3i(5));
  buffer->clear(V3f(0.0));

  cout << "writePoint()" << endl;
  writePoint(Vector(0.5), V3f(1.0), buffer);
  cout << "  " << buffer->value(0, 0, 0) << endl;

  buffer->clear(V3f(0.0));
  cout << "writeAntialiasedPoint()" << endl;
  writeAntialiasedPoint(Vector(0.7, 0.5, 0.5), V3f(1.0), buffer);
  cout << "  " << buffer->value(0, 0, 0) << endl
       << "  " << buffer->value(1, 0, 0) << endl;
}

//----------------------------------------------------------------------------//
