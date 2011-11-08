//-*-c++-*--------------------------------------------------------------------//

/*! \file FrustumMapping.h
  Contains the FrustumMapping class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_FRUSTUMMAPPING_H__
#define __INCLUDED_PVR_FRUSTUMMAPPING_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

// Library includes

#include <Field3D/FieldMapping.h>

// Project includes

#include "pvr/Camera.h"
#include "pvr/Math.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// FrustumMapping
//----------------------------------------------------------------------------//

/*! \class FrustumMapping
  \brief Implements a frustum mapping based on pvr::Render::Camera

  \todo Write definition of coordinate spaces
*/

//----------------------------------------------------------------------------//

class FrustumMapping : public Field3D::FieldMapping
{
public:

  // Typedefs ------------------------------------------------------------------

  //! Convenience typedef
  typedef boost::intrusive_ptr<FrustumMapping> Ptr;
  
  // Ctors, dtor ---------------------------------------------------------------

  //! \name Constructors & destructor
  //! \{

  DECLARE_CREATE_FUNC(FrustumMapping);

  FrustumMapping();
  FrustumMapping(const Field3D::Box3i &extents);

  //! \}

  // Main methods --------------------------------------------------------------

  //! Sets the camera to use for the frustum transformation
  void setCamera(Render::PerspectiveCamera::CPtr camera);
  //! Calculates near/far planes for the given bounds.
  //! \note The near plane will never be closer than 0.0.
  //! \todo zero near plane is no good.. clip to epsilon!
  void calculateClipPlanes(const BBox &wsBounds);

  // From FieldMapping ---------------------------------------------------------
 
  virtual void worldToVoxel(const Field3D::V3d &wsP, Field3D::V3d &vsP) const;
  virtual void worldToVoxel(const Field3D::V3d &wsP, Field3D::V3d &vsP, 
                            float time) const;
  virtual void worldToVoxel(std::vector<Field3D::V3d>::const_iterator wsP,
                            std::vector<Field3D::V3d>::const_iterator end,
                            std::vector<Field3D::V3d>::iterator vsP) const;
  virtual void voxelToWorld(const Field3D::V3d &vsP, Field3D::V3d &wsP) const;
  virtual void voxelToWorld(std::vector<Field3D::V3d>::const_iterator vsP, 
                            std::vector<Field3D::V3d>::const_iterator end, 
                            std::vector<Field3D::V3d>::iterator wsP) const;

  virtual void worldToLocal(const Field3D::V3d &wsP, Field3D::V3d &lsP) const;
  virtual void worldToLocal(const Field3D::V3d &wsP, Field3D::V3d &lsP, 
                            float time) const;
  virtual void worldToLocal(std::vector<Field3D::V3d>::const_iterator wsP,
                            std::vector<Field3D::V3d>::const_iterator end,
                            std::vector<Field3D::V3d>::iterator lsP) const;
  virtual void localToWorld(const Field3D::V3d &lsP, Field3D::V3d &wsP) const;
  virtual std::string className() const;;
  virtual bool isIdentical(FieldMapping::Ptr other, 
                           double tolerance = 0.0) const;
  virtual Field3D::V3d wsVoxelSize(int, int, int k) const;
  virtual FieldMapping::Ptr clone() const;
  virtual void extentsChanged();

  // Transform methods ---------------------------------------------------------

  Field3D::V3d screenToLocal(const Field3D::V3d &ssP) const;
  Field3D::V3d localToScreen(const Field3D::V3d &lsP) const;
  Field3D::V3d screenToVoxel(const Field3D::V3d &ssP) const;
  Field3D::V3d voxelToScreen(const Field3D::V3d &vsP) const;

protected:

  // Utility methods -----------------------------------------------------------

  //! Calculates the voxel size for each depth slice
  void calculateVoxelSize();

  // Data members --------------------------------------------------------------

  //! Pointer to camera. We can assume that we are the sole owner of the pointer
  Render::PerspectiveCamera::Ptr m_camera;
  //! Store the approximate (measured for a voxel at center of x/y plane) 
  //! voxel size of each z slice
  std::vector<Vector> m_voxelSize;

};

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
