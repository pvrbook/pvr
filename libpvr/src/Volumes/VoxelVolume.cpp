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

  int sign(int x) { return (x > 0) - (x < 0); }

  //--------------------------------------------------------------------------//

  void handleNaN(pvr::Vector &v) 
  {
    if (std::isnan(v.x)) 
      v.x = std::numeric_limits<double>::max();
    if (std::isnan(v.y)) 
      v.y = std::numeric_limits<double>::max();
    if (std::isnan(v.z)) 
      v.z = std::numeric_limits<double>::max();
  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

using namespace Field3D;
using namespace Imath;

using namespace pvr::Util; 

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// UniformMappingIntersection
//----------------------------------------------------------------------------//

UniformMappingIntersection::UniformMappingIntersection
(Field3D::MatrixFieldMapping::Ptr mapping)
  : m_mapping(mapping)
{
  
}
  
//----------------------------------------------------------------------------//

IntervalVec UniformMappingIntersection::intersect(const Ray &wsRay, 
                                                  const PTime time) const
{
  // Transform ray to local space for intersection test
  Ray lsRay;
  m_mapping->worldToLocal(wsRay.pos, lsRay.pos);
  m_mapping->worldToLocalDir(wsRay.dir, lsRay.dir);
  // Use unit bounding box to intersect against
  BBox lsBBox = Bounds::zeroOne();
  // Calculate intersection points
  double t0, t1;
  if (Math::intersect(lsRay, lsBBox, t0, t1)) {
    return IntervalVec(1, makeInterval(wsRay, t0, t1, m_mapping));
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
  for (PointVec::iterator lsP = lsCorners.begin(), wsP = wsCorners.begin(),
         end = lsCorners.end(); lsP != end; ++lsP, ++wsP) {
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
    return IntervalVec(1, makeInterval(wsRay, t0, t1, m_mapping));
  } else {
    return IntervalVec();
  }
}

//----------------------------------------------------------------------------//
// SparseOptimizer
//----------------------------------------------------------------------------//

IntervalVec SparseOptimizer::optimize(const RayState &state, 
                                      const IntervalVec &intervals) const
{
  if (intervals.size() != 1) {
    return intervals;
  }

  IntervalVec result;
  const Ray &wsRay = state.wsRay;
  const PTime &time = state.time;

  // Find start and end voxel
  Vector wsStart = wsRay(intervals[0].t0);
  Vector vsStart;
  m_sparse->mapping()->worldToVoxel(wsStart, vsStart);
  V3i in(vsStart);
  Box3i extents = m_sparse->extents();
  in.x = clamp(in.x, extents.min.x, extents.max.x);
  in.y = clamp(in.y, extents.min.y, extents.max.y);
  in.z = clamp(in.z, extents.min.z, extents.max.z);

  // Find start block
  V3i bStart;
  m_sparse->getBlockCoord(in.x, in.y, in.z, bStart.x, bStart.y, bStart.z);

  // Find runs along ray ---

  V3i last(bStart), startRun(bStart);

  Ray bsRay;
  m_mapping->worldToVoxel(wsRay.pos, bsRay.pos);
  m_mapping->worldToVoxelDir(wsRay.dir, bsRay.dir);
  bsRay.pos /= m_sparse->blockSize();
  bsRay.dir /= m_sparse->blockSize();
  
  // Implementation is based on:
  // "A Fast Voxel Traversal Algorithm for Ray Tracing"
  // John Amanatides, Andrew Woo
  // http://www.cse.yorku.ca/~amana/research/grid.pdf

  int x = bStart.x, y = bStart.y, z = bStart.z;
  int sX = sign(bsRay.dir.x), sY = sign(bsRay.dir.y), sZ = sign(bsRay.dir.z);
  V3i cell(x + (sX > 0 ? 1 : 0), y + (sY > 0 ? 1 : 0), z + (sZ > 0 ? 1 : 0));

  Vector tMax((cell.x - bsRay.pos.x) / bsRay.dir.x,
              (cell.y - bsRay.pos.y) / bsRay.dir.y,
              (cell.z - bsRay.pos.z) / bsRay.dir.z);
  Vector tDelta(sX / bsRay.dir.x, sY / bsRay.dir.y, sZ / bsRay.dir.z);
  handleNaN(tMax);
  handleNaN(tDelta);

  bool run = m_sparse->blockIsAllocated(x, y, z);

  while (m_sparse->blockIndexIsValid(x, y, z)) {

    if (m_sparse->blockIsAllocated(x, y, z)) {
      if (!run) {
        startRun = V3i(x, y, z);
        run = true;
      }
    } else {
      if (run) {
        result.push_back(intervalForRun(wsRay, time, startRun, last));
        run = false;
      }
    }

    last = V3i(x, y, z);
 
    if (tMax.x < tMax.y && tMax.x < tMax.z) {
      x += sX;
      tMax.x += tDelta.x;
    } else if (tMax.y < tMax.z) {
      y += sY;
      tMax.y += tDelta.y;
    } else {
      z += sZ;
      tMax.z += tDelta.z;
    }

  }

  if (run) {
    result.push_back(intervalForRun(wsRay, time, startRun, last));
  }

  return result;
}

//----------------------------------------------------------------------------//

Interval SparseOptimizer::intervalForRun(const Ray &wsRay, const PTime time,
                                         const Imath::V3i &start, 
                                         const Imath::V3i &end) const
{
  double t0, t1, intT0, intT1;
  intersect(wsRay, time, start, t0, t1);
  intT0 = std::min(t0, t1);
  intersect(wsRay, time, end, t0, t1);
  intT1 = std::max(t0, t1);
  return makeInterval(wsRay, intT0, intT1, m_sparse->mapping());
}

//----------------------------------------------------------------------------//

void SparseOptimizer::intersect(const Ray &wsRay, const PTime time, 
                                const Imath::V3i block,
                                double &t0, double &t1) const
{
  Ray lsRay;
  m_mapping->worldToLocal(wsRay.pos, lsRay.pos);
  m_mapping->worldToLocalDir(wsRay.dir, lsRay.dir);
  Imath::Box3i box;
  int blockSz = m_sparse->blockSize();
  box.min = V3i(block.x * blockSz, block.y * blockSz, block.z * blockSz);
  box.max = box.min + V3i(blockSz - 1, blockSz - 1, blockSz - 1);
  BBox vsBox(box.min, box.max + V3i(1)), lsBox;
  m_mapping->voxelToLocal(vsBox.min, lsBox.min);
  m_mapping->voxelToLocal(vsBox.max, lsBox.max);
  Math::intersect(lsRay, lsBox, t0, t1);
}

//----------------------------------------------------------------------------//
// SparseFrustumOptimizer
//----------------------------------------------------------------------------//

IntervalVec 
SparseFrustumOptimizer::optimize(const RayState &state, 
                                 const IntervalVec &intervals) const
{
  if (intervals.size() != 1) {
    return intervals;
  }

  if (state.rayType != RayState::FullRaymarch ||
      state.rayDepth != 0) {
    return intervals;
  }

  IntervalVec result;
  const Ray &wsRay = state.wsRay;
  const PTime &time = state.time;

  // Find start voxel
  Vector wsStart = wsRay(intervals[0].t0);
  Vector wsEnd = wsRay(intervals[0].t1);
  Vector vsStart, vsEnd;
  m_sparse->mapping()->worldToVoxel(wsStart, vsStart);
  m_sparse->mapping()->worldToVoxel(wsEnd, vsEnd);
  V3i in(vsStart), out(vsEnd);
  Box3i extents = m_sparse->extents();
  in.x = clamp(in.x, extents.min.x, extents.max.x);
  in.y = clamp(in.y, extents.min.y, extents.max.y);
  in.z = clamp(in.z, extents.min.z, extents.max.z);
  out.x = clamp(out.x, extents.min.x, extents.max.x);
  out.y = clamp(out.y, extents.min.y, extents.max.y);
  out.z = clamp(out.z, extents.min.z, extents.max.z);

  // Find start and end block
  V3i bStart, bEnd;
  m_sparse->getBlockCoord(in.x, in.y, in.z, bStart.x, bStart.y, bStart.z);
  m_sparse->getBlockCoord(out.x, out.y, out.z, bEnd.x, bEnd.y, bEnd.z);
  int x = bStart.x, y = bStart.y, z = bStart.z;

  bool run = m_sparse->blockIsAllocated(x, y, z);
  int startRun = z, last;

  for (; z <= bEnd.z; z++) {
    if (m_sparse->blockIsAllocated(x, y, z)) {
      if (!run) {
        startRun = z;
        run = true;
      }
    } else {
      if (run) {
        result.push_back(intervalForRun(wsRay, time, vsStart, startRun, last));
        run = false;
      }
    }
    last = z;
  }

  if (run) {
    result.push_back(intervalForRun(wsRay, time, vsStart, startRun, last));
  }

  return result;
}

//----------------------------------------------------------------------------//

Interval 
SparseFrustumOptimizer::intervalForRun(const Ray &wsRay, const PTime time,
                                       const Vector &vsFirst, 
                                       const int start, const int end) const
{
  double vsStartZ = start * m_sparse->blockSize();
  double vsEndZ = (end + 1) * m_sparse->blockSize();
  Vector vsStart(vsFirst.x, vsFirst.y, vsStartZ);
  Vector vsEnd(vsFirst.x, vsFirst.y, vsEndZ);
  Vector wsStart, wsEnd;
  m_mapping->voxelToWorld(vsStart, wsStart);
  m_mapping->voxelToWorld(vsEnd, wsEnd);
  double t0 = (wsStart - wsRay.pos).length();
  double t1 = (wsEnd - wsRay.pos).length();
  return makeInterval(wsRay, t0, t1, m_mapping);
}

//----------------------------------------------------------------------------//
// VoxelVolume
//----------------------------------------------------------------------------//

VoxelVolume::VoxelVolume()
  : m_interpType(LinearInterp), m_useEmptySpaceOptimization(true)
{
  // Empty
}

//----------------------------------------------------------------------------//

Volume::AttrNameVec VoxelVolume::attributeNames() const
{
  return m_attrNames;
}

//----------------------------------------------------------------------------//

VolumeSample VoxelVolume::sample(const VolumeSampleState &state,
                                 const VolumeAttr &attribute) const
{
  // Check (and set up) attribute index ---

  if (attribute.index() == VolumeAttr::IndexNotSet) {
    // Then, set up the input attribute
    AttrNameVec::const_iterator i = 
      std::find(m_attrNames.begin(), m_attrNames.end(), attribute.name());
    if (i != m_attrNames.end()) {
      attribute.setIndex(std::distance(i, m_attrNames.begin()));
    } else {
      attribute.setIndexInvalid();
    }
  }
  if (attribute.index() == VolumeAttr::IndexInvalid) {
    return VolumeSample(Colors::zero(), m_phaseFunction);
  }

  // Transform to voxel space for sampling ---
  
  Vector vsP;
  m_buffer->mapping()->worldToVoxel(state.wsP, vsP);

  if (!Math::isInBounds(vsP, m_buffer->dataWindow())) {
    return VolumeSample(Colors::zero(), m_phaseFunction);
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

  return VolumeSample(m_attrValues[attribute.index()] * value, 
                      m_phaseFunction);
}

//----------------------------------------------------------------------------//

BBox VoxelVolume::wsBounds() const
{
  return m_wsBounds;
}

//----------------------------------------------------------------------------//

IntervalVec VoxelVolume::intersect(const RayState &state) const
{
  assert (m_intersectionHandler && "Missing intersection handler");
  if (m_eso && m_useEmptySpaceOptimization) {
    IntervalVec i = m_intersectionHandler->intersect(state.wsRay, state.time);
    return m_eso->optimize(state, i);                   
  } else {
    return m_intersectionHandler->intersect(state.wsRay, state.time);
  }
}

//----------------------------------------------------------------------------//

Volume::StringVec VoxelVolume::info() const
{
  StringVec info;
  for (size_t i = 0, size = m_attrNames.size(); i < size; ++i) {
    info.push_back(m_attrNames[i] + " : " + str(m_attrValues[i]));
  }
  if (m_useEmptySpaceOptimization) {
    info.push_back("Empty space optimization: " + m_eso->typeName());
  } else {
    info.push_back("Empty space optimization disabled");
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
  SparseBuffer::Ptr sparse = field_dynamic_cast<SparseBuffer>(buffer);
  if (sparse) {
    MatrixFieldMapping::Ptr mMapping = 
      field_dynamic_cast<MatrixFieldMapping>(sparse->mapping());
    FrustumFieldMapping::Ptr fMapping = 
      field_dynamic_cast<FrustumFieldMapping>(sparse->mapping());
    if (mMapping) {
      m_eso = SparseOptimizer::create(sparse, mMapping);
    } else if (fMapping) {
      m_eso = SparseFrustumOptimizer::create(sparse, fMapping);
    }
  }
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

void VoxelVolume::setUseEmptySpaceOptimization(const bool enabled)
{
  m_useEmptySpaceOptimization = enabled;
}

//----------------------------------------------------------------------------//

void VoxelVolume::updateIntersectionHandler()
{
  // Error checks
  if (!m_buffer) {
    throw MissingBufferException();
  }
  if (!m_buffer->mapping()) {
    throw MissingMappingException();
  }
  // Update intersection handler
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
  // Update bounds
  m_wsBounds.makeEmpty();
  BBox lsBounds = Bounds::zeroOne();
  std::vector<Vector> lsCorners = Math::cornerPoints(lsBounds);
  Vector wsP;
  for (std::vector<Vector>::iterator lsP = lsCorners.begin(), end = lsCorners.end(); 
       lsP != end; ++lsP) {
    m_buffer->mapping()->localToWorld(*lsP, wsP);
    m_wsBounds.extendBy(wsP);
  }
}

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

Interval makeInterval(const Ray &wsRay, const double t0, const double t1,
                      Field3D::FieldMapping::Ptr mapping)
{
  Vector wsNear = wsRay(t0);
  Vector wsFar = wsRay(t1);
  Vector vsNear, vsFar;
  mapping->worldToVoxel(wsNear, vsNear);
  mapping->worldToVoxel(wsFar, vsFar);
  double numSamples = (vsFar - vsNear).length();
  double stepLength = (t1 - t0) / numSamples;
  return Interval(t0, t1, stepLength);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
