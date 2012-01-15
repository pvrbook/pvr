//-*-c++-*--------------------------------------------------------------------//

/*! \file ParamBase.h
  Contains the ParamBase class.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PARAMBASE_H__
#define __INCLUDED_PVR_PARAMBASE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

// Project headers

#include "pvr/ParamMap.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//

namespace Util {
  
//! Base class that provides a convenient entry point for control
//! via ParamMap input. Also provides the typeName() necessary for
//! ClassFactory registration. 
class ParamBase
{
public:
  PVR_TYPEDEF_SMART_PTRS(ParamBase);
  //! Virtual destructor
  virtual ~ParamBase()
  { }
  //! Returns the class' type name. Used for plugin registration
  virtual std::string typeName() const = 0;    
  //! Subclasses implement this to decide what they want to do with
  //! default attribute values. ParamBase itself does nothing.
  virtual void setParams(const ParamMap &params)
  { 
    // Default implementation does nothing.
  }
};
  
//----------------------------------------------------------------------------//

} // namespace Util

//----------------------------------------------------------------------------//

namespace Model {

//----------------------------------------------------------------------------//
// Filter
//----------------------------------------------------------------------------//

//! Base class for geometry filters
//! \todo Move to Filter.h
class Filter : public Util::ParamBase
{
public:
  PVR_TYPEDEF_SMART_PTRS(Filter);
};

//----------------------------------------------------------------------------//
// VoxelShader
//----------------------------------------------------------------------------//

//! Base class for voxel shaders used during rasterization
//! \todo Move to VoxelShader.h
class VoxelShader : public Util::ParamBase
{
public:
  PVR_TYPEDEF_SMART_PTRS(VoxelShader);
};

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
