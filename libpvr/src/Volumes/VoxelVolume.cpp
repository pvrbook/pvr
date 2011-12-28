//----------------------------------------------------------------------------//

/*! \file VoxelVolume.cpp
  Contains implementations of VoxelVolume class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Volumes/VoxelVolume.h"

// System includes

// Library includes

#include <Field3D/Field3DFile.h>
#include <Field3D/DenseField.h>
#include <Field3D/SparseField.h>

// Project headers

#include "pvr/Constants.h"
#include "pvr/Log.h"
#include "pvr/Math.h"
#include "pvr/VoxelBuffer.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace pvr;

  //--------------------------------------------------------------------------//

  //! Checks continuous coordinate against discrete coordinate bounds
  bool isInBounds(const Vector &vsP, const Imath::Box3i &dataWindow) 
  {
    for (int dim = 0; dim < 3; ++dim) {
      if (vsP[dim] < static_cast<double>(dataWindow.min[dim]) ||
          vsP[dim] > static_cast<double>(dataWindow.max[dim])) {
        return false;
      }
    }
    return true;
  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

using namespace Field3D;

using namespace pvr::Util; 

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// UniformMappingIntersection
//----------------------------------------------------------------------------//

UniformMappingIntersection::UniformMappingIntersection
(Field3D::MatrixFieldMapping::Ptr mapping)
{
  m_worldToLocal = mapping->localToWorld().inverse();
  m_worldToVoxel = mapping->worldToVoxel();
}
  
//----------------------------------------------------------------------------//

IntervalVec UniformMappingIntersection::intersect(const Ray &wsRay, 
                                                  const PTime time) const
{
  // Transform ray to local space for intersection test
  Ray lsRay;
  m_worldToLocal.multVecMatrix(wsRay.pos, lsRay.pos);
  m_worldToLocal.multDirMatrix(wsRay.dir, lsRay.dir);
  // Use unit bounding box to intersect against
  BBox lsBBox = Bounds::zeroOne();
  // Calculate intersection points
  double t0, t1;
  if (Math::intersect(lsRay, lsBBox, t0, t1)) {
    Vector wsNear = wsRay(t0);
    Vector wsFar = wsRay(t1);
    Vector vsNear, vsFar;
    m_worldToVoxel.multVecMatrix(wsNear, vsNear);
    m_worldToVoxel.multVecMatrix(wsFar, vsFar);
    double numSamples = (vsFar - vsNear).length();
    double stepLength = (t1 - t0) / numSamples;
    return IntervalVec(1, Interval(t0, t1, stepLength));
  } else {
    return IntervalVec();
  }
}
  
//----------------------------------------------------------------------------//
// FrustumMappingIntersection
//----------------------------------------------------------------------------//

FrustumMappingIntersection::FrustumMappingIntersection
(Field3D::FrustumFieldMapping::Ptr mapping)
  : m_mapping(mapping)
{
  typedef std::vector<Vector> PointVec;
  // Get the eight corners of the local space bounding box
  BBox lsBounds = Bounds::zeroOne();
  PointVec lsCorners = Math::cornerPoints(lsBounds);
  // Get the world space positions of the eight corners of the frustum
  PointVec wsCorners(lsCorners.size());
  for (PointVec::iterator lsP = lsCorners.begin(), wsP = wsCorners.begin();
       lsP != lsCorners.end(); ++lsP, ++wsP) {
    mapping->localToWorld(*lsP, *wsP);
  }
  // Construct plane for each face of frustum
  m_planes[0] = Plane(wsCorners[4], wsCorners[0], wsCorners[6]);
  m_planes[1] = Plane(wsCorners[1], wsCorners[5], wsCorners[3]);
  m_planes[2] = Plane(wsCorners[4], wsCorners[5], wsCorners[0]);
  m_planes[3] = Plane(wsCorners[2], wsCorners[3], wsCorners[6]);
  m_planes[4] = Plane(wsCorners[0], wsCorners[1], wsCorners[2]);
  m_planes[5] = Plane(wsCorners[5], wsCorners[4], wsCorners[7]);
}

//----------------------------------------------------------------------------//

IntervalVec FrustumMappingIntersection::intersect(const Ray &wsRay, 
                                                  const PTime time) const
{
  double t0 = -std::numeric_limits<double>::max();
  double t1 = std::numeric_limits<double>::max();
  for (int i = 0; i < 6; ++i) {
    double t;
    const Plane &p = m_planes[i];
    if (p.intersectT(wsRay, t)) {
      if (wsRay.dir.dot(p.normal) > 0.0) {
        // Non-opposing plane
        t1 = std::min(t1, t);
      } else {
        // Opposing plane
        t0 = std::max(t0, t);
      }
    }
  }
  if (t0 < t1) {
    t0 = std::max(t0, 0.0);
    Vector wsNear = wsRay(t0);
    Vector wsFar = wsRay(t1);
    Vector vsNear, vsFar;
    m_mapping->worldToVoxel(wsNear, vsNear);
    m_mapping->worldToVoxel(wsFar, vsFar);
    double numSamples = (vsFar - vsNear).length();
    double stepLength = (t1 - t0) / numSamples;
    return IntervalVec(1, Interval(t0, t1, stepLength));
  } else {
    return IntervalVec();
  }
}

//----------------------------------------------------------------------------//
// VoxelVolume
//----------------------------------------------------------------------------//

VoxelVolume::VoxelVolume()
  : m_interpType(LinearInterp)
{
  // Empty
}

//----------------------------------------------------------------------------//

Volume::AttrNameVec VoxelVolume::attributeNames() const
{
  return m_attrNames;
}

//----------------------------------------------------------------------------//

Color VoxelVolume::sample(const VolumeSampleState &state,
                          const VolumeAttr &attribute) const
{
  // Check (and set up) attribute index ---

  if (attribute.index() == VolumeAttr::IndexNotSet) {
    AttrNameVec::const_iterator i = 
      std::find(m_attrNames.begin(), m_attrNames.end(), attribute.name());
    if (i != m_attrNames.end()) {
      attribute.setIndex(std::distance(i, m_attrNames.begin()));
    } else {
      attribute.setIndexInvalid();
    }
  }
  if (attribute.index() == VolumeAttr::IndexInvalid) {
    return Colors::zero();
  }

  // Transform to voxel space for sampling ---
  
  Vector vsP;
  m_buffer->mapping()->worldToVoxel(state.wsP, vsP);

  if (!isInBounds(vsP, m_buffer->dataWindow())) {
    return Colors::zero();
  }

  // Interpolate voxel value ---

  V3f value(0.0);

  switch (m_interpType) {
  case NoInterp:
    {
      V3i dvsP = contToDisc(vsP);
      value = m_buffer->value(dvsP.x, dvsP.y, dvsP.z);
      break;
    }
  case CubicInterp:
    value = m_cubicInterp.sample(*m_buffer, vsP);
    break;
  case MonotonicCubicInterp:
    value = m_monotonicCubicInterp.sample(*m_buffer, vsP);
    break;
  case GaussianInterp:
    value = m_gaussInterp.sample(*m_buffer, vsP);
    break;
  case MitchellInterp:
    value = m_mitchellInterp.sample(*m_buffer, vsP);
    break;
  case LinearInterp:
  default:
    value = m_linearInterp.sample(*m_buffer, vsP);
    break;
  }

  return m_attrValues[attribute.index()] * value;
}

//----------------------------------------------------------------------------//

IntervalVec VoxelVolume::intersect(const RayState &state) const
{
  assert (m_intersectionHandler && "Missing intersection handler");
  return m_intersectionHandler->intersect(state.wsRay, state.time);
}

//----------------------------------------------------------------------------//

Volume::StringVec VoxelVolume::info() const
{
  StringVec info;
  for (size_t i = 0; i < m_attrNames.size(); ++i) {
    info.push_back(m_attrNames[i] + " : " + str(m_attrValues[i]));
  }
  return info;
}

//----------------------------------------------------------------------------//

void VoxelVolume::load(const std::string &filename) 
{
  Log::print("Loading voxel buffer: " + filename);

  Field<Imath::V3f>::Vec buffers;
  Field3DInputFile in;
  if (!in.open(filename)) {
    Log::warning("Couldn't load " + filename);
    return;
  }

  buffers = in.readVectorLayers<float>();
  if (buffers.size() == 0) {
    Log::warning("No <float> fields could be loaded from " + filename);
    return;
  }

  DenseBuffer::Ptr denseBuffer = field_dynamic_cast<DenseBuffer>(buffers[0]);
  if (denseBuffer) {
    m_buffer = denseBuffer;
  } else {
    Log::warning("No DenseField in: " + filename);
  }
  
  updateIntersectionHandler();
}

//----------------------------------------------------------------------------//

void VoxelVolume::setBuffer(VoxelBuffer::Ptr buffer)
{
  m_buffer = buffer;
  updateIntersectionHandler();
}

//----------------------------------------------------------------------------//

void VoxelVolume::addAttribute(const std::string &attrName, 
                               const Imath::V3f &value)
{
  m_attrNames.push_back(attrName);
  m_attrValues.push_back(value);
}

//----------------------------------------------------------------------------//

void VoxelVolume::setInterpolation(const InterpType interpType)
{
  m_interpType = interpType;
}

//----------------------------------------------------------------------------//

void VoxelVolume::updateIntersectionHandler()
{
  if (!m_buffer) {
    throw MissingBufferException();
  }
  if (!m_buffer->mapping()) {
    throw MissingMappingException();
  }
  MatrixFieldMapping::Ptr matrixMapping = 
    field_dynamic_cast<MatrixFieldMapping>(m_buffer->mapping());
  FrustumFieldMapping::Ptr frustumMapping = 
    field_dynamic_cast<FrustumFieldMapping>(m_buffer->mapping());
  if (matrixMapping) {
    m_intersectionHandler.reset(new UniformMappingIntersection(matrixMapping));
  } else if (frustumMapping) {
    m_intersectionHandler.reset(new FrustumMappingIntersection(frustumMapping));
  } else {
    throw UnsupportedMappingException();
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
