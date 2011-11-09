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
 * GPD_PrimNURBSurf.C
 *
 */


#include "GPD_PrimNURBSurf.h"
#include "GPD_PrimType.h"
#include "GPD_Detail.h"

#define GPD_ENDS	"end"
#define GPD_NOENDS	"noend"

GPD_PrimNURBSurf::GPD_PrimNURBSurf(GPD_Detail *d) : GPD_TPSurf(d)
{
    uEndCondition = 0;
    vEndCondition = 1;
}

GPD_PrimNURBSurf::~GPD_PrimNURBSurf()
{
}

int
GPD_PrimNURBSurf::savePrivate(ostream &os, int binary) const
{
    int	  i;
    short dummy;

    if (binary)
    {
	if (!UTwrite(os, &uOrder))		return -1;
	if (!UTwrite(os, &uEndCondition))	return -1;
	if (!UTwrite(os, &dummy))		return -1; // for old bug

	for (i=0; i < uNumKnots; i++)
	    if (!UTwrite(os, &(uKnots[i]))) return -1;

	if (!UTwrite(os, &vOrder))		return -1;
	if (!UTwrite(os, &vEndCondition))	return -1;
	if (!UTwrite(os, &dummy))		return -1; // for old bug

	for (i=0; i < vNumKnots; i++)
	    if (!UTwrite(os, &(vKnots[i]))) return -1;
    }
    else
    {
	if (!(os << GPD_BASIS_N << " " << uOrder)) return -1;
	if (!(os << " " << ((uEndCondition) ? GPD_ENDS : GPD_NOENDS) << " "))
	    return -1;
	for (i=0; i < uNumKnots; i++)
	    if (!(os << " " << uKnots[i])) return -1;
	if (!(os << " \n")) return -1;

	if (!(os << GPD_BASIS_N << " " << vOrder)) return -1;
	if (!(os << " " << ((vEndCondition) ? GPD_ENDS : GPD_NOENDS) << " "))
	    return -1;
	for (i=0; i < vNumKnots; i++)
	    if (!(os << " " << vKnots[i])) return -1;
	if (!(os << " \n")) return -1;
    }

    return 0;
}

int
GPD_PrimNURBSurf::loadPrivate(istream &is, int binary)
{
    char 	bname[GPD_BUFSIZ];
    char 	endcond[256];
    short	dummy;

    if (binary)
    {
	if (!UTread(is, &uOrder))		return -1;
	if (!UTread(is, &uEndCondition))	return -1;
	if (!UTread(is, &dummy))		return -1; // to avoid old bug
    }
    else
    {
	if (!(is >> bname >> uOrder >> endcond)) return -1;
	if      (!strcmp(endcond, GPD_ENDS))  uEndCondition = 1;
	else if (!strcmp(endcond, GPD_NOENDS)) uEndCondition = 0;
	else return -1;
    }
    if (loadKnots(is, uOrder, nCols, uEndCondition, isWrappedU(), uNumKnots,
	          &uKnots, binary) < 0) return -1;
    

    if (binary)
    {
	if (!UTread(is, &vOrder))		return -1;
	if (!UTread(is, &vEndCondition))	return -1;
	if (!UTread(is, &dummy))		return -1; // to avoid old bug
    }
    else
    {
	if (!(is >> bname >> vOrder >> endcond)) return -1;
	if      (!strcmp(endcond, GPD_ENDS))  vEndCondition = 1;
	else if (!strcmp(endcond, GPD_NOENDS)) vEndCondition = 0;
	else return -1;
    }
    if (loadKnots(is, vOrder, nRows, vEndCondition, isWrappedV(), vNumKnots,
	          &vKnots, binary) < 0) return -1;
    
    return 0;
}

int
GPD_PrimNURBSurf::loadKnots(istream &is, unsigned int order, int vtx,
			    short endCondition, int wrap, int &numKnots,
			    float **knots, int binary)
{
    if (!endCondition)
    {
	numKnots = vtx + order - 2;
    }
    else
    {
	if (!wrap) numKnots = vtx - order + 2;
	else	   numKnots = vtx - order + 3;
    }
    *knots = new float[numKnots];

    if (binary)
    {
	for (int i=0; i < numKnots; i++)
	    if (!UTread(is, &((*knots)[i]))) return -1;
    }
    else
    {
	for (int i=0; i < numKnots; i++)
	    if (!(is >> (*knots)[i])) return -1;
    }
    return 0;
}


unsigned
GPD_PrimNURBSurf::getPrimitiveId() const
{
    return GPDPRIMNURBSURF;
}

