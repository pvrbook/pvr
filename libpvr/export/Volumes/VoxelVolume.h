//-*-c++-*--------------------------------------------------------------------//

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

  DECLARE_SMART_PTRS(BufferIntersection);

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

  DECLARE_SMART_PTRS(UniformMappingIntersection);

  // Ctor ----------------------------------------------------------------------

  UniformMappingIntersection(Field3D::MatrixFieldMapping::Ptr mapping);

  // From BufferIntersection ---------------------------------------------------

  virtual IntervalVec intersect(const Ray &wsRay, const PTime time) const;

private:

  // Data members --------------------------------------------------------------

  Matrix m_worldToLocal, m_worldToVoxel;
};

//----------------------------------------------------------------------------//
// FrustumMappingIntersection
//----------------------------------------------------------------------------//

class FrustumMappingIntersection : public BufferIntersection
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(FrustumMappingIntersection);

  // Ctor ----------------------------------------------------------------------

  FrustumMappingIntersection(Field3D::FrustumFieldMapping::Ptr mapping);

  // From BufferIntersection ---------------------------------------------------

  virtual IntervalVec intersect(const Ray &wsRay, const PTime time) const;

private:

  // Data members --------------------------------------------------------------

  //! Pointer to frustum mapping
  Field3D::FrustumFieldMapping::Ptr m_mapping;
  //! The six planes that make up the frustum
  //! \note Order of planes: Left, Right, Bottom, Top, Near, Far
  Plane m_planes[6];
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

  DECLARE_SMART_PTRS(VoxelVolume);

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
  static Ptr create()
  { return Ptr(new VoxelVolume); }

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "VoxelVolume"; }

  // From Volume ---------------------------------------------------------------

  virtual AttrNameVec attributeNames() const;
  virtual VolumeSample sample(const VolumeSampleState &state,
                              const VolumeAttr &attribute) const;
  virtual IntervalVec intersect(const RayState &state) const;
  virtual StringVec info() const;

  // Main methods --------------------------------------------------------------

  //! Loads a Field3D file from disk. 
  void load(const std::string &filename);
  //! Sets the voxel buffer.
  void setBuffer(VoxelBuffer::Ptr buffer);
  //! Adds an attribute to be exposed. The supplied value acts as a scaling
  //! factor on top of the density value sampled from the voxel buffer.
  void addAttribute(const std::string &attrName, const Imath::V3f &value);
  //! Sets the interpolator type to use for lookups.
  void setInterpolation(const InterpType interpType);

protected:

  // Utility methods -----------------------------------------------------------

  void updateIntersectionHandler();

  // Protected data members ----------------------------------------------------

  //! Voxel buffer
  VoxelBuffer::Ptr m_buffer;
  //! Attribute names
  AttrNameVec m_attrNames;
  //! Attribute scaling values
  std::vector<Imath::V3f> m_attrValues;
  //! Handles ray/buffer intersection tests
  BufferIntersection::CPtr m_intersectionHandler;
  //! Interpolation type to use for lookups
  InterpType m_interpType;
  //! Linear interpolator
  Field3D::LinearFieldInterp<Imath::V3f> m_linearInterp;
  //! Cubic interpolator
  Field3D::TriCubicFieldInterp<Imath::V3f> m_cubicInterp;
  //! Monotonic cubic interpolator
  Field3D::CubicFieldInterp<Imath::V3f> m_monotonicCubicInterp;
  //! Gaussian interpolator
  Field3D::GaussianFieldInterp<Imath::V3f> m_gaussInterp;
  //! Mitchell-Netravali interpolator
  Field3D::MitchellFieldInterp<Imath::V3f> m_mitchellInterp;

};

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
