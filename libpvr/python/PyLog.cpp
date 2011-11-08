//----------------------------------------------------------------------------//

/*! \file PyLog.cpp
  Contains the interface definition for the Log class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <boost/shared_ptr.hpp>

#include <boost/python.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Library includes

#include <pvr/Log.h>

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

void printHelper1(const std::string &msg)
{
  pvr::Util::Log::print(msg);
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportLog()
{
  using namespace boost;
  using namespace boost::python;

  using namespace std;
  using namespace pvr;
  using namespace pvr::Util;

  // Log ---

  def("logPrint", &printHelper1);
  def("logWarning", &Log::warning);

}

//----------------------------------------------------------------------------//
