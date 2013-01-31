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

//----------------------------------------------------------------------------//

int sign(float x) { return (x > 0) - (x < 0); }

//----------------------------------------------------------------------------//

void handleNaN(pvr::Vector &v) 
{
#ifdef WIN32
  if (_isnan(v.x)) 
    v.x = std::numeric_limits<double>::max();
  if (_isnan(v.y)) 
    v.y = std::numeric_limits<double>::max();
  if (_isnan(v.z)) 
    v.z = std::numeric_limits<double>::max();
  if (!_finite(v.x)) 
    v.x = std::numeric_limits<double>::max();
  if (!_finite(v.y)) 
    v.y = std::numeric_limits<double>::max();
  if (!_finite(v.z)) 
    v.z = std::numeric_limits<double>::max();
#else
  if (std::isnan(v.x)) 
    v.x = std::numeric_limits<double>::max();
  if (std::isnan(v.y)) 
    v.y = std::numeric_limits<double>::max();
  if (std::isnan(v.z)) 
    v.z = std::numeric_limits<double>::max();
  if (std::isinf(v.x)) 
    v.x = std::numeric_limits<double>::max();
  if (std::isinf(v.y)) 
    v.y = std::numeric_limits<double>::max();
  if (std::isinf(v.z)) 
    v.z = std::numeric_limits<double>::max();
#endif
}

//----------------------------------------------------------------------------//

void worldToBlock(Field3D::MatrixFieldMapping::Ptr mapping, 
                  const int blockSize, const pvr::Ray &wsRay, 
                  pvr::Ray &bsRay)
{
  mapping->worldToVoxel(wsRay.pos, bsRay.pos);
  mapping->worldToVoxelDir(wsRay.dir, bsRay.dir);
  bsRay.pos /= static_cast<double>(blockSize);
  bsRay.dir /= static_cast<double>(blockSize);
}

//----------------------------------------------------------------------------//

void stepToNextBlock(const pvr::Vector &tDelta, const Imath::V3i &sgn, 
                     pvr::Vector &tMax, int &x, int &y, int &z)
{
  if (tMax.x < tMax.y && tMax.x < tMax.z) {
    x += sgn.x;
    tMax.x += tDelta.x;
  } else if (tMax.y < tMax.z) {
    y += sgn.y;
    tMax.y += tDelta.y;
  } else {
    z += sgn.z;
    tMax.z += tDelta.z;
  }
}

//----------------------------------------------------------------------------//

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
  : m_mapping(mapping)
{
  
}
  
//----------------------------------------------------------------------------//

//! \note worldToLocalDir does not yet take a time argument.
IntervalVec UniformMappingIntersection::intersect(const Ray &wsRay, 
                                                  const PTime time) const
{
  // Transform ray to local space for intersection test
  Ray lsRay;
  m_mapping->worldToLocal(wsRay.pos, lsRay.pos, time);
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
  
}

//----------------------------------------------------------------------------//

