//-*-c++-*--------------------------------------------------------------------//

/*! \file AttrTable.h
  Contains the AttrTable class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_ATTRIBUTE_TABLE_H__
#define __INCLUDED_PVR_ATTRIBUTE_TABLE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <map>
#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>

// Project headers

#include "pvr/Exception.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Geo {

//----------------------------------------------------------------------------//
// AttrArray
//----------------------------------------------------------------------------//

/*! \brief Container for n values of a single attribute 
  \note This class is not exposed through python.
  \internal
  The template parameter T determines data stored. 
 */

//----------------------------------------------------------------------------//

template <typename T>
class AttrArray 
{
public:

  // Typedefs ------------------------------------------------------------------

  //! Make template parameter available
  typedef T value_type;

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(DefaultsException, "Defaults vector size mismatch:");

  // Constructors --------------------------------------------------------------

  //! Constructur requires parameters for fully defining the attribute
  AttrArray(const std::string &name, const size_t arraySize, 
            const std::vector<T> &defaults, const size_t size);

  // Main methods --------------------------------------------------------------

  //! Returns the name of the attribute 
  const std::string& name() const
  { return m_name; }

  //! Returns the array size, i.e. the number of attribute values per item
  size_t arraySize() const 
  { return m_arraySize; }

  //! Returns the default value for the attribute. 
  const std::vector<T>& defaults() const
  { return m_defaults; }

  //! Returns the value of the given element
  const T& elem(const size_t idx, const size_t arrayIdx) const
  { return elems[idx * m_arraySize + arrayIdx]; }

  //! Sets the value of a given element
  void setElem(const size_t idx, const size_t arrayIdx, const T& value)
  { elems[idx * m_arraySize + arrayIdx] = value; }

  // Public data members -------------------------------------------------------

  std::vector<T> elems;

private:

  // Data members --------------------------------------------------------------
  
  //! Name of attribute represented
  std::string    m_name;
  //! Number of attribute values per item
  size_t         m_arraySize;
  //! Default value for attribute. This is used to initialize attribute value
  //! when adding new points to the collection.
  std::vector<T> m_defaults;
};

//----------------------------------------------------------------------------//
// AttrRef
//----------------------------------------------------------------------------//

/*! \brief Holds reference information about a given attribute.
  The user must check isValid() before accessing attributes in an AttrTable
  using the AttrRef object.
 */

//----------------------------------------------------------------------------//

class AttrRef
{
public:

  // Constructors --------------------------------------------------------------

  //! A default AttrRef is "invalid"
  AttrRef()
    : m_isValid(false), m_idx(0)
  { /* Empty */  }
  //! Constructs a "valid" AttrRef
  AttrRef(size_t idx, size_t arraySize)
    : m_isValid(true), m_idx(idx), m_arraySize(arraySize)
  { /* Empty */  }

  // Main methods --------------------------------------------------------------

  //! Checks whether the reference is valid. This is set to false by
  //! methods in AttrTable that return values of this type.
  bool isValid() const 
  { return m_isValid; }

  //! Returns the index referenced by this AttrRef instance. 
  size_t idx() const 
  { return m_idx; }

  //! Returns the array size of the attribute referenced.
  size_t arraySize() const
  { return m_arraySize; }

private:

  // Data members --------------------------------------------------------------
  
  //! Whether the reference is valid. An invalid reference is returned
  //! when requesting a non-existant attribute, for example.
  bool   m_isValid;
  //! Index of attribute referenced.
  size_t m_idx;
  //! Array size (Attribute values per item) of attribute referenced.
  size_t m_arraySize;

};

//----------------------------------------------------------------------------//
// AttrTable
//----------------------------------------------------------------------------//

/*! \brief Holds a collection of points along with their attributes.

  The class represents an MxN table where each column N represents an 'attribute'
  and each row M is an element in the table. Used to represent point, primitive,
  and object attributes in classes such as Particles and Triangles.
  
  Attributes are stored internally as a 'structure of vectors', meaning that
  new attributes can be added without disturbing existing data. It also means
  that the attributes for a given element are stored in different memory areas.
 */

//----------------------------------------------------------------------------//

