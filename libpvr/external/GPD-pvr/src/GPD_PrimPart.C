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
 * GPD_PrimPart.C
 *
 */


#include "GPD_PrimPart.h"
#include "GPD_PrimType.h"
#include "GPD_Detail.h"

GPD_PrimParticle::GPD_PrimParticle(GPD_Detail *d) : GPD_Primitive(d) {}

GPD_PrimParticle::~GPD_PrimParticle()
{
    delete[] vtxList;
}

int
GPD_PrimParticle::save(ostream &os, int binary) const
{
    if (binary)
    {
	if (!UTwrite(os, &nVtx)) return -1;
    }
    else
    {
	if (!(os << " " << nVtx)) return -1;
    }
    for (int i=0; i < nVtx; i++)
	if (getParent()->saveVertex(os, vtxList[i]) < 0) return -1;
    
    if (getParent()->primitiveAttribs().save(os, attribData, binary, '[') < 0)
	return -1;
    
    if (!binary) os << endl;
    return (!os) ? -1 : 0;

}

int
GPD_PrimParticle::load(istream &is, int binary)
{
    if (binary)
    {
	if (!UTread(is, &nVtx)) return -1;
    }
    else
    {
	if (!(is >> nVtx)) return -1;
    }

    vtxList = new GPD_Vertex[nVtx];
    for (int i=0; i < nVtx; i++)
	{
	    vtxList[i].initAttributeData(getParent());
	    if (getParent()->loadVertex(is, vtxList[i]) < 0 ) return -1;
	}
    if (getParent()->primitiveAttribs().load(is, attribData, binary) < 0)
	return -1;

    if (!binary) is.ignore(GPD_BUFSIZ-1, '\n'); //get junk at end of line
    return (!is) ? -1 : 0;
}

unsigned
GPD_PrimParticle::getPrimitiveId() const
{
    return GPDPRIMPART;
}

