//----------------------------------------------------------------------------//

/*! \file Modeler.cpp
  Contains implementations of Modeler class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Modeler.h"

// System includes

// Library includes

#include <Field3D/Field3DFile.h>
#include <Field3D/FieldMapping.h>

// Project headers

#include "pvr/Constants.h"
#include "pvr/Log.h"
#include "pvr/Primitives/InstantiationPrim.h"
#include "pvr/Primitives/RasterizationPrim.h"
#include "pvr/Strings.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace pvr;

  //--------------------------------------------------------------------------//

  template <class Buffer_T>
  Vector computeBufferEdgeLengths(const Buffer_T &buffer)
  {
    Vector lsXMin(0.0, 0.5, 0.5);
    Vector lsYMin(0.5, 0.0, 0.5);
    Vector lsZMin(0.5, 0.5, 0.0);
    Vector lsXMax(1.0, 0.5, 0.5);
    Vector lsYMax(0.5, 1.0, 0.5);
    Vector lsZMax(0.5, 0.5, 1.0);
    Vector wsXMin, wsYMin, wsZMin, wsXMax, wsYMax, wsZMax;
    buffer.mapping()->localToWorld(lsXMin, wsXMin);
    buffer.mapping()->localToWorld(lsYMin, wsYMin);
    buffer.mapping()->localToWorld(lsZMin, wsZMin);
    buffer.mapping()->localToWorld(lsXMax, wsXMax);
    buffer.mapping()->localToWorld(lsYMax, wsYMax);
    buffer.mapping()->localToWorld(lsZMax, wsZMax);
    return Vector((wsXMax - wsXMin).length(),
                  (wsYMax - wsYMin).length(),
                  (wsZMax - wsZMin).length());
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
namespace Model {

//----------------------------------------------------------------------------//
// Modeler
//----------------------------------------------------------------------------//

Modeler::Modeler()
  : m_mapping(UniformMappingType), 
    m_dataStructure(DenseBufferType),
    m_sparseBlockSize(SparseBlockSize16)
{ 
  // Empty
}

//----------------------------------------------------------------------------//

Modeler::Ptr Modeler::create()
{ 
  return Ptr(new Modeler); 
}

//----------------------------------------------------------------------------//

Modeler::Ptr Modeler::clone()
{ 
  Modeler::Ptr temp = create();
  *temp = *this;
  return temp;
}
  
//----------------------------------------------------------------------------//

void Modeler::addInput(ModelerInput::Ptr modelerInput)
{
  if (!modelerInput->geometry()) {
    Log::warning("Modeler::addInput() got input without geometry. "
                 "Skipping");
    return;
  }

  if (!modelerInput->volumePrimitive()) {
    Log::warning("Modeler::addInput() got input without volume primitive. "
                 "Skipping");
    return;
  }

  m_inputs.push_back(modelerInput);
}
  
//----------------------------------------------------------------------------//

void Modeler::clearInputs()
{
  m_inputs.clear();
}

//----------------------------------------------------------------------------//

void Modeler::updateBounds()
{
  BBox wsBounds;
  
  BOOST_FOREACH (ModelerInput::CPtr i, m_inputs) {
    Prim::Primitive::CPtr prim = i->volumePrimitive();
    if (prim) {
      if (i->geometry()) {
        wsBounds.extendBy(prim->wsBounds(i->geometry()));
      } else {
        Log::warning("ModelerInput had missing Geometry (null pointer)");
      }
    } else {
      Log::warning("Got ModelerInput with missing Primitive (null pointer)");
    }
  }

  if (!wsBounds.hasVolume()) {
    Log::warning("Modeler::updateBounds() got an empty bounding box. "
                 "No voxel buffer will be created.");
    m_buffer = VoxelBuffer::Ptr();
    return;
  }

  Log::print("Updated bounds to: "
             "(" + str(wsBounds.min) + ", " + str(wsBounds.max) + ")");

  switch (m_dataStructure) {
  case SparseBufferType:
    {
      SparseBuffer::Ptr buffer(new SparseBuffer);
      Log::print("Creating sparse buffer");
      switch (m_sparseBlockSize) {
      case SparseBlockSize8:
        Log::print("  Setting block size to 8");
        buffer->setBlockOrder(3);
        break;
      case SparseBlockSize32:
        Log::print("  Setting block size to 32");
        buffer->setBlockOrder(5);
        break;
      case SparseBlockSize16:
      default:
        Log::print("  Setting block size to 16");
        buffer->setBlockOrder(4);
      }
      m_buffer = buffer;
    }
    break;
  case DenseBufferType:
  default:
    m_buffer = DenseBuffer::Ptr(new DenseBuffer);
    Log::print("Creating dense buffer");
    break;
  }

  switch (m_mapping) {
  case FrustumMappingType:
    {
      if (!m_camera) {
        Log::warning("Tried to construct frustum mapped buffer without "
                     "camera. Aborting.");
        m_buffer.reset();
        return;
      }
      setupFrustumMapping(wsBounds);
      Log::print("Using frustum mapping");
    }
    break;
  case UniformMappingType:
  default:
    {
      setupUniformMapping(wsBounds);
      Log::print("Using uniform/matrix mapping");
    }
  }

  m_buffer->attribute = "density";
  m_buffer->name = "pvr";
}

//----------------------------------------------------------------------------//

void Modeler::setResolution(const size_t x, const size_t y, const size_t z)
{
  if (!m_buffer) {
    Log::warning("Modeler::setResolution() was called before updateBounds()");
    return;
  }

  Log::print("Setting voxel buffer resolution to: " + str(V3i(x, y, z)));
  
  m_buffer->setSize(V3i(x, y, z));
  m_buffer->clear(Colors::zero());
}

//----------------------------------------------------------------------------//

void Modeler::setResolution(const size_t res)
{
  if (!m_buffer) {
    Log::warning("Modeler::setResolution() was called before updateBounds()");
    return;
  }

  Vector edgeLengths = computeBufferEdgeLengths(*(m_buffer));
  Vector edgeVoxelSize = edgeLengths / static_cast<double>(res);
  double voxelSize = max(max(edgeVoxelSize.x, edgeVoxelSize.y), 
                         edgeVoxelSize.z);
  // In the case of frustum mappings, we use only x/y resolution.
  if (m_mapping == FrustumMappingType) {
    voxelSize = max(edgeVoxelSize.x, edgeVoxelSize.y);
  }
  setVoxelSize(Vector(voxelSize));
}

//----------------------------------------------------------------------------//

void Modeler::setVoxelSize(const Vector &size)
{
  if (!m_buffer) {
    Log::warning("Modeler::setVoxelSize() was called before updateBounds()");
    return;
  }

  Vector edgeLengths = computeBufferEdgeLengths(*(m_buffer));
  Vector voxelsPerEdge = edgeLengths / size;
  size_t x = static_cast<size_t>(floor(voxelsPerEdge.x));
  size_t y = static_cast<size_t>(floor(voxelsPerEdge.y));
  size_t z = static_cast<size_t>(floor(voxelsPerEdge.z));
  setResolution(x, y, z);
}

//----------------------------------------------------------------------------//

void Modeler::setMapping(const Mapping mapping)
{
  m_mapping = mapping;
}

//----------------------------------------------------------------------------//

void Modeler::setDataStructure(const DataStructure dataStructure)
{
  m_dataStructure = dataStructure;
}

//----------------------------------------------------------------------------//

void Modeler::setSparseBlockSize(const SparseBlockSize size)
{
  m_sparseBlockSize = size;
}

//----------------------------------------------------------------------------//

void Modeler::setCamera(Render::PerspectiveCamera::CPtr camera)
{
  m_camera = camera;
}

//----------------------------------------------------------------------------//

void Modeler::execute()
{
  if (!m_buffer) {
    Log::warning("No voxel buffer created before calling Modeler::execute()");
    return;
  }

  BOOST_FOREACH (ModelerInput::Ptr i, m_inputs) {

    Prim::Primitive::CPtr prim = i->volumePrimitive();

    if (!prim) {
      Log::warning("Got null volume primitive in modeler input");
      continue;
    }

    Prim::Inst::InstantiationPrim::CPtr instPrim = 
      dynamic_pointer_cast<const Prim::Inst::InstantiationPrim>(prim);
    Prim::Rast::RasterizationPrim::CPtr rastPrim = 
      dynamic_pointer_cast<const Prim::Rast::RasterizationPrim>(prim);

    if (instPrim) {
      // Handle instantiation primitives
      ModelerInput::Ptr newInput = instPrim->execute(i->geometry());
      Modeler::Ptr modeler = clone();
      modeler->clearInputs();
      modeler->addInput(newInput);
      modeler->execute();
    } else if (rastPrim) {
      // Handle rasterization primitives
      rastPrim->execute(i->geometry(), m_buffer);
    } else {
      throw InvalidPrimitiveException(prim->typeName());
    }

  } 

  //! \todo Move out of this function.
  float mbUse = m_buffer->memSize() / (1024 * 1024);
  Log::print("Voxel buffer memory use: " + str(mbUse) + "MB");

  m_inputs.clear();
}
  
//----------------------------------------------------------------------------//

void Modeler::saveBuffer(const std::string &filename) const
{
  if (!m_buffer) {
    Log::print("No buffer available for output. Skipping.");
    return;
  }

  Log::print("Writing voxel buffer: " + filename);

  Field3DOutputFile out;
  out.create(filename);
  out.writeVectorLayer<float>(m_buffer);
  
  Log::print("  Done");
}

//----------------------------------------------------------------------------//

VoxelBuffer::Ptr Modeler::buffer() const
{
  return m_buffer;
}

//----------------------------------------------------------------------------//

void Modeler::setupFrustumMapping(const BBox &wsBounds) const
{
  using namespace Render;

  // Clone the camera so we can change the clip planes
  PerspectiveCamera::Ptr cam = m_camera->clone();
  // Check each corner vertex of the incoming bounds
  double near = std::numeric_limits<double>::max(), far = 0.0f;
  std::vector<Vector> wsCornerPoints = Math::cornerPoints(wsBounds);
  BOOST_FOREACH (const Vector &wsP, wsCornerPoints) {
    Vector csP = m_camera->worldToCamera(wsP, PTime(0.0));
    near = std::min(near, csP.z);
    far = std::max(far, csP.z);
  }
  // Clamp at zero
  near = std::max(near, 0.0);
  far = std::max(far, 0.0);
  // Set clip planes on cloned camera
  cam->setClipPlanes(near, far);
  // Copy the transforms from the camera
  FrustumFieldMapping::Ptr mapping(new FrustumFieldMapping);
  const Camera::MatrixVec& ssToWs = cam->screenToWorldMatrices();
  const Camera::MatrixVec& csToWs = cam->cameraToWorldMatrices();
  const size_t numSamples = ssToWs.size();
  for (size_t i = 0; i < numSamples; ++i) {
    float t = static_cast<float>(i) / (numSamples - 1);
    mapping->setTransforms(t, ssToWs[i], csToWs[i]);
  }
  m_buffer->setMapping(mapping);
}

//----------------------------------------------------------------------------//

void Modeler::setupUniformMapping(const BBox &wsBounds) const
{
  MatrixFieldMapping::Ptr mapping(new MatrixFieldMapping);
  Matrix offset, scaling;
  offset.setTranslation(wsBounds.min);
  scaling.setScale(wsBounds.size());
  mapping->setLocalToWorld(scaling * offset);
  m_buffer->setMapping(mapping);
}

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
