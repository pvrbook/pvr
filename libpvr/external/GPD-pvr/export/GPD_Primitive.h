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
 * GPD_Primitive.h
 *
 */


#ifndef __GEO_Primitive_H__
#define __GEO_Primitive_H__

#include "GPD_Attribute.h"
#include <iostream.h>

#include "GPD_Point.h"
class	GPD_Detail;
class	GPD_Vertex;
class	GPD_VertexAttribDict;

class GPD_Primitive {
public:
    GPD_Primitive(GPD_Detail *d) : attribData()
    {
	parent = d;
    }
    virtual ~GPD_Primitive();

    int			 getNum() const { return num; }
    virtual unsigned	 getPrimitiveId() const = 0;
    void	 	 copyAttributeData(const GPD_Primitive *src);

    virtual int		 save(ostream &os, int binary) const = 0;
    virtual int		 load(istream &is, int binary) = 0;

    void		*getAttribData(int n) const { return attribData.getData(n); }

    GPD_Detail	*getParent() const	{ return parent; }
    static unsigned	 getPrimitiveType(const char *name);
    static const char   *getPrimitiveName(unsigned type);

protected:
    // Methods for finding out about point references
    int			 num;	// The primitive number (kept by detail)
    GPD_Detail		*parent;	// Who I belong to
    GPD_AttributeData	 attribData;	// Data for attributes of the primitive

private:
    friend class	 GPD_Detail;	// Allow detail to get at private stuff
};
#endif
