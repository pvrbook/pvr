//----------------------------------------------------------------------------//

/*! \file PyNoise.cpp
  Contains the interface definition for the Noise class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/python.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Library includes

#include <pvr/Noise/Noise.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace boost::python;
using namespace pvr;
using namespace pvr::Noise;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

inline double perlinHelper1(const float x)
{
  SNoise noise;
  float result;
  noise(result, x);
  return result;
}

//----------------------------------------------------------------------------//

inline double perlinHelper2(const float x, const float y)
{
  SNoise noise;
  float result;
  noise(result, x, y);
  return result;
}

//----------------------------------------------------------------------------//

inline double perlinHelper3(const float x, const float y, const float z)
{
  SNoise noise;
  float result;
  noise(result, Vector(x, y, z));
  return result;
}

//----------------------------------------------------------------------------//

inline double perlinHelperV(const Vector &p)
{
  SNoise noise;
  float result;
  noise(result, p);
  return result;
}

//----------------------------------------------------------------------------//

inline double noiseFunctionHelper1(const NoiseFunction &self, const float x)
{
  return self.eval(x);
}

//----------------------------------------------------------------------------//

inline double noiseFunctionHelper2(const NoiseFunction &self, const float x,
                                   const float y)
{
  return self.eval(x, y);
}

//----------------------------------------------------------------------------//

inline double noiseFunctionHelper3(const NoiseFunction &self, const float x,
                                   const float y, const float z)
{
  return self.eval(x, y, z);
}

//----------------------------------------------------------------------------//

inline double noiseFunctionHelperV(const NoiseFunction &self, 
                                   const Vector &p)
{
  return self.eval(p);
}

//----------------------------------------------------------------------------//

inline double fractalHelper1(const Fractal &self, const float x)
{
  return self.eval(x);
}

//----------------------------------------------------------------------------//

inline double fractalHelper2(const Fractal &self, const float x,
                             const float y)
{
  return self.eval(x, y);
}

//----------------------------------------------------------------------------//

inline double fractalHelper3(const Fractal &self, const float x,
                             const float y, const float z)
{
  return self.eval(x, y, z);
}

//----------------------------------------------------------------------------//

inline double fractalHelperV(const Fractal &self, 
                             const Vector &p)
{
  return self.eval(p);
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportNoiseFunctions()
{
  def("perlin", &perlinHelper1);
  def("perlin", &perlinHelper2);
  def("perlin", &perlinHelper3);
  def("perlin", &perlinHelperV);
}

//----------------------------------------------------------------------------//

void exportNoiseClasses()
{

  // Range

  class_<std::pair<float, float> >("Range")
    .def_readwrite("first", &pair<float, float>::first)
    .def_readwrite("second", &pair<float, float>::second)
    ;

  // NoiseFunction ---

  class_<NoiseFunction, NoiseFunction::Ptr, boost::noncopyable>
    ("NoiseFunction", no_init)
    .def("range", &NoiseFunction::range)
    .def("eval",  &noiseFunctionHelper1)
    .def("eval",  &noiseFunctionHelper2)
    .def("eval",  &noiseFunctionHelper3)
    .def("eval",  &noiseFunctionHelperV)
    ;
  implicitly_convertible<NoiseFunction::Ptr, NoiseFunction::CPtr>();

  // Fractal ---

  class_<Fractal, Fractal::Ptr, boost::noncopyable>
    ("Fractal", no_init)
    .def("range", &Fractal::range)
    .def("eval",  &fractalHelper1)
    .def("eval",  &fractalHelper2)
    .def("eval",  &fractalHelper3)
    .def("eval",  &fractalHelperV)
    ;
  implicitly_convertible<Fractal::Ptr, Fractal::CPtr>();

  // NoiseFunction subclasses ---

  class_<PerlinNoise, PerlinNoise::Ptr, bases<NoiseFunction> >
    ("PerlinNoise", no_init)
    .def("__init__", make_constructor(PerlinNoise::create))
    ;
  class_<AbsPerlinNoise, AbsPerlinNoise::Ptr, bases<NoiseFunction> >
    ("AbsPerlinNoise", no_init)
    .def("__init__", make_constructor(AbsPerlinNoise::create))
    ;

  // Fractal subclasses ---

  class_<fBm, fBm::Ptr, bases<Fractal> >("fBm", no_init)
    // .def(init<NoiseFunction::CPtr, float, float, float, float, float>()).
    .def("__init__", make_constructor(fBm::create))
    ;

}

//----------------------------------------------------------------------------//
