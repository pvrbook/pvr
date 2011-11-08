//----------------------------------------------------------------------------//

/*! \file Polygons.cpp
  Contains implementations of Polygons class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Meshes.h"

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
// Meshes
//----------------------------------------------------------------------------//

Meshes::Meshes()
{
  m_pRef = m_pointAttrs.addVectorAttr("P", Imath::V3f(0.0f));
}

//----------------------------------------------------------------------------//

size_t Meshes::size() const
{
  return m_meshInfo.size();
}

//----------------------------------------------------------------------------//

size_t Meshes::addMesh(const size_t numRows, const size_t numCols)
{
  size_t numPoints = numRows * numCols;

  MeshInfo info(numRows, numCols, m_pointAttrs.size());
  m_meshInfo.push_back(info);

  m_pointAttrs.addElems(numPoints);
  m_meshAttrs.addElems(1);

  return m_meshAttrs.size() - 1;
}

//----------------------------------------------------------------------------//

size_t Meshes::numRows(const size_t meshIdx) const
{
  return m_meshInfo[meshIdx].numRows;
}

//----------------------------------------------------------------------------//

size_t Meshes::numCols(const size_t meshIdx) const
{
  return m_meshInfo[meshIdx].numCols;
}

//----------------------------------------------------------------------------//

size_t Meshes::startPoint(const size_t meshIdx) const
{
  return m_meshInfo[meshIdx].startPoint;
}

//----------------------------------------------------------------------------//

size_t Meshes::pointForVertex(const size_t meshIdx, 
                              const size_t row, const size_t col) const
{
  return startPoint(meshIdx) + row * numCols(meshIdx) + col;
}

//----------------------------------------------------------------------------//

AttrTable& Meshes::pointAttrs()
{
  return m_pointAttrs;
}

//----------------------------------------------------------------------------//

const AttrTable& Meshes::pointAttrs() const
{
  return m_pointAttrs;
}

//----------------------------------------------------------------------------//

AttrTable& Meshes::meshAttrs()
{
  return m_meshAttrs;
}

//----------------------------------------------------------------------------//

const AttrTable& Meshes::meshAttrs() const
{
  return m_meshAttrs;
}

//----------------------------------------------------------------------------//

void Meshes::print() const
{
  for (std::vector<MeshInfo>::const_iterator i = m_meshInfo.begin(), 
         end = m_meshInfo.end(); i != end; ++i) {
    Util::Log::print("Mesh: " + Util::str(i->numRows) + ", " + 
                     Util::str(i->numCols) + ", " + 
                     Util::str(i->startPoint));
#if 0
    size_t rows = i->numRows;
    size_t cols = i->numCols;
    size_t start = i->startPoint;
    size_t end = start + rows * cols;
    for (int i = start; i < end; ++i) {
      cout << m_pointAttrs.vectorAttr(m_pRef, i);
    }
    cout << endl;
#endif
  }
}

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//
