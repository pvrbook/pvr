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

#include <pvr/Camera.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace pvr;
using namespace pvr::Render;

//----------------------------------------------------------------------------//

int main()
{
  PerspectiveCamera::Ptr cam(new PerspectiveCamera);
  
  Vector wsP, csP, rsP, ssP;
  PTime time(0.0);

  for (double t = -2.0; t < 2.0; t += 0.1) {
    wsP.setValue(0.0, 0.0, t);
    cout << "wsP " << wsP << endl
         << "  csP " << cam->worldToCamera(wsP, time) << endl
         << "  ssP " << cam->worldToScreen(wsP, time) << endl
         << "  rsP " << cam->worldToCamera(rsP, time) << endl;
  }
  ssP.setValue(0.0, 0.0, 1.0);
  cout << "ssP " << ssP << endl
       << "  wsP " << cam->screenToWorld(ssP, time) << endl;
}

//----------------------------------------------------------------------------//
