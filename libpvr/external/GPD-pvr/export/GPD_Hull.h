/*
 * Copyright (c) 2008
 *	Side Effects Software Inc.  All rights reserved.
 *
 * Redistribution and use of the GPD library in source and binary forms, with
 * or without modification, are permitted provided that the following
 * conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. The name of Side Effects Software may not be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SIDE EFFECTS SOFTWARE `AS IS' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL SIDE EFFECTS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *----------------------------------------------------------------------------
 */
/*
 * GPD_Hull.h
 *
 */


#ifndef __GPD_Hull_H__
#define __GPD_Hull_H__

#include "GPD_Primitive.h"
#include "GPD_Vertex.h"

enum GPD_SurfaceType {
	GPD_PATCH_ROWS		= 0,	// Just the rows
	GPD_PATCH_COLS		= 1,	// Just the columns
	GPD_PATCH_ROWCOL	= 2,	// Both rows and columns
	GPD_PATCH_TRIANGLE	= 3,	// Solid surface of triagles
	GPD_PATCH_QUADS		= 4,	// Solid surface of quads
	GPD_PATCH_ALTTRIANGLE	= 5,	// Triangles but common corners
	GPD_PATCH_REVTRIANGLE	= 6,	// Triangles but opposite direction
};

class	GPD_Point;
class	GPD_Detail;

class GPD_HullFlags {
public:
	GPD_HullFlags()
	{
		wrapu = 0;
		wrapv = 0;
	}
	unsigned	wrapu:1,
			wrapv:1;
};

class GPD_Hull : public GPD_Primitive {
public:
    GPD_Hull(GPD_Detail *d);
    virtual ~GPD_Hull();

    virtual unsigned	getPrimitiveId() const = 0;

    virtual int		save(ostream &os, int binary) const;
    virtual int		load(istream &is, int binary);

    unsigned		isWrappedU() const { return flags.wrapu; }
    unsigned		isWrappedV() const { return flags.wrapv; }

    int			numRows() const { return nRows; }
    int			numCols() const { return nCols; }
    GPD_SurfaceType	getSurfaceType() const { return surfaceType; }

    virtual void	wrapU();
    virtual void	openU();
    virtual void	wrapV();
    virtual void	openV();

    // Resizes the hull to the specified size.  Warning: does not
    // save any of the current vertex data.
    void		 setSize(int rows, int cols, int appendpt = 1);
    void		 setSurfaceType(GPD_SurfaceType type) { surfaceType = type; }
    GPD_Vertex		*getVertex(int row, int col);

protected:
    virtual int		savePrivate(ostream &os, int binary) const = 0;
    virtual int		loadPrivate(istream &is, int binary) = 0;

    virtual int		loadExtra(istream &is, int binary);
    virtual int 	saveExtra(ostream &is, int binary) const;

    GPD_Vertex               *vtxMatx;
    GPD_HullFlags		flags;
    GPD_SurfaceType		surfaceType;

    int				nRows, nCols;
};
#endif
