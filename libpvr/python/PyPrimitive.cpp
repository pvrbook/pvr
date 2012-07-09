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

/*! \file PyPrimitive.cpp
  Contains the interface definition for the Primitive class
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

#include <boost/shared_ptr.hpp>

#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <pvr/Primitives/Primitive.h>
#include <pvr/Primitives/InstantiationPrim.h>
#include <pvr/Primitives/Instantiation/Sphere.h>
#include <pvr/Primitives/Instantiation/Line.h>
#include <pvr/Primitives/Instantiation/Surface.h>
#include <pvr/Primitives/RasterizationPrim.h>
#include <pvr/Primitives/Rasterization/Point.h>
#include <pvr/Primitives/Rasterization/PyroclasticPoint.h>
#include <pvr/Primitives/Rasterization/Line.h>
#include <pvr/Primitives/Rasterization/PyroclasticLine.h>

#include "Common.h"

//----------------------------------------------------------------------------//
// Helper functions
//----------------------------------------------------------------------------//

void setParamsHelper(pvr::Model::Prim::Primitive &self, boost::python::dict d)
{
  self.setParams(pvr::dictToParamMap(d));
}

//----------------------------------------------------------------------------//
// Pvr python module
//----------------------------------------------------------------------------//

void exportPrimitive()
{
  using namespace std;
  using namespace boost;
  using namespace boost::python;

  namespace bp = boost::python;

  using namespace pvr;
  using namespace pvr::Model::Prim;

  // Prim scope
  {
    // Create Prim module
    object primModule(handle<>
                      (borrowed(PyImport_AddModule("pvr.Prim"))));
    scope().attr("Prim") = primModule;
    scope primScope = primModule;

    // Primitive
    class_<Primitive, Primitive::Ptr, boost::noncopyable>
      ("Primitive", no_init)
      .def("typeName", &Primitive::typeName)
      .def("setParams", &setParamsHelper)
      ;
    implicitly_convertible<Primitive::Ptr, Primitive::CPtr>();
    
    // Inst scope
    {
      // Create Inst module
      object instModule(handle<>
                        (borrowed(PyImport_AddModule("pvr.Prim.Inst"))));
      scope().attr("Inst") = instModule;
      scope instScope = instModule;

      // InstantiationPrim
      class_<Inst::InstantiationPrim, bases<Primitive>, 
        Inst::InstantiationPrim::Ptr, boost::noncopyable>
        ("InstantiationPrim", no_init)
        ;
      implicitly_convertible<Inst::InstantiationPrim::Ptr,
        Inst::InstantiationPrim::CPtr>();
      
      // Sphere
      class_<Inst::Sphere, bases<Inst::InstantiationPrim>,
        Inst::Sphere::Ptr>
        ("Sphere", no_init)
        .def("__init__", make_constructor(Inst::Sphere::create))
        ;
      implicitly_convertible<Inst::Sphere::Ptr,
        Inst::Sphere::CPtr>();
      
      // Line
      class_<Inst::Line, bases<Inst::InstantiationPrim>,
        Inst::Line::Ptr>
        ("Line", no_init)
        .def("__init__", make_constructor(Inst::Line::create))
        ;
      implicitly_convertible<Inst::Line::Ptr,
        Inst::Line::CPtr>();
      
      // Surface
      class_<Inst::Surface, bases<Inst::InstantiationPrim>,
        Inst::Surface::Ptr>
        ("Surface", no_init)
        .def("__init__", make_constructor(Inst::Surface::create))
        ;
      implicitly_convertible<Inst::Surface::Ptr,
        Inst::Surface::CPtr>();
      
    } // end inst scope
    
    // Rast scope
    {
      // Create Rast module
      object rastModule(handle<>
                        (borrowed(PyImport_AddModule("pvr.Prim.Rast"))));
      scope().attr("Rast") = rastModule;
      scope rastScope = rastModule;
      
      // RasterizationPrim
      class_<Rast::RasterizationPrim, bases<Primitive>, 
        Rast::RasterizationPrim::Ptr, boost::noncopyable>
        ("RasterizationPrim", no_init)
        ;
      implicitly_convertible<Rast::RasterizationPrim::Ptr,
        Rast::RasterizationPrim::CPtr>();
      
      // Point
      class_<Rast::Point, bases<Rast::RasterizationPrim>,
        Rast::Point::Ptr>
        ("Point", no_init)
        .def("__init__", make_constructor(Rast::Point::create))
        ;
      implicitly_convertible<Rast::Point::Ptr,
        Rast::Point::CPtr>();
      
      // PyroclasticPoint
      class_<Rast::PyroclasticPoint, bases<Rast::RasterizationPrim>,
        Rast::PyroclasticPoint::Ptr>
        ("PyroclasticPoint", no_init)
        .def("__init__", make_constructor(Rast::PyroclasticPoint::create))
        ;
      implicitly_convertible<Rast::PyroclasticPoint::Ptr,
        Rast::PyroclasticPoint::CPtr>();
      
      // Line
      class_<Rast::Line, bases<Rast::RasterizationPrim>,
        Rast::Line::Ptr>
        ("Line", no_init)
        .def("__init__", make_constructor(Rast::Line::create))
        ;
      implicitly_convertible<Rast::Line::Ptr,
        Rast::Line::CPtr>();
      
      // PyroclasticLine
      class_<Rast::PyroclasticLine, bases<Rast::RasterizationPrim>,
        Rast::PyroclasticLine::Ptr>
        ("PyroclasticLine", no_init)
        .def("__init__", make_constructor(Rast::PyroclasticLine::create))
        ;
      implicitly_convertible<Rast::PyroclasticLine::Ptr,
        Rast::PyroclasticLine::CPtr>();
      
    } // end rast scope

  } // end prim scope
    
}

//----------------------------------------------------------------------------//

