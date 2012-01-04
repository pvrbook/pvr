//----------------------------------------------------------------------------//

/*! \file PyCamera.cpp
  Contains the interface definition for the Camera class
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

#include <pvr/Camera.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;
using namespace pvr::Util;

using pvr::Render::Camera;
using pvr::Render::PerspectiveCamera;
using pvr::Render::calculateVerticalFOV;

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

template <typename T>
struct BindCurve
{
  static boost::python::list samplePoints(Curve<T> &self)
  { return vecToList(self.samplePoints()); }

  static boost::python::list sampleValues(Curve<T> &self)
  { return vecToList(self.sampleValues()); }

  static void bind(const char *name)
  {
    using namespace boost::python;

    class_<Curve<T>, typename Curve<T>::Ptr>(name, no_init)
      .def("__init__",     make_constructor(Curve<T>::create))
      .def("addSample",    &Curve<T>::addSample)
      .def("interpolate",  &Curve<T>::interpolate)
      .def("samplePoints", &samplePoints)
      .def("sampleValues", &sampleValues)
      ;

    implicitly_convertible<T, Curve<T> >();
    implicitly_convertible<typename Curve<T>::Ptr, typename Curve<T>::CPtr>();
  }
};

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportCurve()
{
  using namespace boost::python;

  BindCurve<float>::bind("FloatCurve");
  BindCurve<Color>::bind("ColorCurve");
  BindCurve<Vector>::bind("VectorCurve");
  BindCurve<Quat>::bind("QuatCurve");
}

//----------------------------------------------------------------------------//

void exportPerspectiveCamera()
{
  using namespace boost::python;

  class_<Camera, Camera::Ptr, boost::noncopyable>
    ("Camera", no_init)
    .def("setPosition",    &PerspectiveCamera::setPosition)
    .def("setOrientation", &PerspectiveCamera::setOrientation)
    .def("setResolution",  &PerspectiveCamera::setResolution)
    .def("worldToCamera",  &PerspectiveCamera::worldToCamera)
    .def("cameraToWorld",  &PerspectiveCamera::cameraToWorld)
    .def("setNumTimeSamples", &Camera::setNumTimeSamples)
    ;

  implicitly_convertible<Camera::Ptr, Camera::CPtr>();

  class_<PerspectiveCamera, bases<Camera>, PerspectiveCamera::Ptr>
    ("PerspectiveCamera", no_init)
    .def("__init__",       make_constructor(PerspectiveCamera::create))
    .def("setClipPlanes",  &PerspectiveCamera::setClipPlanes)
    .def("setVerticalFOV", &PerspectiveCamera::setVerticalFOV)
    .def("worldToScreen",  &PerspectiveCamera::worldToScreen)
    .def("screenToWorld",  &PerspectiveCamera::screenToWorld)
    .def("worldToRaster",  &PerspectiveCamera::worldToRaster)
    .def("rasterToWorld",  &PerspectiveCamera::rasterToWorld)
    ;

  implicitly_convertible<PerspectiveCamera::Ptr, PerspectiveCamera::CPtr>();

}

//----------------------------------------------------------------------------//

void exportCameraFunctions()
{
  using namespace boost::python;

  def("calculateVerticalFOV", &calculateVerticalFOV);
}

//----------------------------------------------------------------------------//
