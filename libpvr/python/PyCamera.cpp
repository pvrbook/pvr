//----------------------------------------------------------------------------//

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

/*! \file PyCamera.cpp
  Contains the interface definition for the Camera class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Needs to be first, to avoid macro "tolower" passed 2 arguments, on OSX, python 2.7
#include <Python.h>

// System includes

#include <boost/python.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Library includes

#include <pvr/Camera.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;

using namespace pvr;
using namespace pvr::Util;
using namespace pvr::Render;

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
  BindCurve<Matrix>::bind("MatrixCurve");
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

  class_<SphericalCamera, bases<Camera>, SphericalCamera::Ptr>
    ("SphericalCamera", no_init)
    .def("__init__",       make_constructor(SphericalCamera::create))
    ;

  implicitly_convertible<SphericalCamera::Ptr, SphericalCamera::CPtr>();

}

//----------------------------------------------------------------------------//

void exportCameraFunctions()
{
  using namespace boost::python;

  def("calculateVerticalFOV", &calculateVerticalFOV);
}

//----------------------------------------------------------------------------//
