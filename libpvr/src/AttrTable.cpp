//----------------------------------------------------------------------------//

/*! \file AttrTable.cpp
  Contains implementations of AttrTable class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/AttrTable.h"

// System includes

#include <cassert>
#include <string>
#include <algorithm>

// Project includes

#include "pvr/StlUtil.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace boost;
  using namespace std;
  using namespace pvr;
  using namespace pvr::Geo;

  //--------------------------------------------------------------------------//

  template <typename AttrVec_T>
  void clearAndResize(AttrVec_T &attrArrays, const size_t size)
  {
    typedef typename AttrVec_T::value_type::value_type T;
    typedef typename AttrVec_T::iterator Iter;
    for (Iter attr = attrArrays.begin(); attr != attrArrays.end(); ++attr) {
      size_t arraySize = attr->arraySize();
      swapClear(attr->elems);
      try {
        attr->elems.resize(size * arraySize);
      }
      catch (bad_alloc &e) {
        throw AttrTable::AttrResizeException("");
      }
      const std::vector<T> &defaults = attr->defaults();
      for (size_t i = 0; i < size * arraySize; i++) {
        attr->elems[i] = defaults[i % arraySize];
      }
    }
  }

  //--------------------------------------------------------------------------//

  template <typename AttrVec_T>
  void addElemUtil(AttrVec_T &attrMap, const size_t size)
  {
    typedef typename AttrVec_T::value_type::value_type T;
    typedef typename AttrVec_T::iterator Iter;
    for (Iter attr = attrMap.begin(); attr != attrMap.end(); ++attr) {
      size_t arraySize = attr->arraySize();
      size_t oldSize = attr->elems.size();
      size_t newSize = oldSize + size * arraySize;
      try {
        attr->elems.resize(newSize);
      }
      catch (bad_alloc &e) {
        throw AttrTable::AttrResizeException("");
      }
      const std::vector<T> &defaults = attr->defaults();
      for (size_t i = oldSize; i < newSize; i++) {
        attr->elems[i] = defaults[i % arraySize];
      }
    }
  }

  //--------------------------------------------------------------------------//

  template <typename Attr_T>
  void checkForExistingAttr(const Attr_T &attrVec, const string &attrName)
  {
    typename Attr_T::const_iterator i = attrVec.begin();
    for (; i != attrVec.end(); ++i) {
      if (i->name() == attrName) {
        throw AttrTable::AttrExistsException(attrName);
      }
    }
  }

  //--------------------------------------------------------------------------//

  AttrTable::StringVec createInitialStringVec() 
  {
    AttrTable::StringVec vec;
    vec.push_back("");
    return vec;
  }

  //--------------------------------------------------------------------------//

  template <typename Vec_T>
  size_t findAttrIdx(const Vec_T &vec, const string &attrName)
  {
    for (size_t i = 0; i < vec.size(); ++i) {
      if (vec[i].name() == attrName) {
        return i;
      }
    }
    throw AttrTable::AttrNotFoundException(attrName);
  }

  //--------------------------------------------------------------------------//

  void checkRefValid(const AttrRef &ref)
  {
    if (!ref.isValid()) {
      throw AttrTable::AttrInvalidException(lexical_cast<string>(ref.idx()));
    }
  }

  //--------------------------------------------------------------------------//

  void checkElemRange(const size_t elem, const size_t size)
  {
    if (elem >= size) {
      throw AttrTable::ElemIdxException
        (lexical_cast<string>(elem) + 
         " (size is " + lexical_cast<string>(size) + ")");
    }
  }

  //--------------------------------------------------------------------------//

  void checkAttrSize(const size_t idx, const size_t size)
  {
    if (idx >= size) {
      throw AttrTable::ArrayIdxException
        (lexical_cast<string>(idx) + 
         " (size is " + lexical_cast<string>(size) + ")");
    }
  }

  //--------------------------------------------------------------------------//

  template <class T>
  T attrLookup(const vector<AttrArray<T> > &attrs, const AttrRef &ref, 
               const size_t elem, const size_t arrayIdx)
  {
    checkRefValid(ref);
    checkElemRange(elem, attrs[ref.idx()].elems.size());
    checkAttrSize(arrayIdx, attrs[ref.idx()].arraySize());
    return attrs[ref.idx()].elem(elem, arrayIdx);
  }

  //--------------------------------------------------------------------------//

  template <class T>
  void attrSet(const AttrRef &ref, const size_t elem, const size_t arrayIdx, 
               const T &value, vector<AttrArray<T> > &attrs)
  {
    checkRefValid(ref);
    checkElemRange(elem, attrs[ref.idx()].elems.size());
    checkAttrSize(arrayIdx, attrs[ref.idx()].arraySize());
    attrs[ref.idx()].setElem(elem, arrayIdx, value);
  }

  //--------------------------------------------------------------------------//

  template <typename T>
  vector<string> attrNames(const T &attrArray)
  {
    vector<string> ret;
    for (typename T::const_iterator i = attrArray.begin();
         i != attrArray.end(); ++i) {
      ret.push_back(i->name());
    }
    return ret;
  }

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
// AttrTable
//----------------------------------------------------------------------------//

AttrTable::AttrTable()
  : m_size(0)
{
  // Empty
}

//----------------------------------------------------------------------------//

AttrTable::Ptr AttrTable::create()
{
  return Ptr(new AttrTable);
}

//----------------------------------------------------------------------------//

void AttrTable::resize(const size_t size)
{
  // Clear existing attribute entries
  clearAndResize(m_intAttrs, size);
  clearAndResize(m_floatAttrs, size);
  clearAndResize(m_vectorAttrs, size);
  clearAndResize(m_stringIdxAttrs, size);
  // Update size
  m_size = size;
}

//----------------------------------------------------------------------------//

size_t AttrTable::size() const
{ 
  return m_size; 
}

//----------------------------------------------------------------------------//

void AttrTable::addElems(const size_t numItems)
{
  // Add elements to existing attributes
  addElemUtil(m_intAttrs, numItems);
  addElemUtil(m_floatAttrs, numItems);
  addElemUtil(m_vectorAttrs, numItems);
  addElemUtil(m_stringIdxAttrs, numItems);
  // Update size
  m_size += numItems;
}

//----------------------------------------------------------------------------//

std::vector<std::string> AttrTable::intAttrNames() const
{
  return attrNames(m_intAttrs);
}

//----------------------------------------------------------------------------//

std::vector<std::string> AttrTable::floatAttrNames() const
{
  return attrNames(m_floatAttrs);
}

//----------------------------------------------------------------------------//

std::vector<std::string> AttrTable::vectorAttrNames() const
{
  return attrNames(m_vectorAttrs);
}

//----------------------------------------------------------------------------//

std::vector<std::string> AttrTable::stringAttrNames() const
{
  return attrNames(m_stringIdxAttrs);
}

//----------------------------------------------------------------------------//

//! \todo Not exception safe.
AttrRef AttrTable::addIntAttr(const std::string &attrName, const size_t size,
                              const vector<int> &defaults)
{
  checkForExistingAttr(m_intAttrs, attrName);
  m_intAttrs.push_back(IntAttr(attrName, size, defaults, m_size));
  return intAttrRef(attrName);
}

//----------------------------------------------------------------------------//

//! \todo Not exception safe.
AttrRef AttrTable::addFloatAttr(const std::string &attrName, const size_t size,
                                const vector<float> &defaults)
{
  checkForExistingAttr(m_floatAttrs, attrName);
  m_floatAttrs.push_back(FloatAttr(attrName, size, defaults, m_size));
  return floatAttrRef(attrName);
}

//----------------------------------------------------------------------------//

//! \todo Not exception safe.
AttrRef AttrTable::addVectorAttr(const std::string &attrName, 
                                const Imath::V3f &defaults)
{
  checkForExistingAttr(m_vectorAttrs, attrName);
  vector<Imath::V3f> defaultVec;
  defaultVec.push_back(defaults);
  m_vectorAttrs.push_back(VectorAttr(attrName, 1, defaultVec, m_size));
  return vectorAttrRef(attrName);
}

//----------------------------------------------------------------------------//

//! \todo Not exception safe.
AttrRef AttrTable::addStringAttr(const std::string &attrName)
{
  checkForExistingAttr(m_stringIdxAttrs, attrName);
  // Add the index attribute
  vector<size_t> defaultVec;
  defaultVec.push_back(0);
  m_stringIdxAttrs.push_back(StringIdxAttr(attrName, 1, defaultVec, m_size));
  // Set up the string table
  StringVec initial = createInitialStringVec();
  m_stringTables.push_back(initial);
  // Return index of newly created attr
  return stringAttrRef(attrName);
}

//----------------------------------------------------------------------------//

AttrRef AttrTable::intAttrRef(const std::string &attrName) const
{
  try {
    size_t idx = findAttrIdx(m_intAttrs, attrName);
    return AttrRef(idx, m_intAttrs[idx].arraySize());
  }
  catch (AttrNotFoundException &e) {
    return AttrRef();
  }
}

//----------------------------------------------------------------------------//

AttrRef AttrTable::floatAttrRef(const std::string &attrName) const
{
  try {
    size_t idx = findAttrIdx(m_floatAttrs, attrName);
    return AttrRef(idx, m_floatAttrs[idx].arraySize());
  }
  catch (AttrNotFoundException &e) {
    return AttrRef();
  }
}

//----------------------------------------------------------------------------//

AttrRef AttrTable::vectorAttrRef(const std::string &attrName) const
{
  try {
    size_t idx = findAttrIdx(m_vectorAttrs, attrName);
    return AttrRef(idx, 1);
  }
  catch (AttrNotFoundException &e) {
    return AttrRef();
  }
}

//----------------------------------------------------------------------------//

AttrRef AttrTable::stringAttrRef(const std::string &attrName) const
{
  try {
    size_t idx = findAttrIdx(m_stringIdxAttrs, attrName);
    return AttrRef(idx, 1);
  }
  catch (AttrNotFoundException &e) {
    return AttrRef();
  }
}

//----------------------------------------------------------------------------//

int AttrTable::intAttr(const AttrRef &ref, const size_t elem, 
                       const size_t arrayIdx) const
{
  return attrLookup<int>(m_intAttrs, ref, elem, arrayIdx);
}

//----------------------------------------------------------------------------//

float AttrTable::floatAttr(const AttrRef &ref, const size_t elem, 
                           const size_t arrayIdx) const
{
  return attrLookup<float>(m_floatAttrs, ref, elem, arrayIdx);
}

//----------------------------------------------------------------------------//

Imath::V3f AttrTable::vectorAttr(const AttrRef &ref, const size_t elem) const
{
  return attrLookup<Imath::V3f>(m_vectorAttrs, ref, elem, 0);
}

//----------------------------------------------------------------------------//

size_t AttrTable::stringIdxAttr(const AttrRef &ref, const size_t elem) const
{
  return attrLookup<size_t>(m_stringIdxAttrs, ref, elem, 0);
}

//----------------------------------------------------------------------------//

std::vector<int>&
AttrTable::intAttrElems(const AttrRef &ref)
{
  checkRefValid(ref);
  return m_intAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

std::vector<float>&
AttrTable::floatAttrElems(const AttrRef &ref)
{
  checkRefValid(ref);
  return m_floatAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

std::vector<Imath::V3f>&
AttrTable::vectorAttrElems(const AttrRef &ref)
{
  checkRefValid(ref);
  return m_vectorAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

std::vector<size_t>&
AttrTable::stringIdxAttrElems(const AttrRef &ref)
{
  checkRefValid(ref);
  return m_stringIdxAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

const std::vector<int>&
AttrTable::intAttrElems(const AttrRef &ref) const
{
  checkRefValid(ref);
  return m_intAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

const std::vector<float>&
AttrTable::floatAttrElems(const AttrRef &ref) const
{
  checkRefValid(ref);
  return m_floatAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

const std::vector<Imath::V3f>&
AttrTable::vectorAttrElems(const AttrRef &ref) const
{
  checkRefValid(ref);
  return m_vectorAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

const std::vector<size_t>&
AttrTable::stringIdxAttrElems(const AttrRef &ref) const
{
  checkRefValid(ref);
  return m_stringIdxAttrs[ref.idx()].elems;
}
  
//----------------------------------------------------------------------------//

void AttrTable::setIntAttr(const AttrRef &ref, const size_t elem, 
                           const size_t arrayIdx, const int value)
{
  attrSet(ref, elem, arrayIdx, value, m_intAttrs);
}

//----------------------------------------------------------------------------//

void AttrTable::setFloatAttr(const AttrRef &ref, const size_t elem, 
                             const size_t arrayIdx, const float value)
{
  attrSet(ref, elem, arrayIdx, value, m_floatAttrs);
}

//----------------------------------------------------------------------------//

void AttrTable::setVectorAttr(const AttrRef &ref, const size_t elem, 
                              const Imath::V3f &value)
{
  attrSet(ref, elem, 0, value, m_vectorAttrs);
}
  
//----------------------------------------------------------------------------//
  
void AttrTable::setStringIdxAttr(const AttrRef &ref, const size_t elem, 
                                 const size_t value)
{
  attrSet(ref, elem, 0, value, m_stringIdxAttrs);
}

//----------------------------------------------------------------------------//

const std::string& 
AttrTable::stringAttr(const AttrRef &ref, const size_t elem) const
{
  size_t strIdx = attrLookup<size_t>(m_stringIdxAttrs, ref, elem, 0);
  return m_stringTables[ref.idx()][strIdx];
}

//----------------------------------------------------------------------------//

void 
AttrTable::setStringAttr(const AttrRef &ref, const size_t elem, 
                         const std::string &value)
{
  checkRefValid(ref);
  checkElemRange(elem, m_size);
  StringVec &strVec = m_stringTables[ref.idx()];
  StringVec::iterator i = find(strVec.begin(), strVec.end(), value);
  size_t idx = i - strVec.begin();
  if (i == strVec.end()) {
    strVec.push_back(value);
  }
  attrSet(ref, elem, 0, idx, m_stringIdxAttrs);
}

//----------------------------------------------------------------------------//

const std::string& 
AttrTable::stringFromTable(const AttrRef &ref, const size_t strIdx) const
{
  return m_stringTables[ref.idx()][strIdx];
}

//----------------------------------------------------------------------------//

size_t 
AttrTable::addStringToTable(const AttrRef &ref, const std::string &s)
{
  checkRefValid(ref);
  StringVec &strVec = m_stringTables[ref.idx()];
  StringVec::iterator i = find(strVec.begin(), strVec.end(), s);
  size_t idx = i - strVec.begin();
  if (i == strVec.end()) {
    strVec.push_back(s);
  }
  return idx;
}

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//
