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