class AttrTable
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(AttrTable);

  typedef std::vector<std::string>           StringVec;
  typedef std::vector<StringVec>             StringTableVec;

  typedef std::vector<int>                   IntVec;
  typedef std::vector<float>                 FloatVec;
  typedef std::vector<Imath::V3f>            VectorVec;
  typedef std::vector<size_t>                StringIdxVec;

  typedef AttrArray<int>                     IntAttr;
  typedef AttrArray<float>                   FloatAttr;
  typedef AttrArray<Imath::V3f>              VectorAttr;
  typedef AttrArray<size_t>                  StringIdxAttr;

  typedef std::vector<IntAttr>               IntAttrVec;
  typedef std::vector<FloatAttr>             FloatAttrVec;
  typedef std::vector<VectorAttr>            VectorAttrVec;
  typedef std::vector<StringIdxAttr>         StringIdxAttrVec;

  // Exceptions ----------------------------------------------------------------

  DECLARE_PVR_RT_EXC(AttrExistsException,   "Attribute exists:");  
  DECLARE_PVR_RT_EXC(AttrIdxException,      "Invalid index:");  
  DECLARE_PVR_RT_EXC(AttrResizeException,   "Couldn't resize.");  
  DECLARE_PVR_RT_EXC(AttrNotFoundException, "Attribute not found:");
  DECLARE_PVR_RT_EXC(AttrInvalidException,  "Invalid AttrRef used:");
  DECLARE_PVR_RT_EXC(ElemIdxException,      "Invalid element index:");
  DECLARE_PVR_RT_EXC(ArrayIdxException,     "Invalid array index:");

  // Constructor, destructor, factory ------------------------------------------

  //! Initializes an empty attribute table
  AttrTable();

  //! Factory function. Use this whenever the life span of the object needs
  //! to be managed.
  static Ptr create();

  // Main methods --------------------------------------------------------------

  //! Adds elements to the table. Items are inserted at the back of the 
  //! table.
  void   addElems(const size_t numItems);
  //! Resizes the entire attribute table. Will destroy any existing data but
  //! will retain attribute definitions.
  void   resize  (const size_t size);
  //! Returns size of table
  size_t size    () const;

  // Attribute names -----------------------------------------------------------

  /*! \{
    \name Attribute names
  */

  //! Returns the names of the int attributes
  StringVec intAttrNames   () const;
  //! Returns the names of the float attributes
  StringVec floatAttrNames () const;
  //! Returns the names of the vector attributes
  StringVec vectorAttrNames() const;
  //! Returns the names of the string attributes
  StringVec stringAttrNames() const;

  //! \}

  // Adding attributes ---------------------------------------------------------

  /*! \{
    \name Adding attributes
  */

  //! Adds attribute to AttrTable
  //! \param attrName Name of attribute to add
  //! \param size     Array size for attribute element
  //! \param defaults Vector of default values. Must be same length as size
  //! \returns        Reference to the created attribute
  //! \throws         AddAttrException if adding fails
  AttrRef addIntAttr   (const std::string &attrName, const size_t size,
                        const std::vector<int> &defaults);
  AttrRef addFloatAttr (const std::string &attrName, const size_t size,
                        const std::vector<float> &defaults);
  //! Add attribute to AttrTable
  //! \param attrName Name of attribute to add
  //! \returns        The index of the newly created attribute
  //! \throws         AddAttrException if adding fails
  AttrRef addVectorAttr(const std::string &attrName, 
                        const Imath::V3f &defaults);
  //! Add attribute to AttrTable
  //! \param attrName Name of attribute to add
  //! \returns        The index of the newly created attribute
  //! \throws         AddAttrException if adding fails
  AttrRef addStringAttr(const std::string &attrName);

  //! \}

  // Attribute references ------------------------------------------------------

  /*! \{
    \name Attribute references
  */

  //! Returns a reference to an attribute
  //! \param attrName Name of attribute to find
  //! \returns An 'invalid' AttrRef if the attribute couldn't be found.
  AttrRef intAttrRef   (const std::string &attrName) const;
  AttrRef floatAttrRef (const std::string &attrName) const;
  AttrRef vectorAttrRef(const std::string &attrName) const;
  AttrRef stringAttrRef(const std::string &attrName) const;

  //! \}

  // Attribute read access -----------------------------------------------------

  /*! \{
    \name Access to attribute values (by-value)
  */
  
  //! Read access to int attributes
  int        intAttr   (const AttrRef &ref, const size_t elem, 
                        size_t arrayIdx) const;
  //! Read access to float attributes
  float      floatAttr (const AttrRef &ref, const size_t elem, 
                        size_t arrayIdx) const;
  //! Read access to vector attributes
  Imath::V3f vectorAttr(const AttrRef &ref, const size_t elem) const;

  //! \}

  // Attribute write access ----------------------------------------------------

  /*! \{
    \name Write access to attributes
  */

  //! Sets the int attribute value of the given element
  void setIntAttr   (const AttrRef &ref, const size_t elem, 
                     const size_t arrayIdx, const int value);
  //! Sets the float attribute value of the given element
  void setFloatAttr (const AttrRef &ref, const size_t elem, 
                     const size_t arrayIdx, const float value);
  //! Sets the vector attribute value of the given element
  void setVectorAttr(const AttrRef &ref, const size_t elem, 
                     const Imath::V3f &value);

  //! \}

  // String attribute access ---------------------------------------------------

  /*! \{
    \name String attribute methods
  */

  //! Read access to string index attributes.
  //! \note This only returns the string -index-. To get the actual string,
  //! pass the index to the stringFromTable() method.
  size_t             stringIdxAttr   (const AttrRef &ref, 
                                      const size_t elem) const;
  //! Sets the string attribute value of the given element
  void               setStringIdxAttr(const AttrRef &ref, 
                                      const size_t elem, 
                                      const size_t value);
  //! Shortcut for getting a string directly from an element. 
  //! \warning Please note that this is slow if called repeatedly for each 
  //! element.
  const std::string& stringAttr      (const AttrRef &ref, 
                                      const size_t elem) const;
  //! Shortcut for setting a string directly. Slow if used repeatedly.
  void               setStringAttr   (const AttrRef &ref, 
                                      const size_t elem, 
                                      const std::string &value);
  //! Retrieves a string from the string table.
  //! \param ref Reference to the attribute. Use stringAttr() to get a 
  //! reference.
  //! \param strIdx String index to fetch. Use stringIdxAttr() to get the
  //! string index.
  const std::string& stringFromTable (const AttrRef &ref, 
                                      const size_t strIdx) const;
  //! Write access to string table
  //! \returns Index of string added
  size_t             addStringToTable(const AttrRef &ref, 
                                      const std::string &s);

  //! \}

  // ---------------------------------------------------------------------------

  /*! \{
    \name Access to attribute vectors (read/write)
  */

  //! Read/write access to the entire attribute vector. 
  IntVec&                 intAttrElems        (const AttrRef &ref);
  FloatVec&               floatAttrElems      (const AttrRef &ref);
  VectorVec&              vectorAttrElems     (const AttrRef &ref);
  StringIdxVec&           stringIdxAttrElems  (const AttrRef &ref);

  //! Read access to the entire attribute vector. 
  const IntVec&           intAttrElems        (const AttrRef &ref) const;
  const FloatVec&         floatAttrElems      (const AttrRef &ref) const;
  const VectorVec&        vectorAttrElems     (const AttrRef &ref) const;
  const StringIdxVec&     stringIdxAttrElems  (const AttrRef &ref) const;

  //! \}

