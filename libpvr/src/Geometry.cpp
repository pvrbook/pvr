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

/*! \file Geometry.cpp
  Contains implementations of Geometry class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Geometry.h"

// System includes

#include <string>

// Library includes

#include <boost/foreach.hpp>

#include <GPD-pvr/GPD_Detail.h>
#include <GPD-pvr/GPD_Point.h>
#include <GPD-pvr/GPD_PrimMesh.h>
#include <GPD-pvr/GPD_PrimPart.h>
#include <GPD-pvr/GPD_PrimPoly.h>

// Project includes

#include "pvr/Log.h"

//----------------------------------------------------------------------------//

#ifdef GPD_VERSION_NS 
using namespace SPI::GPD;
#endif

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace std;
  using namespace Imath;
  using namespace pvr;
  using namespace pvr::Geo;

  //--------------------------------------------------------------------------//
  // AttrTraits<T>
  //--------------------------------------------------------------------------//

  template <class T>
  struct AttrTraits
  {
    static AttrRef addAttr(const std::string &name, const size_t arraySize, 
                           const std::vector<T> defaults, AttrTable &points);
    static AttrRef attrRef(const std::string &name, const AttrTable &points);
    static void setAttr   (const AttrRef &ref, const size_t idx, 
                           const size_t arrayIdx, const T value, 
                           AttrTable &points);
  };

  //--------------------------------------------------------------------------//

  template <>
  struct AttrTraits<int>
  {
    static AttrRef addAttr(const std::string &name, const size_t arraySize, 
                           const std::vector<int> defaults, AttrTable &points)
    { 
      return points.addIntAttr(name, arraySize, defaults); 
    }
    static AttrRef attrRef(const std::string &name, const AttrTable &points)
    {
      return points.intAttrRef(name);
    }
    static void setAttr(const AttrRef &ref, const size_t idx, 
                        const size_t arrayIdx, const int value, 
                        AttrTable &points)
    { 
      points.setIntAttr(ref, idx, arrayIdx, value); 
    }
  };

  //--------------------------------------------------------------------------//

  template <>
  struct AttrTraits<float>
  {
    static AttrRef addAttr(const std::string &name, const size_t arraySize, 
                           const std::vector<float> defaults, AttrTable &points)
    { 
      return points.addFloatAttr(name, arraySize, defaults); 
    }
    static AttrRef attrRef(const std::string &name, const AttrTable &points)
    {
      return points.floatAttrRef(name);
    }
    static void setAttr(const AttrRef &ref, const size_t idx, 
                        const size_t arrayIdx, const float value, 
                        AttrTable &points)
    { 
      points.setFloatAttr(ref, idx, arrayIdx, value); 
    }
  };

  //--------------------------------------------------------------------------//
  // Utility functions
  //--------------------------------------------------------------------------//

  template <class T>
  void addAttr(GPD_Detail &detail, const GPD_Attribute *attrPtr, 
               const GPD_AttribType attrType, 
               const std::vector<int> & vertList, 
               const std::vector<int> & pvrVertList, AttrTable &points)
  {
    const size_t    numVerts = vertList.size();
    const string    attrName = attrPtr->getName();
    const int       hAttrSize = attrPtr->getSize();
    const int       pvrAttrSize = hAttrSize / sizeof(T);
    const vector<T> defaults(pvrAttrSize, 0);
    const int       offset = detail.pointAttribs().getOffset(attrName.c_str(), 
                                                             hAttrSize, 
                                                             attrType);
    AttrRef         attrRef = AttrTraits<T>::attrRef(attrName, points);
    if (!attrRef.isValid()) {
       attrRef = AttrTraits<T>::addAttr(attrName, pvrAttrSize, 
                                        defaults, points);
    }
    for (size_t idx = 0; idx < numVerts; idx++) {
      for (int arrayIdx = 0; arrayIdx < pvrAttrSize; arrayIdx++) {
#ifdef GPD_VERSION_NS
        const GPD_Point *point = &detail.points[vertList[idx]];
#else
        const GPD_Point *point = detail.point(vertList[idx]);
#endif
        void *data = point->attribData.getData(offset + arrayIdx * sizeof(T));
        T val = *reinterpret_cast<T *>(data);
        AttrTraits<T>::setAttr(attrRef, pvrVertList[idx], arrayIdx, val, points);
      }
    }
  }

  //--------------------------------------------------------------------------//
  
  void addVectorAttr(GPD_Detail &detail, const GPD_Attribute *attrPtr, 
                     const GPD_AttribType attrType, 
                     const std::vector<int> & vertList, 
                     const std::vector<int> & pvrVertList, AttrTable &points)
  {
    const size_t    numVerts = vertList.size();
    const string    attrName = attrPtr->getName();
    const int       hAttrSize = attrPtr->getSize();
    const V3f       defaults(0.0);
    const int       offset = detail.pointAttribs().getOffset(attrName.c_str(), 
                                                             hAttrSize, 
                                                             attrType);
    AttrRef         attrRef = points.vectorAttrRef(attrName);
    if (!attrRef.isValid()) {
       attrRef = points.addVectorAttr(attrName, defaults);
    }
    for (size_t idx = 0; idx < numVerts; idx++) {
#ifdef GPD_VERSION_NS
      void *data = 
        detail.points[vertList[idx]].attribData.getData(offset);
#else
      void *data = 
        detail.point(vertList[idx])->attribData.getData(offset);
#endif
      V3f val = *reinterpret_cast<V3f *>(data);
      points.setVectorAttr(attrRef, pvrVertList[idx], val);
    }
  }

  //--------------------------------------------------------------------------//

  void addStringAttr(GPD_Detail &detail, const GPD_Attribute *attrPtr, 
                     const GPD_AttribType attrType, 
                     const std::vector<int> & vertList, 
                     const std::vector<int> & pvrVertList, AttrTable &points)
  {
    const size_t    numVerts = vertList.size();
    const string    attrName = attrPtr->getName();
    const int       hAttrSize = attrPtr->getSize();
    const V3f       defaults(0.0);
    const int       offset = detail.pointAttribs().getOffset(attrName.c_str(), 
                                                             hAttrSize, 
                                                             attrType);
    AttrRef         attrRef = points.stringAttrRef(attrName);
    if (!attrRef.isValid()) {
       attrRef = points.addStringAttr(attrName);
    }
    for (size_t idx = 0; idx < numVerts; idx++) {
#ifdef GPD_VERSION_NS
      void *data = 
        detail.points[vertList[idx]].attribData.getData(offset);
#else
      void *data = 
        detail.point(vertList[idx])->attribData.getData(offset);
#endif
      int strIdx = *reinterpret_cast<int *>(data);
#ifdef GPD_VERSION_NS
      const char *str = attrPtr->indexArray[strIdx];
#else
      const char *str = attrPtr->getIndex(strIdx);
#endif
      points.setStringAttr(attrRef, pvrVertList[idx], string(str));
    }
  }
  
  //--------------------------------------------------------------------------//



  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace Imath;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {

//----------------------------------------------------------------------------//
// Geometry
//----------------------------------------------------------------------------//

Geometry::Geometry()
{
  m_globalAttrs.resize(1);
}
  
//----------------------------------------------------------------------------//

Geometry::Ptr Geometry::create()
{
  return Ptr(new Geometry);
}

//----------------------------------------------------------------------------//

//! \todo Geometry::read is not done!
Geometry::Ptr Geometry::read(const std::string &filename)
{
  Geometry::Ptr geo = Geometry::create();
  Particles::Ptr parts = Particles::create();
  Polygons::Ptr polys = Polygons::create();
  Meshes::Ptr meshes = Meshes::create();

  GPD_Detail detail;
  int status = detail.load(filename.c_str());
  if (status < 0) {
    Log::warning("Couldn't load (B)GEO: " + filename);
    return Geometry::Ptr();
  }

  // For each prim in file ---

  for (int iPrim = 0, size = detail.numprim(); iPrim < size; iPrim++) {

    GPD_Primitive *prim = detail.prim(iPrim);

    const GPD_PrimParticle *particlePrim = 
      dynamic_cast<GPD_PrimParticle*>(prim);
    const GPD_PrimPoly *polyPrim = 
      dynamic_cast<GPD_PrimPoly*>(prim);
    GPD_PrimMesh *meshPrim = 
      dynamic_cast<GPD_PrimMesh*>(prim);

    if (particlePrim) {

      AttrTable &points = parts->pointAttrs();

      size_t numVerts = particlePrim->nVtx;
      parts->add(numVerts);
        
      // First build vertex list ---

      vector<int> vertList(numVerts);
      vector<int> pvrVertList(numVerts);
      for (size_t idx = 0; idx < numVerts; idx++) {
        vertList[idx] = particlePrim->vtxList[idx].getPt()->getNum();
        pvrVertList[idx] = idx;
      }
      
      // Then get point positions ---

      AttrRef pRef = points.vectorAttrRef("P");
      for (size_t idx = 0; idx < numVerts; idx++) {
#ifdef GPD_VERSION_NS
        GPD_Point *p = &detail.points[vertList[idx]];
#else
        GPD_Point *p = detail.point(vertList[idx]);
#endif
        V3f wsP(p->pos[0], p->pos[1], p->pos[2]);
        points.setVectorAttr(pRef, idx, wsP);
      }
      
      // Then copy the other attributes ---

      int numPointAttrs = detail.pointAttribs().numAttribs();
      GPD_Attribute *attrPtr = detail.pointAttribs().getHead();
      
      for (int iAttr = 0; iAttr < numPointAttrs; iAttr++) {
        GPD_AttribType attrType = attrPtr->getType();
        switch (attrType) {
        case GPD_ATTRIB_INT:
          addAttr<int>(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_FLOAT:
          addAttr<float>(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_VECTOR:
          addVectorAttr(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_INDEX:
          addStringAttr(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        default:
          {
            // Nothing
          }
        }

        // Advance to next attr. This is ugly but it's just how GPD works.
        attrPtr = static_cast<GPD_Attribute *>(attrPtr->next());
      }

    } else if (polyPrim) {

      AttrTable &points = polys->pointAttrs();

      size_t numVerts = polyPrim->nVtx;
      bool isClosed = polyPrim->isClosed() != 0;

      size_t polyIdx = polys->addPolygon(numVerts);
      polys->setIsClosed(polyIdx, isClosed);
      
      // Add vertices ---

      for (size_t iVert = 0; iVert < numVerts; iVert++) {
        size_t pointIdx = polys->addPoint();
        polys->setVertex(polyIdx, iVert, pointIdx);
      }

      // First build vertex lists ---

      vector<int> vertList(numVerts);
      vector<int> pvrVertList(numVerts);
      for (size_t idx = 0; idx < numVerts; idx++) {
        vertList[idx] = polyPrim->vtxList[idx].getPt()->getNum();
        pvrVertList[idx] = polys->pointForVertex(polyIdx, idx);
      }

      // Then get point positions ---

      AttrRef pRef = polys->pointAttrs().vectorAttrRef("P");
      for (size_t iVert = 0; iVert < numVerts; iVert++) {
#ifdef GPD_VERSION_NS
        GPD_Point *p = &detail.points[vertList[iVert]];
#else
        GPD_Point *p = detail.point(vertList[iVert]);
#endif
        V3f wsP(p->pos[0], p->pos[1], p->pos[2]);
        points.setVectorAttr(pRef, pvrVertList[iVert], wsP);
      }

      // Then copy the other attributes ---

      int numPointAttrs = detail.pointAttribs().numAttribs();
      GPD_Attribute *attrPtr = detail.pointAttribs().getHead();
      
      for (int iAttr = 0; iAttr < numPointAttrs; iAttr++) {
        GPD_AttribType attrType = attrPtr->getType();
        const string    attrName = attrPtr->getName();
        switch (attrType) {
        case GPD_ATTRIB_INT:
          addAttr<int>(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_FLOAT:
          addAttr<float>(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_VECTOR:
          addVectorAttr(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_INDEX:
          addStringAttr(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        default:
          {
            // Nothing
          }
        }
        // Advance to next attr. This is ugly but it's just how GPD works.
        attrPtr = static_cast<GPD_Attribute *>(attrPtr->next());
      }

    } else if (meshPrim) {
      
      AttrTable &points = meshes->pointAttrs();

      size_t numRows = meshPrim->numRows();
      size_t numCols = meshPrim->numCols();
      size_t numVerts = numRows * numCols;

      size_t meshIdx = meshes->addMesh(numRows, numCols);
      size_t firstVertex = meshes->startPoint(meshIdx);
      
      // First build vertex lists ---

      vector<int> vertList(numVerts);
      vector<int> pvrVertList(numVerts);
      size_t idx = 0;
      for (size_t row = 0; row < numRows; row++) {
        for (size_t col = 0; col < numCols; col++, idx++) {
          vertList[idx] = meshPrim->getVertex(row, col)->getPt()->getNum();
          pvrVertList[idx] = firstVertex + idx;
        }
      }

      // Then get point positions ---

      AttrRef pRef = meshes->pointAttrs().vectorAttrRef("P");
      for (size_t iVert = 0; iVert < numVerts; iVert++) {
#ifdef GPD_VERSION_NS
        GPD_Point *p = &detail.points[vertList[iVert]];
#else
        GPD_Point *p = detail.point(vertList[iVert]);
#endif
        V3f wsP(p->pos[0], p->pos[1], p->pos[2]);
        points.setVectorAttr(pRef, pvrVertList[iVert], wsP);
      }

      // Then copy the other attributes ---

      int numPointAttrs = detail.pointAttribs().numAttribs();
      GPD_Attribute *attrPtr = detail.pointAttribs().getHead();
      
      for (int iAttr = 0; iAttr < numPointAttrs; iAttr++) {
        GPD_AttribType attrType = attrPtr->getType();
        const string    attrName = attrPtr->getName();
        switch (attrType) {
        case GPD_ATTRIB_INT:
          addAttr<int>(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_FLOAT:
          addAttr<float>(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_VECTOR:
          addVectorAttr(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        case GPD_ATTRIB_INDEX:
          addStringAttr(detail, attrPtr, attrType, vertList, pvrVertList, points);
          break;
        default:
          {
            // Nothing
          }
        }
        // Advance to next attr. This is ugly but it's just how GPD works.
        attrPtr = static_cast<GPD_Attribute *>(attrPtr->next());
      }

    } else {
      
    }

  }

  Log::print("Loaded file: " + filename);

  // Finish up ---
  
  geo->setParticles(parts);
  geo->setPolygons(polys);
  geo->setMeshes(meshes);
  
  return geo;
}

//----------------------------------------------------------------------------//

void Geometry::setParticles(Particles::Ptr particles)
{
  m_particles = particles;
}
  
//----------------------------------------------------------------------------//

void Geometry::setPolygons(Polygons::Ptr polygons)
{
  m_polygons = polygons;
}
  
//----------------------------------------------------------------------------//

void Geometry::setMeshes(Meshes::Ptr meshes)
{
  m_meshes = meshes;
}
  
//----------------------------------------------------------------------------//

Particles::Ptr Geometry::particles()
{
  return m_particles;
}

//----------------------------------------------------------------------------//

Particles::CPtr Geometry::particles() const
{
  return m_particles;
}
  
//----------------------------------------------------------------------------//

Polygons::Ptr Geometry::polygons()
{
  return m_polygons;
}

//----------------------------------------------------------------------------//

Polygons::CPtr Geometry::polygons() const
{
  return m_polygons;
}

//----------------------------------------------------------------------------//

Meshes::Ptr Geometry::meshes()
{
  return m_meshes;
}

//----------------------------------------------------------------------------//

Meshes::CPtr Geometry::meshes() const
{
  return m_meshes;
}

//----------------------------------------------------------------------------//

AttrTable& Geometry::globalAttrs()
{ 
  return m_globalAttrs; 
}
  
//----------------------------------------------------------------------------//

const AttrTable& Geometry::globalAttrs() const
{ 
  return m_globalAttrs; 
}

//----------------------------------------------------------------------------//

} // namespace pvr

//----------------------------------------------------------------------------//
