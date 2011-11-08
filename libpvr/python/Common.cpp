//----------------------------------------------------------------------------//

/*! \file Common.cpp
  Contains implementations of functions from Common.h
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "Common.h"

// System includes

// Library includes

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

Util::ParamMap dictToParamMap(boost::python::dict d)
{
  using namespace std;
  using namespace pvr;

  pvr::Util::ParamMap params;
  params.intMap = pyDictValues<string, int>(d);
  params.floatMap = pyDictValues<string, float>(d);
  params.vectorMap = pyDictValues<string, Imath::V3f>(d);
  params.stringMap = pyDictValues<string, string>(d);

  return params;
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//
