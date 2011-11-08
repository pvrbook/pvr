//----------------------------------------------------------------------------//

/*! \file SpherePrimitive.cpp
  Contains implementations of SpherePrimitive class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Primitives/SpherePrimitive.h"

// System includes

// Library includes

// Project includes

#include "pvr/Constants.h"
#include "pvr/Interrupt.h"
#include "pvr/Log.h"
#include "pvr/Math.h"
#include "pvr/AttrSampler.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace pvr;
  using namespace Field3D;

  //--------------------------------------------------------------------------//

  void getDerivatives(Field3D::FieldMapping::Ptr mapping, 
                      const int i, const int j, const int k,
                      Model::SamplingDerivatives &derivs)
  {
    Vector wsCenter, wsX, wsY, wsZ;
    mapping->voxelToWorld(discToCont(V3i(i, j, k)), wsCenter);
    mapping->voxelToWorld(discToCont(V3i(i + 1, j, k)), wsX);
    mapping->voxelToWorld(discToCont(V3i(i, j + 1, k)), wsY);
    mapping->voxelToWorld(discToCont(V3i(i, j, k + 1)), wsZ);
    derivs.dPdx = wsX - wsCenter;
    derivs.dPdy = wsY - wsCenter;
    derivs.dPdz = wsZ - wsCenter;
  }

  //--------------------------------------------------------------------------//

  float evaluateSphere(const Vector &wsP, const Vector &wsCenter, 
                       const float wsRadius, const float density,
                       const float falloffWidth)
  {
    float dist = (wsCenter - wsP).length();
    return Math::fit(dist, falloffWidth * wsRadius, wsRadius, 
                     1.0f, 0.0f) * density;
  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace pvr::Geo;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {

//----------------------------------------------------------------------------//
// SpherePrimitive
//----------------------------------------------------------------------------//

SpherePrimitive::Ptr SpherePrimitive::create()
{
  return Ptr(new SpherePrimitive);
}

//----------------------------------------------------------------------------//

BBox SpherePrimitive::wsBounds(Geo::Geometry::CPtr geometry) const
{
  assert(geometry != NULL);
  assert(geometry->particles() != NULL);

  if (!geometry->particles()) {
    Log::warning("SpherePrimitive has no particles. "
                 "Skipping bounds generation.");
    return BBox();
  }

  BBox wsBBox;

  AttrVisitor visitor(geometry->particles()->pointAttrs(), m_params);
  Attr<V3f>   wsCenter("P");
  Attr<float> radius("radius");

  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i) {
    i.update(wsCenter);
    i.update(radius);
    wsBBox.extendBy(wsCenter.as<Vector>() + radius.as<Vector>());
    wsBBox.extendBy(wsCenter.as<Vector>() - radius.as<Vector>());
  }

  return wsBBox;
}

//----------------------------------------------------------------------------//
  
void SpherePrimitive::execute(Geo::Geometry::CPtr geometry,
                              ScalarBuffer::Ptr buffer) const
{
  Log::print("Rasterizing SpherePrimitive instances");

  SamplingDerivatives wsDerivs;
  getDerivatives(buffer->mapping(), 0, 0, 0, wsDerivs);

  FieldMapping::Ptr mapping = buffer->mapping();
    
  AttrVisitor visitor(geometry->particles()->pointAttrs(), m_params);
  Attr<V3f>   wsCenter("P");
  Attr<float> radius  ("radius");
  Attr<float> density ("density");

  Timer timer;

  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i) {

    // Check if user terminated
    Sys::Interrupt::throwOnAbort();

    // Update per-point attributes
    i.update(wsCenter);
    i.update(radius);
    i.update(density);

    // Calculate rasterization bounds
    BBox vsBounds = calculateVsBounds(mapping, wsCenter.as<Vector>(), radius);
    DiscreteBBox dvsBounds = Math::discreteBounds(vsBounds);

    // Iterate over voxels
    for (ScalarBuffer::iterator i = buffer->begin(dvsBounds), 
           end = buffer->end(dvsBounds); i != end; ++i) {
      Vector vsP = discToCont(V3i(i.x, i.y, i.z)), wsP;
      mapping->voxelToWorld(vsP, wsP);
      float value = evaluateSphere(wsP, wsCenter.as<Vector>(), 
                                   radius, density, 0.9f);
      if (value > 0.0f) {
        *i += value;
      }
    }
  }

  Log::print("Sphere primitive processed " + str(points.size()) + 
             " input points");
  Log::print("  Time elapsed: " + str(timer.elapsed()));
}

//----------------------------------------------------------------------------//

} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//
