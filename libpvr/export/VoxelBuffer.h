//-*-c++-*--------------------------------------------------------------------//

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
