//----------------------------------------------------------------------------//

/*! \file PyPvr.cpp
  Contains the interface definition for the 'pvr' python module
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
void exportClassFactory();
void exportCurve();
void exportDeepImage();
void exportGeometry();
void exportGlobals();
void exportField3D();
void exportImage();
void exportLights();
void exportLog();
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
  exportClassFactory();
  exportCurve();
  exportDeepImage();
  exportField3D();
  exportGeometry();
  exportGlobals();
  exportImage();
  exportLights();
  exportLog();
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
