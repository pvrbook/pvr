//-*-c++-*--------------------------------------------------------------------//

/*! \file Polygons.h
  Contains the Polygons class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_POLYGONS_H__
#define __INCLUDED_PVR_POLYGONS_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/lexical_cast.hpp>

// Project headers

#include "pvr/AttrTable.h"
#include "pvr/Exception.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// Polygons
//----------------------------------------------------------------------------//

/*! \class Polygons
  \brief Holds a collection of polygons. It is primarily intended for
  fast sequential insertion and querying. Modifying the structure once created
  (for example deleting existing polygons) is not supported.
 */

//----------------------------------------------------------------------------//

class Polygons
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Polygons);

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(ConstructionException, 
                     "Failed to create Polygons object:");

  // Constructor, destructor, factory ------------------------------------------

  //! Constructs an empty Polygons object with "P" point attribute 
  Polygons();

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC(Polygons);

  // Main methods --------------------------------------------------------------
  
  //! Returns number of polygons in collection
  size_t size() const;

  //! Adds a point to the collection
  //! \returns The index of the point
  size_t addPoint();

  //! Adds a polygon to the collection
  //! \returns The index of the polygon
  size_t addPolygon(const size_t numVertices);

  //! Number of vertices for the given polygon
  size_t numVertices(const size_t polyIdx) const;

  //! Sets the vertex indices's for a given triangle
  void setVertex(const size_t polyIdx, const size_t vertIdx,
                 const size_t pointIdx);

  //! Sets the vertex indices's for a given triangle
  void setVertices(const size_t polyIdx, const std::vector<size_t> pointIndices);

  //! Returns the point index of the given vertex
  size_t pointForVertex(const size_t polyIdx, const size_t vertIdx) const;

  //! Whether the given polygon is closed or open
  bool isClosed(const size_t polyIdx) const;

  //! Sets whether the given polygon is closed or open
  void setIsClosed(const size_t polyIdx, bool closed);

  //! Access to point data
  AttrTable& pointAttrs();

  //! Const access to point data
  const AttrTable& pointAttrs() const;

  //! Access to polygon data
  AttrTable& polyAttrs();

  //! Const access to primitive data
  const AttrTable& polyAttrs() const;

  //! DEBUG
  void print() const;

protected:
  
  // Structs -------------------------------------------------------------------

  struct PolyInfo
  {
    PolyInfo()
      : numVertices(0), vertexTableOffset(0)
    { }
    PolyInfo(size_t verts, size_t offset)
      : numVertices(verts), vertexTableOffset(offset)
    { }
    size_t numVertices;
    size_t vertexTableOffset;
  };

  // Enums ---------------------------------------------------------------------

  static const int UninitVertexIdx;

  // Utility methods -----------------------------------------------------------

  // Protected data members ----------------------------------------------------

  //! Reference to P attribute
  AttrRef m_pRef;
  //! Reference to is_closed attribute
  AttrRef m_isClosedRef;

  //! Point attributes
  AttrTable m_pointAttrs;

  //! Polygon attributes. 
  AttrTable m_polyAttrs;

  //! Per-polygon information
  std::vector<PolyInfo> m_polyInfo;

  //! Vertex table. This refers to indices in m_pointAttrs. A negative value
  //! means the vertex is not yet initialized.
  std::vector<int> m_vertexTable;

};

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
