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

/*! \file VoxelVolume.h
  Contains the VoxelVolume class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_VOXELVOLUME_H__
#define __INCLUDED_PVR_VOXELVOLUME_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Library headers

#include <boost/foreach.hpp>

#include <Field3D/FieldMapping.h>

// Project headers

#include "pvr/CubicInterp.h"
#include "pvr/GaussianInterp.h"
#include "pvr/MitchellInterp.h"
#include "pvr/Volumes/Volume.h"
#include "pvr/VoxelBuffer.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// BufferIntersection
//----------------------------------------------------------------------------//

class BufferIntersection
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(BufferIntersection);

  // To be implemented by subclasses -------------------------------------------

  //! Returns a list of intersections for the given ray and time.
  virtual IntervalVec intersect(const Ray &wsRay, const PTime time) const = 0;
};

//----------------------------------------------------------------------------//
// UniformMappingIntersection
//----------------------------------------------------------------------------//

class UniformMappingIntersection : public BufferIntersection
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(UniformMappingIntersection);

  // Ctor ----------------------------------------------------------------------

  UniformMappingIntersection(Field3D::MatrixFieldMapping::Ptr mapping);

  // From BufferIntersection ---------------------------------------------------

  virtual IntervalVec intersect(const Ray &wsRay, const PTime time) const;

private:

  // Data members --------------------------------------------------------------

  // Matrix m_worldToLocal, m_worldToVoxel;
  Field3D::MatrixFieldMapping::Ptr m_mapping;
};

//----------------------------------------------------------------------------//
// FrustumMappingIntersection
//----------------------------------------------------------------------------//

class FrustumMappingIntersection : public BufferIntersection
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(FrustumMappingIntersection);

  // Ctor ----------------------------------------------------------------------

  FrustumMappingIntersection(Field3D::FrustumFieldMapping::Ptr mapping);

  // From BufferIntersection ---------------------------------------------------

  virtual IntervalVec intersect(const Ray &wsRay, const PTime time) const;

private:

  // Data members --------------------------------------------------------------

  //! Pointer to frustum mapping
  Field3D::FrustumFieldMapping::Ptr m_mapping;
};

//----------------------------------------------------------------------------//
// EmptySpaceOptimizer
//----------------------------------------------------------------------------//

class EmptySpaceOptimizer : public Util::ParamBase
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(EmptySpaceOptimizer);

  // To be implemented by subclasses -------------------------------------------

  virtual IntervalVec optimize(const RayState &state, 
                               const IntervalVec &intervals) const = 0;
};

//----------------------------------------------------------------------------//
// SparseUniformOptimizer
//----------------------------------------------------------------------------//

class SparseUniformOptimizer : public EmptySpaceOptimizer
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(SparseUniformOptimizer);

  // Ctor, factory -------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC_2_ARG(SparseUniformOptimizer, SparseBuffer::Ptr,
                               Field3D::MatrixFieldMapping::Ptr);
  SparseUniformOptimizer(SparseBuffer::Ptr sparse, 
                         Field3D::MatrixFieldMapping::Ptr mapping);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(SparseUniformOptimizer);

  // From EmptySpaceOptimizer --------------------------------------------------

  virtual IntervalVec optimize(const RayState &state, 
                               const IntervalVec &intervals) const;
private:

  // Utility methods -----------------------------------------------------------

  //! Constructs an Interval given the start and end blocks along the ray
  Interval            intervalForRun(const Ray &wsRay, const PTime time,
                                     const Imath::V3i &start, 
                                     const Imath::V3i &end) const;
  //! Intersects the ray with the given sparse block
  void                intersect(const Ray &wsRay, const PTime time, 
                                const Imath::V3i block,
                                double &t0, double &t1) const;

  // Private data members ------------------------------------------------------

  //! Pointer to the sparse buffer
  SparseBuffer::Ptr m_sparse;
  //! Pointer to the uniform mapping
  Field3D::MatrixFieldMapping::Ptr m_mapping;
};

//----------------------------------------------------------------------------//
// SparseFrustumOptimizer
//----------------------------------------------------------------------------//

class SparseFrustumOptimizer : public EmptySpaceOptimizer
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(SparseFrustumOptimizer);

  // Ctor, factory -------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC_2_ARG(SparseFrustumOptimizer, SparseBuffer::Ptr,
                            Field3D::FrustumFieldMapping::Ptr);
  SparseFrustumOptimizer(SparseBuffer::Ptr sparse, 
                         Field3D::FrustumFieldMapping::Ptr mapping);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(SparseFrustumOptimizer);

  // From EmptySpaceOptimizer --------------------------------------------------

  virtual IntervalVec optimize(const RayState &state, 
                               const IntervalVec &intervals) const;
private:

  // Utility methods -----------------------------------------------------------

  Interval            intervalForRun(const Ray &wsRay, const PTime time, 
                                     const Vector &vsFirst, 
                                     const int start, const int end) const;

  // Private data members ------------------------------------------------------
  
  //! Pointer to the sparse buffer
  SparseBuffer::Ptr m_sparse;
  //! Pointer to the frustum mapping
  Field3D::FrustumFieldMapping::Ptr m_mapping;
};

//----------------------------------------------------------------------------//
// VoxelVolume
//----------------------------------------------------------------------------//

/*! \class VoxelVolume
  \brief Wraps a VoxelBuffer in the Volume interface.
 */

