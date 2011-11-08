//----------------------------------------------------------------------------//

/*! \file Polygons.cpp
  Contains implementations of Polygons class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Polygons.h"

// System includes

// Project includes

#include "pvr/Log.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//



  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// Polygons static members
//----------------------------------------------------------------------------//

const int Polygons::UninitVertexIdx  = -1;

//----------------------------------------------------------------------------//
// Polygons
//----------------------------------------------------------------------------//

Polygons::Polygons()
{
  // Set up "P" point attribute
  m_pRef = m_pointAttrs.addVectorAttr("P", Imath::V3f(0.0f));
  // Set up "is_closed" point attribute
  std::vector<int> isClosedDefault(1, 1);
  m_isClosedRef = m_polyAttrs.addIntAttr("is_closed", 1, isClosedDefault);
  // Check that attrs were created successfully
  if (!m_pRef.isValid()) {
    throw ConstructionException("Couldn't create P attribute");
  }
  if (!m_isClosedRef.isValid()) {
    throw ConstructionException("Couldn't create is_closed attribute");
  }
}

//----------------------------------------------------------------------------//

size_t Polygons::size() const
{
  return m_polyAttrs.size();
}

//----------------------------------------------------------------------------//

size_t Polygons::addPoint()
{ 
  m_pointAttrs.addElems(1);
  return m_pointAttrs.size() - 1;
}
  
//----------------------------------------------------------------------------//

size_t Polygons::addPolygon(const size_t numVertices)
{ 
  assert(m_polyInfo.size() == m_polyAttrs.size());

  PolyInfo info(numVertices, m_vertexTable.size());
  m_polyInfo.push_back(info);
  m_polyAttrs.addElems(1);
  for (size_t i = 0; i < numVertices; ++i) {
    m_vertexTable.push_back(UninitVertexIdx);
  }
  return m_polyAttrs.size() - 1;
}

//----------------------------------------------------------------------------//

size_t Polygons::numVertices(const size_t polyIdx) const
{
  assert(polyIdx < m_polyInfo.size() && "Poly index out of bounds");
  return m_polyInfo[polyIdx].numVertices;
}

//----------------------------------------------------------------------------//

void Polygons::setVertex(const size_t polyIdx, const size_t vertIdx,
                         const size_t pointIdx)
{
  size_t startIdx = m_polyInfo[polyIdx].vertexTableOffset;
  assert(startIdx + vertIdx < m_vertexTable.size());
  m_vertexTable[startIdx + vertIdx] = pointIdx;
}
  
//----------------------------------------------------------------------------//

void Polygons::setVertices(const size_t polyIdx, 
                           const std::vector<size_t> pointIndices)
{
  assert(polyIdx < m_polyInfo.size() && "Poly index out of bounds");
  assert(m_polyInfo[polyIdx].numVertices == pointIndices.size() && 
         "Vertex count mismatch");
  for (size_t i = 0; i < m_polyInfo[polyIdx].numVertices; ++i) {
    setVertex(polyIdx, i, pointIndices[i]);
  }  
}

//----------------------------------------------------------------------------//

size_t Polygons::pointForVertex(const size_t polyIdx, 
                                const size_t vertIdx) const
{
  assert(polyIdx < m_polyInfo.size() && "Poly index out of bounds");
  size_t startIdx = m_polyInfo[polyIdx].vertexTableOffset;
  assert(startIdx + vertIdx < m_vertexTable.size());
  return m_vertexTable[startIdx + vertIdx];  
}
  
//----------------------------------------------------------------------------//

bool Polygons::isClosed(const size_t polyIdx) const
{
  return static_cast<bool>(m_polyAttrs.intAttr(m_isClosedRef, polyIdx, 0));
}

//----------------------------------------------------------------------------//

void Polygons::setIsClosed(const size_t polyIdx, bool closed)
{
  m_polyAttrs.setIntAttr(m_isClosedRef, polyIdx, 0, static_cast<int>(closed));
}

//----------------------------------------------------------------------------//

AttrTable& Polygons::pointAttrs()
{ 
  return m_pointAttrs; 
}

//----------------------------------------------------------------------------//

const AttrTable& Polygons::pointAttrs() const
{ 
  return m_pointAttrs; 
}

//----------------------------------------------------------------------------//

AttrTable& Polygons::polyAttrs()
{ 
  return m_polyAttrs; 
}

//----------------------------------------------------------------------------//

const AttrTable& Polygons::polyAttrs() const
{ 
  return m_polyAttrs; 
}

//----------------------------------------------------------------------------//

void Polygons::print() const
{
  using namespace pvr::Util;
  for (size_t iPoly = 0; iPoly < m_polyInfo.size(); ++iPoly) {
    Log::print("Polygon " + str(iPoly));
    for (size_t iVert = 0; iVert < m_polyInfo[iPoly].numVertices; ++iVert) {
      Log::print("  Vertex " + str(iVert) + " -> " + 
                 str(pointForVertex(iPoly, iVert)));
    }
  }
}

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//
