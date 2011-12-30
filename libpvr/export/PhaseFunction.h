//-*-c++-*--------------------------------------------------------------------//

/*! \file PhaseFunction.h
  Contains the PhaseFunction base class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_PHASEFUNCTION_H__
#define __INCLUDED_PVR_PHASEFUNCTION_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

#include <boost/shared_ptr.hpp>

// Project headers

#include "pvr/ParamBase.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {
namespace Phase {

//----------------------------------------------------------------------------//
// PhaseFunction
//----------------------------------------------------------------------------//

//! Base class for phase functions, used by Volume classes to determine
//! scattering probability.

class PhaseFunction : public Util::ParamBase
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(PhaseFunction);

  // To be implemented by subclasses -------------------------------------------

  //! Returns the scattering probability given two normalized vectors
  virtual float probability(const Vector &in, const Vector &out) const = 0;

};

//----------------------------------------------------------------------------//
// Composite
//----------------------------------------------------------------------------//

class Composite : public PhaseFunction
{
public:

  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Composite);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  DECLARE_CREATE_FUNC(Composite);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "Composite"; }

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const; 

  // Main methods --------------------------------------------------------------

  //! Adds a phase function to the composite
  void add(PhaseFunction::CPtr phaseFunction);
  //! Sets the weight of one phase function
  void setWeight(const size_t idx, const float weight);

private:

  // Private data members ------------------------------------------------------

  //! Array of phase functions to be composited
  std::vector<PhaseFunction::CPtr> m_functions;
  //! Array of phase function weights
  std::vector<float>               m_weights;

};

//----------------------------------------------------------------------------//
// Isotropic
//----------------------------------------------------------------------------//

class Isotropic : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Isotropic);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  DECLARE_CREATE_FUNC(Isotropic);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "Isotropic"; }

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

};

//----------------------------------------------------------------------------//
// HenyeyGreenstein
//----------------------------------------------------------------------------//

class HenyeyGreenstein : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(HenyeyGreenstein);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  DECLARE_CREATE_FUNC(HenyeyGreenstein);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "HenyeyGreenstein"; }

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

};

//----------------------------------------------------------------------------//
// Schlick
//----------------------------------------------------------------------------//

class Schlick : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Schlick);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  DECLARE_CREATE_FUNC(Schlick);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "Schlick"; }

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

};

//----------------------------------------------------------------------------//
// Rayleigh
//----------------------------------------------------------------------------//

class Rayleigh : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Rayleigh);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  DECLARE_CREATE_FUNC(Rayleigh);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "Rayleigh"; }

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

};

//----------------------------------------------------------------------------//
// Mie
//----------------------------------------------------------------------------//

class Mie : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  DECLARE_SMART_PTRS(Mie);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  DECLARE_CREATE_FUNC(Mie);

  // From ParamBase ------------------------------------------------------------

  virtual std::string typeName() const
  { return "Mie"; }

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

};

//----------------------------------------------------------------------------//
// Constants
//----------------------------------------------------------------------------//

static const float k_isotropic = 1.0 / (4.0 * M_PI);

//----------------------------------------------------------------------------//

} // namespace Phase
} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
