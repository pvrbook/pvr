//-*-c++-*--------------------------------------------------------------------//

/* Copyright (c) 2011 Magnus Wrenninge
 *
 * All rights reserved.
 * For educational use only. All commercial use expressly forbidden.
 *
 * Redistributions of source code must retain the above copyright
 * notice, the list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//----------------------------------------------------------------------------//

/*! \file Line.h
  Contains the Rast::Line class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_RASTERIZATION_LINE_H__
#define __INCLUDED_PVR_RASTERIZATION_LINE_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System headers

// Library headers

// Project headers

#include "pvr/export.h"
#include "pvr/Primitives/Rasterization/LineBase.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Model {
namespace Prim {
namespace Rast {

//----------------------------------------------------------------------------//
// Line
//----------------------------------------------------------------------------//

/*! \class Line
  \brief Implements the rasterization line primitive described in chapter XX.

  See chapter XX for details.
*/

//----------------------------------------------------------------------------//

class LIBPVR_PUBLIC Line : public LineBase
{
public:
  
  // Typedefs ------------------------------------------------------------------

  PVR_TYPEDEF_SMART_PTRS(Line);

  // Factory -------------------------------------------------------------------

  PVR_DEFINE_CREATE_FUNC(Line);

  // From ParamBase ------------------------------------------------------------

  PVR_DEFINE_TYPENAME(Line);

protected:

  // From RasterizationPrimitive -----------------------------------------------

  virtual void getSample(const RasterizationState &state,
                         RasterizationSample &sample) const;

};

//----------------------------------------------------------------------------//

} // namespace Rast
} // namespace Prim
} // namespace Model
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard

//----------------------------------------------------------------------------//
