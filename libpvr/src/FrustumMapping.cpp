//----------------------------------------------------------------------------//

/*! \file FrustumMapping.cpp
  Contains implementations of FrustumMapping class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/FrustumMapping.h"

// System includes

// Library includes

#include <boost/foreach.hpp>

// Project includes

#include "pvr/RenderGlobals.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace Field3D;

//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// FrustumMapping
//----------------------------------------------------------------------------//

FrustumMapping::FrustumMapping()
  : FieldMapping()
{ 
  /* Empty */ 
}
  
//----------------------------------------------------------------------------//

FrustumMapping::FrustumMapping(const Field3D::Box3i &extents)
  : FieldMapping(extents)
{ 
  /* Empty */ 
}

//----------------------------------------------------------------------------//

void FrustumMapping::setCamera(Render::PerspectiveCamera::CPtr camera)
{ 
  m_camera = Render::PerspectiveCamera::staticClone(camera); 
  // Voxel size will need to be recalculated
  calculateVoxelSize();
}

//----------------------------------------------------------------------------//

void FrustumMapping::calculateClipPlanes(const BBox &wsBounds)
{
  assert(m_camera && "No camera set");
  // Check each corner vertex
  double near = std::numeric_limits<double>::max(), far = 0.0f;
  std::vector<Vector> wsCornerPoints = Math::cornerPoints(wsBounds);
  BOOST_FOREACH (const Vector &wsP, wsCornerPoints) {
    Vector csP = m_camera->worldToCamera(wsP, PTime(0.0));
    near = std::min(near, -csP.z);
    far = std::max(far, -csP.z);
  }
  // Clip at zero
  near = std::max(near, 0.0);
  far = std::max(far, 0.0);
  // Update variables
  m_camera->setClipPlanes(near, far);
  // Voxel size will need to be recalculated
  calculateVoxelSize();
}

//----------------------------------------------------------------------------//

void FrustumMapping::calculateVoxelSize()
{
  if (!m_camera) {
    return;
  }
  m_voxelSize.resize(static_cast<int>(m_origin.z + m_res.z));
  int i = m_origin.x + m_res.x / 2;
  int j = m_origin.y + m_res.y / 2;
  // Do all z slices except last
  int zMax = static_cast<int>(m_origin.z + m_res.z - 1);
  for (int k = m_origin.z; k < zMax; ++k) {
    Vector wsP, wsPx, wsPy, wsPz;
    Vector vsP = discToCont(V3i(i, j, k));
    Vector vsPx = discToCont(V3i(i + 1, j, k));
    Vector vsPy = discToCont(V3i(i, j + 1, k));
    Vector vsPz = discToCont(V3i(i, j, k + 1));
    voxelToWorld(vsP, wsP);
    voxelToWorld(vsPx, wsPx);
    voxelToWorld(vsPy, wsPy);
    voxelToWorld(vsPz, wsPz);
    m_voxelSize[k] = Vector((wsPx - wsP).length(), 
                            (wsPy - wsP).length(),
                            (wsPz - wsP).length());
  }
  // Duplicate last value since there are not further slices to differentiate
  // against
  m_voxelSize[m_origin.z + m_res.z - 1] = m_voxelSize[m_origin.z + m_res.z - 2];
}

//----------------------------------------------------------------------------//

void FrustumMapping::worldToVoxel(const Field3D::V3d &wsP, 
                                  Field3D::V3d &vsP) const 
{ 
  return worldToVoxel(wsP, vsP, 0.0);
}

//----------------------------------------------------------------------------//

void FrustumMapping::worldToVoxel(const Field3D::V3d &wsP, Field3D::V3d &vsP, 
                                  float time) const 
{ 
  Field3D::V3d ssP = m_camera->worldToScreen(wsP, PTime(time));
  vsP = screenToVoxel(ssP);
}

//----------------------------------------------------------------------------//

void FrustumMapping::worldToVoxel(std::vector<Field3D::V3d>::const_iterator wsP,
                                  std::vector<Field3D::V3d>::const_iterator end,
                                  std::vector<Field3D::V3d>::iterator vsP) const 
{ 
  for (; wsP != end; ++wsP, ++vsP) {
    worldToVoxel(*wsP, *vsP);
  }
}

//----------------------------------------------------------------------------//

