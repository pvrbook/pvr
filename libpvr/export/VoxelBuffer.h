//-*-c++-*--------------------------------------------------------------------//

/*! \file VoxelBuffer.h
  Contains the VoxelBuffer class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_VOXELBUFFER_H__
#define __INCLUDED_PVR_VOXELBUFFER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Libraray includes

#include <Field3D/DenseField.h>
#include <Field3D/SparseField.h>

#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Field3D typedefs
//----------------------------------------------------------------------------//

typedef Field3D::ResizableField<Imath::V3f> VoxelBuffer;
typedef Field3D::DenseField<Imath::V3f>     DenseBuffer;
typedef Field3D::SparseField<Imath::V3f>    SparseBuffer;

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
