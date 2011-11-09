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
 * GPD_Vertex.h
 *
 *	The vertex class. As opposed to GPD_Points, all vertices are unique.
 *
 */


#ifndef __GPD_Vertex_H__
#define __GPD_Vertex_H__

#include "GPD_Attribute.h"

class GPD_Point;
class GPD_Detail;

class GPD_Vertex {
public:
     GPD_Vertex(GPD_Point *geoPt = 0) : attribData() { ppt = geoPt; }
     GPD_Vertex(GPD_Detail *d, GPD_Point *geoPt = 0)
     {
	ppt = geoPt;
	initAttributeData(d);
     }

    // Avoid using this c-tor because it's dangerously shallow!
     GPD_Vertex(const GPD_Vertex &vtx)	{ ppt = vtx.ppt; } // shallow!
    ~GPD_Vertex();

    void		 initAttributeData(GPD_Detail *d);
    void		 setPt(GPD_Point *pt) { ppt = pt; } 
    GPD_Point		*getPt(void) const { return ppt; }
    void		 copyAttributeData(const GPD_Vertex *src, int size);

    void		*getAttribData(int n) const { return attribData[n]; }

    GPD_AttributeData	 attribData;

private:
    GPD_Point		*ppt;
};

#endif
