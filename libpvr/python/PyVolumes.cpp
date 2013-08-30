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

/*! \file PyVolumes.cpp
  Contains the interface definition for the Volumes subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Needs to be first, to avoid macro "tolower" passed 2 arguments, on OSX, python 2.7
#include <Python.h>

// System includes
#include "Common.h"


#include <map>

// Library includes

#include <pvr/Volumes/FractalCloud.h>
#include <pvr/Volumes/CompositeVolume.h>
#include <pvr/Volumes/ConstantVolume.h>
#include <pvr/Volumes/VoxelVolume.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportVolumes()
{
  using namespace boost::python;
  using namespace pvr::Render;

  // Volume ---

  class_<Volume, Volume::Ptr, boost::noncopyable>
    ("Volume", no_init)
    .def("typeName",         &Volume::typeName)
    .def("setPhaseFunction", &Volume::setPhaseFunction)
    .def("phaseFunction",    &Volume::phaseFunction)
    ;

  implicitly_convertible<Volume::Ptr, Volume::CPtr>();

  // VoxelVolume ---

  enum_<VoxelVolume::InterpType>("InterpType")
    .value("NoInterp",             VoxelVolume::NoInterp)
    .value("LinearInterp",         VoxelVolume::LinearInterp)
    .value("CubicInterp",          VoxelVolume::CubicInterp)
    .value("MonotonicCubicInterp", VoxelVolume::MonotonicCubicInterp)
    .value("GaussianInterp",       VoxelVolume::GaussianInterp)
    .value("MitchellInterp",       VoxelVolume::MitchellInterp)
    ;

  class_<VoxelVolume, bases<Volume>, VoxelVolume::Ptr>
    ("VoxelVolume")
    .def("__init__",         make_constructor(VoxelVolume::create))
    .def("load",             &VoxelVolume::load)
    .def("setBuffer",        &VoxelVolume::setBuffer)
    .def("addAttribute",     &VoxelVolume::addAttribute)
    .def("setInterpolation", &VoxelVolume::setInterpolation)
    .def("setUseEmptySpaceOptimization", &VoxelVolume::setUseEmptySpaceOptimization)
    ;

  implicitly_convertible<VoxelVolume::Ptr, VoxelVolume::CPtr>();

  // CompositeVolume ---

  class_<CompositeVolume, bases<Volume>, CompositeVolume::Ptr>
    ("CompositeVolume")
    .def("__init__", make_constructor(CompositeVolume::create))
    .def("add",      &CompositeVolume::add)
    ;

  implicitly_convertible<CompositeVolume::Ptr, CompositeVolume::CPtr>();

  // ConstantVolume ---

  class_<ConstantVolume, bases<Volume>, ConstantVolume::Ptr>
    ("ConstantVolume", no_init)
    .def("__init__", make_constructor(ConstantVolume::create))
    .def("addAttribute", &ConstantVolume::addAttribute)
    ;

  implicitly_convertible<ConstantVolume::Ptr, ConstantVolume::CPtr>();

  // FractalCloud ---

  class_<FractalCloud, bases<Volume>, FractalCloud::Ptr>
    ("FractalCloud")
    .def("__init__",  make_constructor(FractalCloud::create))
    .def("setFractal", &FractalCloud::setFractal)
    .def("setDensity", &FractalCloud::setDensity)
    ;

  implicitly_convertible<FractalCloud::Ptr, FractalCloud::CPtr>();

}

//----------------------------------------------------------------------------//

