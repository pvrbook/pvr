//-*-c++-*--------------------------------------------------------------------//

/*! \file Modeler.h
  Contains the Modeler class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_MODELER_H__
#define __INCLUDED_PVR_MODELER_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

#include <boost/foreach.hpp>

// Project headers

#include "pvr/Camera.h"
#include "pvr/Geometry.h"
#include "pvr/ModelerInput.h"
#include "pvr/Types.h"
#include "pvr/VoxelBuffer.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {

//----------------------------------------------------------------------------//
// Modeler
//----------------------------------------------------------------------------//

/*! \class Modeler
  \brief The modeler class is the core of PVR's volume modeling pipeline. 

  See chapter XX for an introduction.
 */

//----------------------------------------------------------------------------//

class Modeler
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Modeler);

  typedef Field3D::FrustumFieldMapping::Ptr FrustumMappingPtr;
  typedef Field3D::MatrixFieldMapping::Ptr UniformMappingPtr;

  // Enums ---------------------------------------------------------------------

  //! Enumerates mapping types supported by modeler
  enum Mapping {
    UniformMappingType,
    FrustumMappingType
  };

  //! Enumerates data structures supported by modeler
  enum DataStructure {
    DenseBufferType,
    SparseBufferType
  };

  //! Enumerates supported sparse block sizes
  enum SparseBlockSize {
    SparseBlockSize8,
    SparseBlockSize16,
    SparseBlockSize32
  };

  // Exceptions ----------------------------------------------------------------  

  DECLARE_PVR_RT_EXC(InvalidPrimitiveException, "Invalid primitive:");
  DECLARE_PVR_RT_EXC(MissingPrimitiveException, "Missing primitive:");

  // Constructor, destructor, factory ------------------------------------------

  //! Default constructor. Initializes defaults
  Modeler();
  //! Factory method. Use this for all objects that require lifetime management
  static Ptr create();
  //! Clones the object. This is useful when creating a new Modeler that retains
  //! the same settings as the first, and which writes into the same voxel 
  //! buffer. 
  Ptr clone();

  // Settings ------------------------------------------------------------------

  //! Sets the resolution of the voxel buffer explicitly.
  void setResolution(const size_t x, const size_t y, const size_t z);
  //! Sets the resolution of the longest edge of the buffer. Then sets 
  //! divisions along the other edges to make voxel size uniform in all 
  //! dimensions.
  void setResolution(const size_t res);
  //! Sets the resolution of the voxel buffer by defining the voxel size.
  void setVoxelSize(const Vector &size);
  //! Sets the mapping type to use
  void setMapping(const Mapping mapping);
  //! Sets the data structure to use
  void setDataStructure(const DataStructure dataStructure);
  //! Sets the block size in the case of a sparse voxel buffer
  void setSparseBlockSize(const SparseBlockSize size);
  //! Sets the camera to be used during rendering. Required for frustum mappings
  void setCamera(Render::PerspectiveCamera::CPtr camera);

  // Main methods --------------------------------------------------------------

  //! Adds the given ModelerInput to the list of inputs that will be processed
  //! by the next updateBounds() and execute() commands.
  void addInput(ModelerInput::Ptr modelerInput);
  //! Updates the FieldMapping (i.e. transform) of the voxel buffer that will
  //! be created by the execute() call. This is separate so that the bounds
  //! can be set prior to calling execute, allowing more ModelerInput objects
  //! to be added before the modeling process starts.
  void updateBounds();
  //! Executes all the inputs currently added to the Modeler. Once modeling is
  //! complete the ModelerInput objects are purged from the list of current 
  //! inputs.
  void execute();
  //! Saves the state of the voxel buffer to disk.
  void saveBuffer(const std::string &filename) const;
  //! Returns the current buffer
  VoxelBuffer::Ptr buffer() const;

private:

  //! Builds a frustum mapping using m_camera and the provided bounds
  void setupFrustumMapping(const BBox &wsBounds) const;
  //! Builds a uniform/matrix mapping using the provided bounds.
  void setupUniformMapping(const BBox &wsBounds) const;

  // Protected data members ----------------------------------------------------

  //! Current mapping type
  Mapping                         m_mapping;
  //! Current data structure
  DataStructure                   m_dataStructure;
  //! Current sparse block size
  SparseBlockSize                 m_sparseBlockSize;
  //! List of current inputs to the Modeler. This will be cleared by the 
  //! execute() call. 
  std::vector<ModelerInput::Ptr>  m_inputs;
  //! Pointer to the resulting voxel buffer. updateBounds() is responsible for
  //! allocating the pointer.
  VoxelBuffer::Ptr                m_buffer;
  //! Camera used during final rendering. Used when creating frustum mapped
  //! buffers.
  Render::PerspectiveCamera::CPtr m_camera;

};

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
