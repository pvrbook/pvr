//-*-c++-*--------------------------------------------------------------------//

/*! \file ClassFactory.h
  Contains the ClassFactory class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_CLASSFACTORY_H__
#define __INCLUDED_PVR_CLASSFACTORY_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes

#include <map>

// Library includes

// Project includes

#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Sys {

//----------------------------------------------------------------------------//
// ClassFactory
//----------------------------------------------------------------------------//

/*! \class ClassFactory
  \brief Template for class factories.

  The template parameter is the base class of what type of objects the 
  factory will store.
 */

//----------------------------------------------------------------------------//

template <typename Class_T>
class ClassFactory
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(ClassFactory);

  //! Convenience typedef 
  typedef typename Class_T::Ptr ClassPtr;
  //! Function pointer for create functions
  typedef ClassPtr (*CreateFnPtr) ();

  // Main methods --------------------------------------------------------------

  //! Registers the given class by providing a function for creating an 
  //! instance. The class name is found by instantiating a single object before
  //! adding the function to the class factory.
  void registerClass(CreateFnPtr createFn);
  //! Returns an instance of the given type name
  //! \returns Null ptr if type name couldn't be found
  ClassPtr create(const std::string &typeName) const;

  // Access to singleton instance ----------------------------------------------

  //! Returns a pointer to the singleton instance.
  static Ptr singleton();

private:

  // Private constructor -------------------------------------------------------

  //! Prevents instantiation of the class. Only way to access ClassFactory
  //! is through the singleton() static method.
  ClassFactory()
  { }
  
  // Private typedefs ----------------------------------------------------------

  //! Maps a given class name to a create() function
  typedef std::map<std::string, CreateFnPtr> ClassMap;

  // Data members --------------------------------------------------------------

  //! Stores a map of string->creation function
  ClassMap m_classes;

  //! Pointer to singleton instance.
  static Ptr ms_instance;

};
                                         
//----------------------------------------------------------------------------//

} // namespace Sys
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
