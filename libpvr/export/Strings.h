//-*-c++-*--------------------------------------------------------------------//

/*! \file Strings.h
  Contains commonly used strings.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_STRINGS_H__
#define __INCLUDED_PVR_STRINGS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <string>

// Library includes

#include <boost/lexical_cast.hpp>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Common PVR strings
//----------------------------------------------------------------------------//

//! Struct containing commonly used strings.
struct Str {

static const std::string pvr;
static const std::string pvrLogStamp;

static const std::string density;
static const std::string scattering;
static const std::string absorption;
static const std::string emission;

};

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
