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

#include <pvr/CubicInterp.h>
#include <pvr/GaussianInterp.h>
#include <pvr/MitchellInterp.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace Field3D;
using namespace pvr::Filter;

//----------------------------------------------------------------------------//

int main()
{
  const size_t bufSize    = 100;
  const size_t numSamples = 1000000;

  DenseField<float> buffer;
  buffer.setSize(V3i(bufSize));
  std::fill(buffer.begin(), buffer.end(), 1.0f);

  {
    boost::timer timer;
    Rand32 rng;
    V3f vsP;
    float floatBufSize = bufSize;
    float val = 0.0;
    for (size_t i = 0; i < numSamples; i++) {
      vsP.x = rng.nextf() * floatBufSize;
      vsP.y = rng.nextf() * floatBufSize;
      vsP.z = rng.nextf() * floatBufSize;
      V3i dvsP = contToDisc(vsP);
      val += buffer.fastValue(dvsP.x, dvsP.y, dvsP.z);
    }
    cout << "Nearest neighbor\t" << timer.elapsed() << endl;
  }
  {
    boost::timer timer;
    Rand32 rng;
    V3f vsP;
    float floatBufSize = bufSize;
    float val = 0.0;
    DenseField<float>::LinearInterp interp;
    for (size_t i = 0; i < numSamples; i++) {
      vsP.x = rng.nextf() * floatBufSize;
      vsP.y = rng.nextf() * floatBufSize;
      vsP.z = rng.nextf() * floatBufSize;
      val += interp.sample(buffer, vsP);
    }
    cout << "Linear \t\t\t" << timer.elapsed() << endl;
  }
  {
    boost::timer timer;
    Rand32 rng;
    V3f vsP;
    float floatBufSize = bufSize;
    float val = 0.0;
    DenseField<float>::CubicInterp interp;
    for (size_t i = 0; i < numSamples; i++) {
      vsP.x = rng.nextf() * floatBufSize;
      vsP.y = rng.nextf() * floatBufSize;
      vsP.z = rng.nextf() * floatBufSize;
      val += interp.sample(buffer, vsP);
    }
    cout << "Cubic \t\t\t" << timer.elapsed() << endl;
  }
  {
    boost::timer timer;
    Rand32 rng;
    V3f vsP;
    float floatBufSize = bufSize;
    float val = 0.0;
    TriCubicFieldInterp<float> interp;
    for (size_t i = 0; i < numSamples; i++) {
      vsP.x = rng.nextf() * floatBufSize;
      vsP.y = rng.nextf() * floatBufSize;
      vsP.z = rng.nextf() * floatBufSize;
      val += interp.sample(buffer, vsP);
    }
    cout << "TriCubic \t\t" << timer.elapsed() << endl;
  }
  {
    boost::timer timer;
    Rand32 rng;
    V3f vsP;
    float floatBufSize = bufSize;
    float val = 0.0;
    GaussianFieldInterp<float> interp;
    for (size_t i = 0; i < numSamples; i++) {
      vsP.x = rng.nextf() * floatBufSize;
      vsP.y = rng.nextf() * floatBufSize;
      vsP.z = rng.nextf() * floatBufSize;
      val += interp.sample(buffer, vsP);
    }
    cout << "Gauss \t\t\t" << timer.elapsed() << endl;
  }
  {
    boost::timer timer;
    Rand32 rng;
    V3f vsP;
    float floatBufSize = bufSize;
    float val = 0.0;
    MitchellFieldInterp<float> interp;
    for (size_t i = 0; i < numSamples; i++) {
      vsP.x = rng.nextf() * floatBufSize;
      vsP.y = rng.nextf() * floatBufSize;
      vsP.z = rng.nextf() * floatBufSize;
      val += interp.sample(buffer, vsP);
    }
    cout << "Mitchell \t\t" << timer.elapsed() << endl;
  }
}

//----------------------------------------------------------------------------//
