//-*-c++-*--------------------------------------------------------------------//

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

#include "pvr/export.h"
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

  PVR_TYPEDEF_SMART_PTRS(PhaseFunction);

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

  PVR_TYPEDEF_SMART_PTRS(Composite);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC(Composite);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(Composite);

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

class LIBPVR_PUBLIC Isotropic : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Isotropic);

  // Factory -------------------------------------------------------------------

  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC(Isotropic);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(Isotropic);

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

};

//----------------------------------------------------------------------------//
// HenyeyGreenstein
//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC HenyeyGreenstein : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(HenyeyGreenstein);

  // Constructor, factory ------------------------------------------------------

  //! Default constructor, requires the eccentricity parameter
  HenyeyGreenstein(float g);
  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC_1_ARG(HenyeyGreenstein, float);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(HenyeyGreenstein);

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

private:

  // Private data members ------------------------------------------------------

  //! Eccentricity parameter
  const float m_g;

};

//----------------------------------------------------------------------------//
// DoubleHenyeyGreenstein
//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC DoubleHenyeyGreenstein : public PhaseFunction
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(DoubleHenyeyGreenstein);

  // Constructor, factory ------------------------------------------------------

  //! Default constructor, requires the eccentricity parameters and blend
  DoubleHenyeyGreenstein(float g1, float g2, float blend);
  //! Factory creation function. Always use this when creating objects
  //! that need lifespan management.
  PVR_DEFINE_CREATE_FUNC_3_ARG(DoubleHenyeyGreenstein, float, float, float);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(DoubleHenyeyGreenstein);

  // From PhaseFunction --------------------------------------------------------

  virtual float probability(const Vector &in, const Vector &out) const;

private:

  // Private data members ------------------------------------------------------

  //! Eccentricity parameter 1
  const float m_g1;
  //! Eccentricity parameter 2
  const float m_g2;
  //! Blend parameter
  const float m_blend;

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