void FrustumMapping::voxelToWorld(const Field3D::V3d &vsP, 
                                  Field3D::V3d &wsP) const 
{ 
  Field3D::V3d ssP = voxelToScreen(vsP);
  wsP = m_camera->screenToWorld(ssP, PTime(0.0));
}

//----------------------------------------------------------------------------//

void FrustumMapping::voxelToWorld(std::vector<Field3D::V3d>::const_iterator vsP, 
                                  std::vector<Field3D::V3d>::const_iterator end, 
                                  std::vector<Field3D::V3d>::iterator wsP) const 
{ 
  for (; vsP != end; ++vsP, ++wsP) {
    voxelToWorld(*vsP, *wsP);
  }
}

//----------------------------------------------------------------------------//

void FrustumMapping::worldToLocal(const Field3D::V3d &wsP, 
                                  Field3D::V3d &lsP) const 
{ 
  worldToLocal(wsP, lsP, 0.0);
}

//----------------------------------------------------------------------------//

void FrustumMapping::worldToLocal(const Field3D::V3d &wsP, Field3D::V3d &lsP, 
                                  float time) const 
{ 
  Field3D::V3d ssP = m_camera->worldToScreen(wsP, PTime(time));
  lsP = screenToLocal(ssP);
}

//----------------------------------------------------------------------------//

void FrustumMapping::worldToLocal(std::vector<Field3D::V3d>::const_iterator wsP,
                                  std::vector<Field3D::V3d>::const_iterator end,
                                  std::vector<Field3D::V3d>::iterator lsP) const 
{ 
  for (; wsP != end; ++wsP, ++lsP) {
    worldToLocal(*wsP, *lsP);
  }
}

//----------------------------------------------------------------------------//

void FrustumMapping::localToWorld(const Field3D::V3d &lsP, 
                                  Field3D::V3d &wsP) const 
{ 
  Field3D::V3d ssP = localToScreen(lsP);
  wsP = m_camera->screenToWorld(ssP, PTime(0.0));
}

//----------------------------------------------------------------------------//

std::string FrustumMapping::className() const
{ 
  return std::string("pvr::FrustumMapping"); 
}

//----------------------------------------------------------------------------//

bool FrustumMapping::isIdentical(FieldMapping::Ptr other, 
                                 double tolerance) const
{
  //! \todo Fix!
  return false;
}

//----------------------------------------------------------------------------//

Field3D::V3d FrustumMapping::wsVoxelSize(int, int, int k) const
{ 
  k = std::max(0, std::min(k, static_cast<int>(m_origin.z + m_res.z - 1)));
  return m_voxelSize[k];
}
  
//----------------------------------------------------------------------------//

FieldMapping::Ptr FrustumMapping::clone() const
{
  FrustumMapping::Ptr cloned(new FrustumMapping);
  cloned->setCamera(m_camera);
  return cloned;
}
  
//----------------------------------------------------------------------------//

void FrustumMapping::extentsChanged()
{
  calculateVoxelSize();
}

//----------------------------------------------------------------------------//

Field3D::V3d FrustumMapping::screenToLocal(const Field3D::V3d &ssP) const
{
  Field3D::V3d lsP;
  lsP.x = Math::fit(ssP.x, -1.0, 1.0, 0.0, 1.0);
  lsP.y = Math::fit(ssP.y, -1.0, 1.0, 0.0, 1.0);
  lsP.z = ssP.z;
  return lsP;
}

//----------------------------------------------------------------------------//

Field3D::V3d FrustumMapping::localToScreen(const Field3D::V3d &lsP) const
{
  Field3D::V3d ssP;
  ssP.x = Math::fit(lsP.x, 0.0, 1.0, -1.0, 1.0);
  ssP.y = Math::fit(lsP.y, 0.0, 1.0, -1.0, 1.0);
  ssP.z = lsP.z;
  return ssP;
}

//----------------------------------------------------------------------------//

Field3D::V3d FrustumMapping::screenToVoxel(const Field3D::V3d &ssP) const
{
  Field3D::V3d vsP;
  Field3D::V3d lsP = screenToLocal(ssP);
  localToVoxel(lsP, vsP);
  return vsP;
}

//----------------------------------------------------------------------------//

Field3D::V3d FrustumMapping::voxelToScreen(const Field3D::V3d &vsP) const
{
  Field3D::V3d lsP;
  voxelToLocal(vsP, lsP);
  return localToScreen(lsP);
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//