//----------------------------------------------------------------------------//

class VoxelVolume : public Volume
{
public:

  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(VoxelVolume);

  // Enums ---------------------------------------------------------------------
  
  enum InterpType {
    NoInterp,
    LinearInterp,
    CubicInterp, 
    MonotonicCubicInterp, 
    GaussianInterp,
    MitchellInterp
  };

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(MissingBufferException, "No buffer in VoxelVolume");
  DECLARE_PVR_RT_EXC(MissingMappingException, "No mapping in buffer");
  DECLARE_PVR_RT_EXC(UnsupportedMappingException, "Unsupported mapping type");

  // Ctor, factory -------------------------------------------------------------

  //! Default constructor. Initializes interp type.
  VoxelVolume();
  //! Specific factory method
  PVR_DEFINE_CREATE_FUNC(VoxelVolume);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(VoxelVolume);

  // From Volume ---------------------------------------------------------------

  virtual AttrNameVec  attributeNames() const;
  virtual VolumeSample sample(const VolumeSampleState &state,
                              const VolumeAttr &attribute) const;
  virtual BBox         wsBounds() const;
  virtual IntervalVec  intersect(const RayState &state) const;
  virtual StringVec    info() const;

  // Main methods --------------------------------------------------------------

  //! Loads a Field3D file from disk. 
  void                 load(const std::string &filename);
  //! Sets the voxel buffer.
  void                 setBuffer(VoxelBuffer::Ptr buffer);
  //! Adds an attribute to be exposed. The supplied value acts as a scaling
  //! factor on top of the density value sampled from the voxel buffer.
  void                 addAttribute(const std::string &attrName, 
                                    const Imath::V3f &value);
  //! Sets the interpolator type to use for lookups.
  void                 setInterpolation(const InterpType interpType);
  //! Sets whether to use empty space optimization.
  void                 setUseEmptySpaceOptimization(const bool enabled);

protected:

  // Typedefs ------------------------------------------------------------------

  typedef Field3D::LinearFieldInterp<Imath::V3f>   LinearInterpType;
  //! Cubic interpolator
  typedef Field3D::TriCubicFieldInterp<Imath::V3f> CubicInterpType;
  //! Monotonic cubic interpolator
  typedef Field3D::CubicFieldInterp<Imath::V3f>    MonotonicCubicInterpType;
  //! Gaussian interpolator
  typedef Field3D::GaussianFieldInterp<Imath::V3f> GaussianInterpType;
  //! Mitchell-Netravali interpolator
  typedef Field3D::MitchellFieldInterp<Imath::V3f> MitchellInterpType;

  // Utility methods -----------------------------------------------------------

  void                 updateIntersectionHandler();

  // Protected data members ----------------------------------------------------

  //! Voxel buffer
  VoxelBuffer::Ptr          m_buffer;
  //! World space bounds
  BBox                      m_wsBounds;
  //! Attribute names
  AttrNameVec               m_attrNames;
  //! Attribute scaling values
  std::vector<Imath::V3f>   m_attrValues;
  //! Handles ray/buffer intersection tests
  BufferIntersection::CPtr  m_intersectionHandler;
  //! Interpolation type to use for lookups
  InterpType                m_interpType;
  //! Linear interpolator
  LinearInterpType          m_linearInterp;
  //! Cubic interpolator
  CubicInterpType           m_cubicInterp;
  //! Monotonic cubic interpolator
  MonotonicCubicInterpType  m_monotonicCubicInterp;
  //! Gaussian interpolator
  GaussianInterpType        m_gaussInterp;
  //! Mitchell-Netravali interpolator
  MitchellInterpType        m_mitchellInterp;
  //! Empty space optimizer. May be null.
  EmptySpaceOptimizer::CPtr m_eso;
  //! Whether to use empty space optimization
  bool                      m_useEmptySpaceOptimization;

};

//----------------------------------------------------------------------------//

Interval makeInterval(const Ray &wsRay, const double t0, const double t1,
                      Field3D::FieldMapping::Ptr mapping);

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
