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

/*! \file export.h
  macros that are necessary for proper symbol export when doing multi-platform development.
 */



#ifndef LIBPVR_EXPORT_H
#define LIBPVR_EXPORT_H

#ifndef LIBPVR_EXPORT
#  if defined(_MSC_VER) && defined(WIN32)
#    define LIBPVR_EXPORT __declspec(dllexport)
#  else
#    define LIBPVR_EXPORT
#  endif
#endif

#ifndef  LIBPVR_PUBLIC
#  if defined(_MSC_VER) && defined(WIN32)
#    ifdef LIBPVR_EXPORTS
#      define LIBPVR_PUBLIC __declspec(dllexport)
#    else
#      define LIBPVR_PUBLIC __declspec(dllimport)
#    endif
#  else
#    define LIBPVR_PUBLIC
#  endif
#endif

#endif // LIBPVR_EXPORT_H
