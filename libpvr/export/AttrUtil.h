//-*-c++-*--------------------------------------------------------------------//

/*! \file AttrUtil.h
  Contains the Attr and AttrVisitor class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_ATTRUTIL_H__
#define __INCLUDED_PVR_ATTRUTIL_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/AttrTable.h"
#include "pvr/ParamMap.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// Attr<T>
//----------------------------------------------------------------------------//

/*! \class Attr
  \brief Used in conjunction with AttrVisitor to conveniently iterate over 
  points in an AttrTable.

  This class hides the work involved with aquiring an AttrRef, checking if
  it is valid, and then returning the attribute value. It also takes a global
  parameter map, whose values will be used if a point attribute is missing.
  Iteration code can therefor be written assuming that the attribute exists, 
  and in the case where the global param is used, no iteration or memory access
  occurs.

  \code
  AttrVisitor visitor(points, globalParams);
  Attr<float> density("density");
  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i) {
    i.update(density);
    // Do work with density value
  }

  \endcode
 */

//----------------------------------------------------------------------------//

template <typename T>
class Attr 
{
public:

  // Typedefs ------------------------------------------------------------------

  typedef typename std::vector<T>::const_iterator Iter;

  // Constructor ---------------------------------------------------------------

  Attr(const std::string &name)
    : m_pos(0), m_name(name), m_isActive(false), m_isInitialized(false)
  {
    m_defaultValue = static_cast<T>(0);
  }
  Attr(const std::string &name, const T &defaultValue)
    : m_pos(0), m_name(name), m_isActive(false), m_isInitialized(false)
  {
    m_defaultValue = defaultValue;
  }

  // Main methods --------------------------------------------------------------
  
  const std::string& name() const
  { return m_name; }

  void setActive(bool active) 
  {
    m_isActive = active;
    m_isInitialized = true;
  }

  bool isActive() const
  { return m_isActive; }

  bool isInitialized() const
  { return m_isInitialized; }

  void setIterator(const Iter &i)
  { m_value = i; }

  void setDefaultValue(const T defaultValue)
  { m_defaultValue = defaultValue; }

  void updateIndex(size_t newPos)
  {
    std::advance(m_value, (newPos - m_pos));
    m_pos = newPos;
  }

  const T& value() const
  { return m_isActive ? *(m_value) : m_defaultValue; }

  template <typename Other_T>
  Other_T as() const
  { return static_cast<Other_T>(m_isActive ? *(m_value) : m_defaultValue); }
  
  // Overloaded operators ------------------------------------------------------

  operator const T& () const
  { return m_isActive ? *(m_value) : m_defaultValue; }

private:

  // Private data members ------------------------------------------------------

  size_t      m_pos;
  Iter        m_value;
  std::string m_name;
  T           m_defaultValue;
  bool        m_isActive;
  bool        m_isInitialized;
};

//----------------------------------------------------------------------------//
// ArrayAttr<T>
//----------------------------------------------------------------------------//

/*! \class ArrayAttr
  \brief Used in conjunction with AttrVisitor to conveniently iterate over 
  points in an AttrTable that carry array attributes

  This class hides the work involved with aquiring an AttrRef, checking if
  it is valid, and then returning the attribute value. It also takes a global
  parameter map, whose values will be used if a point attribute is missing.
  Iteration code can therefor be written assuming that the attribute exists, 
  and in the case where the global param is used, no iteration or memory access
  occurs.

  \code
  AttrVisitor visitor(points, globalParams);
  Attr<float, 3> scale("scale");
  for (AttrVisitor::const_iterator i = visitor.begin(), end = visitor.end(); 
       i != end; ++i) {
    i.update(scale);
    // Do work with density value
  }

  \endcode
 */
//----------------------------------------------------------------------------//

template <typename T, size_t ArraySize_T>
class ArrayAttr 
{
public:

  // Typedefs ------------------------------------------------------------------

  typedef typename std::vector<T>::const_iterator Iter;

  // Constructor ---------------------------------------------------------------

  ArrayAttr(const std::string &name)
    : m_pos(0), m_name(name), m_isActive(false), m_isInitialized(false)
  {
    for (size_t i = 0; i < ArraySize_T; ++i) {
      m_defaultValue[i] = static_cast<T>(0);
    }
  }

  // Main methods --------------------------------------------------------------
  
  const std::string& name() const
  { return m_name; }

  void setActive(bool active) 
  {
    m_isActive = active;
    m_isInitialized = true;
  }

  bool isActive() const
  { return m_isActive; }

  bool isInitialized() const
  { return m_isInitialized; }

  void setIterator(const Iter &i)
  { m_value = i; }

  void setDefaultValue(const T defaultValue[ArraySize_T])
  {
    for (size_t i = 0; i < ArraySize_T; ++i) {
      m_defaultValue[i] = defaultValue[i];
    }
  }

  void updateIndex(size_t newPos)
  {
    newPos *= ArraySize_T;
    std::advance(m_value, (newPos - m_pos));
    m_pos = newPos;
  }

  // Overloaded operators ------------------------------------------------------

  const T& operator[] (size_t arrayIdx) const
  { return m_isActive ? *(m_value + arrayIdx) : m_defaultValue[arrayIdx]; }

private:

  // Private data members ------------------------------------------------------

  size_t      m_pos;
  Iter        m_value;
  std::string m_name;
  T           m_defaultValue[ArraySize_T];
  bool        m_isActive;
  bool        m_isInitialized;
};

//----------------------------------------------------------------------------//
// AttrVisitor
//----------------------------------------------------------------------------//

/*! \class AttrVisitor
  \brief Used to conveniently iterate over the points in an AttrTable.

  See code example in Attr
 */

//----------------------------------------------------------------------------//

