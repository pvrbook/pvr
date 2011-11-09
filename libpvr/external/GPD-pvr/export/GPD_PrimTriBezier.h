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
 * GPD_PrimTriBezier.h
 *
 */


#ifndef __GPD_PrimTriBezier__
#define __GPD_PrimTriBezier__

#include "GPD_Primitive.h"
#include "GPD_Vertex.h"
#include "GPD_PrimType.h"

class GPD_PrimTriBezier : public GPD_Primitive {
public:
    GPD_PrimTriBezier(GPD_Detail *d);
    virtual ~GPD_PrimTriBezier();

    virtual unsigned	getPrimitiveId() const { return GPDPRIMTRIBEZIER; }

    virtual int		save(ostream &os, int binary) const;
    virtual int		load(istream &is, int binary);

    // These convert a row & column into the correct offset into our
    // vertex list.
    unsigned		 getRawIndex(unsigned row, unsigned col) const
			    { return getRawIndex(row, col, myOrder); }
    unsigned		 getRawIndex(unsigned row, unsigned col, int order)const
			    { return (order - row)*(order-row-1)/2 + col; }
protected:
    virtual int		savePrivate(ostream &os, int binary) const { return 0; }
    virtual int		loadPrivate(istream &is, int binary) { return 0; }

    GPD_Vertex               *vtxList;
    int				nVtx;
    int				myOrder;
};
#endif

