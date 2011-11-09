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
 * GPD_2DVertext.h
 *
 *	The vertex class. As opposed to GPD_2DPoints, all vertices are unique.
 *
 */

#ifndef __GPD_2DVertex_H__
#define __GPD_2DVertex_H__

class GPD_2DPoint;

class GPD_2DVertex {
public:
     GPD_2DVertex(GPD_2DPoint *pt = 0)	{ ppt = pt; }

    // Avoid using this c-tor because it's dangerously shallow!
     GPD_2DVertex(const GPD_2DVertex &vtx)	{ ppt = vtx.ppt; } // shallow!
    ~GPD_2DVertex() {}

    void		 setPt(GPD_2DPoint *pt) { ppt = pt; } 
    GPD_2DPoint		*getPt(void) const      { return ppt; }

private:
    GPD_2DPoint		*ppt;
};

#endif
