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

/*! \file PyPvr.cpp
  Contains the interface definition for the 'pvr' python module
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

#include <Field3D/InitIO.h>

#include <pvr/Globals.h>
#include <pvr/Interrupt.h>
#include <pvr/Log.h>
#include <pvr/Strings.h>
#include <pvr/Time.h>
#include <pvr/Types.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr;

//----------------------------------------------------------------------------//
// Forward declarations
//----------------------------------------------------------------------------//

void exportAttrTable();
void exportCameraFunctions();
void exportCurve();
void exportDeepImage();
void exportGeometry();
void exportGlobals();
void exportField3D();
void exportImage();
void exportLights();
void exportLog();
void exportMath();
void exportModeler();
void exportModelerInput();
void exportNoiseFunctions();
void exportNoiseClasses();
void exportOccluders();
void exportParticles();
void exportPerspectiveCamera();
void exportPhaseFunction();
void exportPolygons();
void exportPrimitive();
void exportRaymarchers();
void exportRaymarchSamplers();
void exportRenderer();
void exportTypes();
void exportVolumes();

//----------------------------------------------------------------------------//
// PythonInterrupt
//----------------------------------------------------------------------------//

class PythonInterrupt : public pvr::Sys::Interrupt
{
public:
  static Ptr create()
  { return Ptr(new PythonInterrupt); }
  virtual bool abort() const
  {
    if (PyErr_CheckSignals() != 0) {
      pvr::Util::Log::print("PVR got interrupt signal. Aborting.");
      return true;
    } else {
      return false;
    }
  }
};

//----------------------------------------------------------------------------//
// Initialization helper
//----------------------------------------------------------------------------//

void initPyPvr()
{
  // PVR globals
  Sys::Globals::init();
  // Field3D initialization
  Field3D::initIO();
  // Register interrupt handler
  pvr::Sys::Interrupt::setGlobalInterrupt(PythonInterrupt::create());
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

BOOST_PYTHON_MODULE(_pvr)
{
  using namespace boost::python;

  initPyPvr();

  exportAttrTable();
  exportCameraFunctions();
  exportCurve();
  exportDeepImage();
  exportField3D();
  exportGeometry();
  exportGlobals();
  exportImage();
  exportLights();
  exportLog();
  exportMath();
  exportModeler();
  exportModelerInput();
  exportNoiseClasses();
  exportNoiseFunctions();
  exportOccluders();
  exportParticles();
  exportPerspectiveCamera();
  exportPhaseFunction();
  exportPolygons();
  exportPrimitive();
  exportRaymarchSamplers();
  exportRaymarchers();
  exportRenderer();
  exportTypes();
  exportVolumes();
}

//----------------------------------------------------------------------------//
