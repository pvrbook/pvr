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
using namespace Imath;

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

void Meshes::computeDerivatives() 
{
  AttrRef dPdsRef = m_pointAttrs.addVectorAttr("dPds", Imath::V3f(0.0f));
  AttrRef dPdtRef = m_pointAttrs.addVectorAttr("dPdt", Imath::V3f(0.0f));
  for (size_t iMesh = 0, endMesh = m_meshAttrs.size(); 
       iMesh < endMesh; ++iMesh) {
    for (size_t row = 0, endRow = numRows(iMesh); row < endRow; ++row) {
      for (size_t col = 0, endCol = numCols(iMesh); col < endCol; ++col) {
        // Variables to compute
        V3f dPds(0.0f), dPdt(0.0f);
        // Center point
        size_t pointIdx = pointForVertex(iMesh, row, col);
        V3f P = m_pointAttrs.vectorAttr(m_pRef, pointIdx);
        // Compute dPds
        if (col == 0) {
          // One-sided
          size_t Pp1Idx = pointForVertex(iMesh, row, col + 1);
          V3f Pp1 = m_pointAttrs.vectorAttr(m_pRef, Pp1Idx);
          dPds = Pp1 - P;
        } else if (col == endCol - 1) {
          // One-sided
          size_t Pm1Idx = pointForVertex(iMesh, row, col - 1);
          V3f Pm1 = m_pointAttrs.vectorAttr(m_pRef, Pm1Idx);
          dPds = P - Pm1;
        } else {
          // Central difference
          size_t Pp1Idx = pointForVertex(iMesh, row, col + 1);
          size_t Pm1Idx = pointForVertex(iMesh, row, col - 1);
          V3f Pp1 = m_pointAttrs.vectorAttr(m_pRef, Pp1Idx);
          V3f Pm1 = m_pointAttrs.vectorAttr(m_pRef, Pm1Idx);
          dPds = (Pp1 - Pm1) * 0.5;
        }
        // Compute dPdt
        if (row == 0) {
          // One-sided
          size_t Pp1Idx = pointForVertex(iMesh, row + 1, col);
          V3f Pp1 = m_pointAttrs.vectorAttr(m_pRef, Pp1Idx);
          dPdt = Pp1 - P;
        } else if (row == endRow - 1) {
          // One-sided
          size_t Pm1Idx = pointForVertex(iMesh, row - 1, col);
          V3f Pm1 = m_pointAttrs.vectorAttr(m_pRef, Pm1Idx);
          dPdt = P - Pm1;
        } else {
          // Central difference
          size_t Pp1Idx = pointForVertex(iMesh, row + 1, col);
          size_t Pm1Idx = pointForVertex(iMesh, row - 1, col);
          V3f Pp1 = m_pointAttrs.vectorAttr(m_pRef, Pp1Idx);
          V3f Pm1 = m_pointAttrs.vectorAttr(m_pRef, Pm1Idx);
          dPdt = (Pp1 - Pm1) * 0.5;
        }
        // Update attributes
        m_pointAttrs.setVectorAttr(dPdsRef, pointIdx, dPds);
        m_pointAttrs.setVectorAttr(dPdtRef, pointIdx, dPdt);
      }
    }
  }
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
