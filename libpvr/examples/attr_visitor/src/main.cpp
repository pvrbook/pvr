//-*-c++-*--------------------------------------------------------------------//

/* This example shows some use of the AttrTable and related classes.
 * 
 * 
 *
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include <iostream>

#include <boost/timer.hpp>

#include <pvr/AttrUtil.h>
#include <pvr/ParamMap.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace Imath;
using namespace pvr::Geo;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

int main()
{
  size_t numItems = 10000000;

  AttrTable at;
  at.addVectorAttr("P", V3f(0.0));
  at.addFloatAttr("float_attr", 1, std::vector<float>(1, 0.0));
  at.addIntAttr("int_attr", 1, std::vector<int>(1, 0));
  at.resize(numItems);
  
  // First we iterate explicitly over all points
  
  {
    boost::timer timer;
    
    AttrRef pRef = at.vectorAttrRef("P");
    AttrRef floatRef = at.floatAttrRef("float_attr");
    AttrRef intRef = at.intAttrRef("int_attr");

    V3f pVal(0.0);
    float floatVal(0.0);
    int intVal(0);

    for (size_t i = 0, end = at.size(); i != end; ++i) {
      pVal += at.vectorAttr(pRef, i);
      floatVal += at.floatAttr(floatRef, i, 0);
      intVal += at.intAttr(intRef, i, 0);
    }

    cout << "Time to iterate explicitly: " << timer.elapsed() << endl;

  }

  // Then, we introduce a global attribute map
  
  ParamMap params;
  params.intMap["int_attr"] = 1;
  params.floatMap["float_attr"] = 1.0;
  params.intMap["int_attr_2"] = 1;
  params.floatMap["float_attr_2"] = 1.0;

  {
    boost::timer timer;

    AttrRef pRef = at.vectorAttrRef("P");
    AttrRef floatRef = at.floatAttrRef("float_attr");
    AttrRef intRef = at.intAttrRef("int_attr");
    AttrRef floatRef2 = at.floatAttrRef("float_attr_2");
    AttrRef intRef2 = at.intAttrRef("int_attr_2");

    V3f pVal(0.0);
    float floatVal(0.0), floatVal2(0.0);
    int intVal(0), intVal2(0);

    if (pRef.isValid()) {
        for (size_t i = 0, end = at.size(); i != end; ++i) {
          pVal += at.vectorAttr(pRef, i);
        }
    } else {
      pVal += params.vectorMap["P"] * at.size();
    }
    if (floatRef.isValid()) {
      for (size_t i = 0, end = at.size(); i != end; ++i) {
        floatVal += at.floatAttr(floatRef, i, 0);
      } 
    } else {
      floatVal += params.floatMap["float_attr"] * at.size();
    }
    if (intRef.isValid()) {
      for (size_t i = 0, end = at.size(); i != end; ++i) {
        intVal += at.intAttr(intRef, i, 0);
      }
    } else {
      intVal += params.intMap["int_attr"] * at.size();
    }
    if (floatRef2.isValid()) {
      for (size_t i = 0, end = at.size(); i != end; ++i) {
        floatVal2 += at.floatAttr(floatRef2, i, 0);
      }
    } else {
      floatVal2 += params.floatMap["float_attr_2"] * at.size();
    }
    if (intRef2.isValid()) {
      for (size_t i = 0, end = at.size(); i != end; ++i) {
        intVal2 += at.intAttr(intRef2, i, 0);
      }
    } else {
      intVal2 += params.intMap["int_attr_2"] * at.size();
    }

    cout << "Time to iterate explicitly with param map: " 
         << timer.elapsed() << endl;
    cout << pVal << " " << floatVal << " " << floatVal2 << " "
         << intVal << " " << intVal2 << endl;
    
  }

  // Lastly, we iterate using AttrVisitor

  {

    boost::timer timer;

    AttrVisitor visitor(at, params);
    Attr<V3f> p("P");
    Attr<float> float1("float_attr");
    Attr<float> float2("float_attr_2");
    Attr<int> int1("int_attr");
    Attr<int> int2("int_attr_2");

    V3f pVal(0.0);
    float floatVal(0.0), floatVal2(0.0);
    int intVal(0), intVal2(0);

    for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end();
         i != end; ++i) {
      i.update(p);
      i.update(float1);
      i.update(float2);
      i.update(int1);
      i.update(int2);
      pVal += p.value();
      floatVal += float1.value();
      floatVal2 += float2.value();
      intVal += int1.value();
      intVal2 += int2.value();
    }

    cout << "Time to iterate using AttrVisitor: " << timer.elapsed() << endl;
    cout << pVal << " " << floatVal << " " << floatVal2 << " "
         << intVal << " " << intVal2 << endl;
    
  }

}

//----------------------------------------------------------------------------//
