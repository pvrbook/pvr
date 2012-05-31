//-*-c++-*--------------------------------------------------------------------//

/*! \file Meshes.h
  Contains the Meshes class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_MESHES_H__
#define __INCLUDED_PVR_MESHES_H__

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
// Meshes
//----------------------------------------------------------------------------//

/*! \class Meshes
  \brief Holds a collection of meshes. It is primarily intended for
  fast sequential insertion and querying. Modifying the structure once created
  (for example deleting existing meshes) is not supported.
 */

//----------------------------------------------------------------------------//

class Meshes
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Meshes);

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(ConstructionException, 
                     "Failed to create Meshes object:");

  // Constructor, destructor, factory ------------------------------------------

  //! Constructs an empty Meshes object with "P" point attribute 
  Meshes();
  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC(Meshes);

  // Main methods --------------------------------------------------------------
  
  //! Returns number of meshes in collection
  size_t           size() const;
  //! Adds a mesh to the collection
  //! \returns The index of the mesh
  size_t           addMesh(const size_t numRows, const size_t numCols);
  //! Number of rows
  size_t           numRows(const size_t meshIdx) const;
  //! Number of columns
  size_t           numCols(const size_t meshIdx) const;
  //! Returns the first vertex of a given mesh index
  size_t           startPoint(const size_t meshIdx) const;
  //! Returns the point index of the given vertex
  size_t           pointForVertex(const size_t meshIdx, 
                                  const size_t row, const size_t col) const;
  //! Access to point data
  AttrTable&       pointAttrs();
  //! Const access to point data
  const AttrTable& pointAttrs() const;
  //! Access to mesh data
  AttrTable&       meshAttrs();
  //! Const access to primitive data
  const AttrTable& meshAttrs() const;
  //! Computes dPds and dPdt derivatives
  void             computeDerivatives();

  // Debugging -----------------------------------------------------------------

  //! DEBUG
  void print() const;

protected:
  
  // Structs -------------------------------------------------------------------

  struct MeshInfo
  {
    MeshInfo()
      : numRows(0), numCols(0), startPoint(0)
    { }
    MeshInfo(size_t rows, size_t cols, size_t start)
      : numRows(rows), numCols(cols), startPoint(start)
    { }
    size_t numRows, numCols;
    size_t startPoint;
  };

  // Enums ---------------------------------------------------------------------

  static const int UninitVertexIdx;

  // Utility methods -----------------------------------------------------------

  // Protected data members ----------------------------------------------------

  //! Reference to P attribute
  AttrRef m_pRef;

  //! Point attributes
  AttrTable m_pointAttrs;

  //! Mesh attributes. 
  AttrTable m_meshAttrs;

  //! Per-mesh information
  std::vector<MeshInfo> m_meshInfo;

};

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