IntervalVec FrustumMappingIntersection::intersect(const Ray &wsRay, 
                                                  const PTime time) const
{
  typedef std::vector<Vector> PointVec;

  // Get the eight corners of the local space bounding box
  BBox lsBounds = Bounds::zeroOne();
  PointVec lsCorners = Math::cornerPoints(lsBounds);
  // Get the world space positions of the eight corners of the frustum
  PointVec wsCorners(lsCorners.size());
  for (PointVec::iterator lsP = lsCorners.begin(), wsP = wsCorners.begin(),
         end = lsCorners.end(); lsP != end; ++lsP, ++wsP) {
    m_mapping->localToWorld(*lsP, *wsP, time);
  }

  // Construct plane for each face of frustum
  Plane planes[6];
  planes[0] = Plane(wsCorners[4], wsCorners[0], wsCorners[6]);
  planes[1] = Plane(wsCorners[1], wsCorners[5], wsCorners[3]);
  planes[2] = Plane(wsCorners[4], wsCorners[5], wsCorners[0]);
  planes[3] = Plane(wsCorners[2], wsCorners[3], wsCorners[6]);
  planes[4] = Plane(wsCorners[0], wsCorners[1], wsCorners[2]);
  planes[5] = Plane(wsCorners[5], wsCorners[4], wsCorners[7]);

  // Intersect ray against planes
  double t0 = -std::numeric_limits<double>::max();
  double t1 = std::numeric_limits<double>::max();
  for (int i = 0; i < 6; ++i) {
    double t;
    const Plane &p = planes[i];
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
// SparseUniformOptimizer
//----------------------------------------------------------------------------//

SparseUniformOptimizer::SparseUniformOptimizer
(SparseBuffer::Ptr sparse, 
 Field3D::MatrixFieldMapping::Ptr mapping)
  : m_sparse(sparse), m_mapping(mapping)
{ 

}

//----------------------------------------------------------------------------//

IntervalVec
SparseUniformOptimizer::optimize(const RayState &state, 
                                 const IntervalVec &intervals) const
{
  if (intervals.size() != 1) {
    return intervals;
  }

  IntervalVec result;

  const Ray &wsRay = state.wsRay;
  const PTime &time = state.time;

  // Find start voxel
  Vector wsStart = wsRay(intervals[0].t0), vsStart;
  m_sparse->mapping()->worldToVoxel(wsStart, vsStart, time);
  V3i in = Imath::clip(V3i(vsStart), m_sparse->extents());

  // Find start block
  V3i bStart;
  m_sparse->getBlockCoord(in.x, in.y, in.z, bStart.x, bStart.y, bStart.z);

  // Transform ray to block space
  Ray bsRay;
  worldToBlock(m_mapping, m_sparse->blockSize(), wsRay, bsRay);
  
  // Find runs along ray ---

  // Implementation is based on:
  // "A Fast Voxel Traversal Algorithm for Ray Tracing"
  // John Amanatides, Andrew Woo
  // http://www.cse.yorku.ca/~amana/research/grid.pdf

  // Current block
  int x = bStart.x, y = bStart.y, z = bStart.z;
  // Direction to step
  V3i sgn(sign(bsRay.dir.x), sign(bsRay.dir.y), sign(bsRay.dir.z));
  // Whether to look at positive or negative side of block
  Vector cell(x + (sgn.x > 0 ? 1 : 0), 
              y + (sgn.y > 0 ? 1 : 0), 
              z + (sgn.z > 0 ? 1 : 0));
  // Distance we could step in each direction, at most
  Vector tMax((cell - bsRay.pos) / bsRay.dir);
  // Size of one step in each dimension
  Vector tDelta(sgn.x / bsRay.dir.x, sgn.y / bsRay.dir.y, sgn.z / bsRay.dir.z);
  // Ensure there are no inf's or nan's
  handleNaN(tMax);
  handleNaN(tDelta);
  // Check if first block starts a run
  bool run = m_sparse->blockIsAllocated(x, y, z);
  // Keep track of start-of-run and last visited block
  V3i last(bStart), startRun(bStart);

  // Traverse blocks
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
    stepToNextBlock(tDelta, sgn, tMax, x, y, z);
  }

  if (run) {
    result.push_back(intervalForRun(wsRay, time, startRun, last));
  }

  return result;
}

//----------------------------------------------------------------------------//

Interval
SparseUniformOptimizer::intervalForRun(const Ray &wsRay, const PTime time,
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

//! \note worldToLocalDir does not yet take a time argument.
void
SparseUniformOptimizer::intersect(const Ray &wsRay, const PTime time, 
                                  const Imath::V3i block,
                                  double &t0, double &t1) const
{
  // Transform to local space
  Ray lsRay;
  m_mapping->worldToLocal(wsRay.pos, lsRay.pos, time);
  m_mapping->worldToLocalDir(wsRay.dir, lsRay.dir);
  // Intersect in local space
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

SparseFrustumOptimizer::SparseFrustumOptimizer
(SparseBuffer::Ptr sparse, 
 Field3D::FrustumFieldMapping::Ptr mapping)
  : m_sparse(sparse), m_mapping(mapping)
{ 

}

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

  // Find start and end voxel
  Vector vsStart, vsEnd;
  m_sparse->mapping()->worldToVoxel(wsRay(intervals[0].t0), vsStart);
  m_sparse->mapping()->worldToVoxel(wsRay(intervals[0].t1), vsEnd);
  V3i in = Imath::clip(V3i(vsStart), m_sparse->extents());
  V3i out = Imath::clip(V3i(vsEnd), m_sparse->extents());

  // Find start and end block
  V3i bStart, bEnd;
  m_sparse->getBlockCoord(in.x, in.y, in.z, bStart.x, bStart.y, bStart.z);
  m_sparse->getBlockCoord(out.x, out.y, out.z, bEnd.x, bEnd.y, bEnd.z);

  // Ensure ray travels down z axis
  if (bStart.x != bEnd.x || bStart.y != bEnd.y) {
    return intervals;
  }

  // Current block
  int x = bStart.x, y = bStart.y, z = bStart.z;
  // Check if first block starts a run
  bool run = m_sparse->blockIsAllocated(x, y, z);
  // Keep track of start-of-run and last visited block
  int startRun = z, last = z;

  // Traverse row of blocks
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
    setIndexForName(attribute, m_attrNames);
  }
  if (attribute.index() == VolumeAttr::IndexInvalid) {
    return VolumeSample(Colors::zero(), m_phaseFunction);
  }

  // Transform to voxel space for sampling ---
  
  Vector vsP;
  m_buffer->mapping()->worldToVoxel(state.wsP, vsP, state.rayState.time);

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
  if (m_eso && m_useEmptySpaceOptimization) {
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
      m_eso = SparseUniformOptimizer::create(sparse, mMapping);
    } else if (fMapping) {
      m_eso = SparseFrustumOptimizer::create(sparse, fMapping);
    } else {
      Log::warning("VoxelVolume::setBuffer(): Unrecognized mapping type.");
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
