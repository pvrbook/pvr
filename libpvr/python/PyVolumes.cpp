//----------------------------------------------------------------------------//

/*! \file PyVolumes.cpp
  Contains the interface definition for the Volumes subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

#include <pvr/Volumes/FractalCloud.h>
#include <pvr/Volumes/CompositeVolume.h>
#include <pvr/Volumes/VoxelVolume.h>

#include "Common.h"

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
    .def("typeName", &Volume::typeName)
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
    ;

  implicitly_convertible<VoxelVolume::Ptr, VoxelVolume::CPtr>();

  // CompositeVolume ---

  class_<CompositeVolume, bases<Volume>, CompositeVolume::Ptr>
    ("CompositeVolume")
    .def("__init__", make_constructor(CompositeVolume::create))
    .def("add",      &CompositeVolume::add)
    ;

  implicitly_convertible<CompositeVolume::Ptr, CompositeVolume::CPtr>();

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