class AttrVisitor
{
public:
  
  // Structs -------------------------------------------------------------------

  //! For iterating over the points in an AttrTable
  struct const_iterator
  {
    //! Constructor. Requires reference to visitor and initial position.
    const_iterator(const AttrVisitor &visitor, size_t pos)
      : m_pos(pos), m_visitor(visitor)
    { }
    //! Inequality operator. Compares current position only
    bool operator != (const const_iterator &rhs)
    { return m_pos != rhs.m_pos; }
    //! Prefix increment. Used to step iterator along
    const const_iterator& operator ++ ()
    {
      m_pos++;
      return *this;
    }
    //! Most important method - updates the value in a Attr by 
    //! incrementing its iterator to the given position.
    template <class T>
    void update(Attr<T> &attr) const
    {
      m_visitor.update(attr, m_pos);      
    }
    //! Most important method - updates the value in a Attr by 
    //! incrementing its iterator to the given position.
    template <class T, size_t ArraySize_T>
    void update(ArrayAttr<T, ArraySize_T> &attr) const
    {
      m_visitor.update(attr, m_pos);      
    }
    //! Returns the current position
    size_t index() const
    { 
      return m_pos; 
    }
  private:
    size_t             m_pos;
    const AttrVisitor& m_visitor;
  };

  // Constructor, destructor, factory ------------------------------------------

  //! Constructs the AttrVisitor given references to the 
  AttrVisitor(const AttrTable &points, const Util::ParamMap &globals);

  // Main methods --------------------------------------------------------------

  //! Returns const_iterator to beginning of point list
  const_iterator begin() const;
  //! Returns const_iterator to beginning of given index
  const_iterator begin(const size_t idx) const;
  //! Returns const_iterator to one step past last element of point list
  const_iterator end() const;
  //! Returns const_iterator to one step past given index
  const_iterator end(const size_t idx) const;

  //! Updates a Attr's value to that of the given point index's.
  template <class T>
  void update(Attr<T> &attr, size_t pos) const;

  //! Updates a Attr's value to that of the given point index's.
  template <class T, size_t ArraySize_T>
  void update(ArrayAttr<T, ArraySize_T> &attr, size_t pos) const;

private:

  // Private data members ------------------------------------------------------

  //! Reference to per-point attributes. If an attribute is requested and 
  //! exists here, that takes priority over the value in m_globals
  const AttrTable& m_points;
  //! Reference to global attribute values. These are used whenever a per-point
  //! attribute isn't found.
  const Util::ParamMap&  m_globals;

};

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

//! No implementation. Specializations only.
template <typename T>
void initialize(const AttrTable &points, const Util::ParamMap &globals,
                Attr<T> &attr);

//----------------------------------------------------------------------------//

//! No implementation. Specializations only.
template <typename T, size_t ArraySize_T>
void initialize(const AttrTable &points, const Util::ParamMap &globals,
                ArrayAttr<T, ArraySize_T> &attr);

//----------------------------------------------------------------------------//
// AttrVisitor - Implementations of templated methods
//----------------------------------------------------------------------------//

template <class T>
void AttrVisitor::update(Attr<T> &attr, size_t pos) const
{
  if (!attr.isInitialized()) {
    initialize(m_points, m_globals, attr);
  }
  if (attr.isActive()) {
    attr.updateIndex(pos);
  } else {
    // If the point attr isn't active it means the global has already been set
    // In this case we do nothing
  }
}
  
//----------------------------------------------------------------------------//

template <class T, size_t ArraySize_T>
void AttrVisitor::update(ArrayAttr<T, ArraySize_T> &attr, 
                              size_t pos) const
{
  if (!attr.isInitialized()) {
    initialize(m_points, m_globals, attr);
  }
  if (attr.isActive()) {
    attr.updateIndex(pos);
  } else {
    // If the point attr isn't active it means the global has already been set
    // In this case we do nothing
  }
}
  
//----------------------------------------------------------------------------//
// AttrVisitor - Template specializations
//----------------------------------------------------------------------------//

//! Partial specialization for <int>
template <size_t ArraySize_T>
void initialize(const AttrTable &points, const Util::ParamMap &globals,
                ArrayAttr<int, ArraySize_T> &attr)
{
  using namespace std;

  AttrRef ref = points.intAttrRef(attr.name());
  if (ref.isValid() && ref.arraySize() == ArraySize_T) {
    attr.setIterator(points.intAttrElems(ref).begin());
    attr.setActive(true);
  } else {
    int defaultValue[ArraySize_T];
    map<string, int>::const_iterator i = globals.intMap.find(attr.name());
    if (i != globals.intMap.end()) {
      defaultValue[0] = i->second;
      attr.setDefaultValue(defaultValue);
    } else {
      // No default value found in globals map. Revert to ArrayAttr's
      // own default.
    }
    attr.setActive(false);
  }
}

//----------------------------------------------------------------------------//

//! Partial specialization for <float>
template <size_t ArraySize_T>
void initialize(const AttrTable &points, const Util::ParamMap &globals,
                ArrayAttr<float, ArraySize_T> &attr)
{
  using namespace std;

  AttrRef ref = points.floatAttrRef(attr.name());
  if (ref.isValid() && ref.arraySize() == ArraySize_T) {
    attr.setIterator(points.floatAttrElems(ref).begin());
    attr.setActive(true);
  } else {
    float defaultValue[ArraySize_T];
    map<string, float>::const_iterator i = globals.floatMap.find(attr.name());
    if (i != globals.floatMap.end()) {
      defaultValue[0] = i->second;
      attr.setDefaultValue(defaultValue);
    } else {
      // No default value found in globals map. Revert to ArrayAttr's
      // own default.
    }
    attr.setActive(false);
  }
}

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr


//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
