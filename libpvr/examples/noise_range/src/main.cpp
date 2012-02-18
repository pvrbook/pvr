//-*-c++-*--------------------------------------------------------------------//

/* This example tries to find the value range of perlin noise
 * 
 * 
 *
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include <iostream>

#include <OpenEXR/ImathRandom.h>

#include <pvr/Noise/Noise.h>
#include <pvr//Types.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;
using namespace pvr::Noise;

//----------------------------------------------------------------------------//

int main()
{
  Imath::Rand48 rng;

  float min = std::numeric_limits<float>::max(); 
  float max = -std::numeric_limits<float>::max();

  PerlinNoise::Ptr perlin(new PerlinNoise);

  for (int i = 0; i < 1000000; i++) {
    Vector p;
    p.x = rng.nextf() * 100;
    p.y = rng.nextf() * 100;
    p.z = rng.nextf() * 100;
    float noise = perlin->eval(p.x, p.y, p.z);
    min = std::min(min, noise);
    max = std::max(max, noise);
  }
  
  cout << "min: " << min << endl
       << "max: " << max << endl;
}

//----------------------------------------------------------------------------//
