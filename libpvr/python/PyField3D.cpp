//----------------------------------------------------------------------------//

/*! \file PyField3D.cpp
  Contains the interface definition for the Field3D subclasses
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

#include <pvr/VoxelBuffer.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportField3D()
{
  using namespace boost::python;
  using namespace pvr;

  // VoxelBuffer ---

  class_<VoxelBuffer, VoxelBuffer::Ptr, boost::noncopyable>
    ("VoxelBuffer", no_init)
    ;

  // DenseBuffer ---

  class_<DenseBuffer, bases<VoxelBuffer>, DenseBuffer::Ptr>
    ("DenseBuffer")
    ;

  // SparseBuffer ---

  class_<SparseBuffer, bases<VoxelBuffer>, SparseBuffer::Ptr>
    ("SparseBuffer")
    ;

}

//----------------------------------------------------------------------------//