private:

  // Private data members ------------------------------------------------------

  //! Store the size here, even though it's implicit in the attributes.
  //! We need to cover the case where the table is resized before adding
  //! any attributes.
  size_t m_size;

  //! No attribute name is stored with the string table, so we rely on
  //! the indices matching those in m_stringIdxAttrs
  StringTableVec m_stringTables;

  IntAttrVec m_intAttrs;
  FloatAttrVec m_floatAttrs;
  VectorAttrVec m_vectorAttrs;
  StringIdxAttrVec m_stringIdxAttrs;

};

//----------------------------------------------------------------------------//
// AttrArray
//----------------------------------------------------------------------------//

template <typename T>   
AttrArray<T>::AttrArray(const std::string &name, const size_t arraySize, 
                        const std::vector<T> &defaults, const size_t size)
  : m_name(name), m_arraySize(arraySize), m_defaults(defaults)
{
  using namespace boost;

  if (defaults.size() != arraySize) {
    std::string error = 
      lexical_cast<std::string>(arraySize) + " does not match " + 
      lexical_cast<std::string>(defaults.size());
    throw DefaultsException(error);
  }

  elems.resize(size * arraySize);
                                                        
  // Write default value
  for (size_t i = 0; i < size * arraySize; i++) {
    elems[i] = defaults[i % arraySize];
  }
}

//----------------------------------------------------------------------------//

} // namespace Geo
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
