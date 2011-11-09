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
 * GPD_2DPrimNURBCurve.C
 *
 */


#include "GPD_2DPrimNURBCurve.h"
#include "GPD_PrimType.h"
#include "GPD_Profiles.h"
#include "GPD_Util.h"

#define GPD_ENDS	"end"
#define GPD_NOENDS	"noend"

GPD_2DPrimNURBCurve::GPD_2DPrimNURBCurve(GPD_Profiles *d) : GPD_2DPrimPoly(d)
{
    knots = 0;
    numKnots = 0;
}

GPD_2DPrimNURBCurve::~GPD_2DPrimNURBCurve()
{
    delete[] knots;
}


int
GPD_2DPrimNURBCurve::savePrivate(ostream &os, int binary) const
{
    if (binary)
    {
	short dummy;

	if (!UTwrite(os, &order))	 return -1;
	if (!UTwrite(os, &endCondition)) return -1;
	if (!UTwrite(os, &dummy))	 return -1; // for old bug's sake

	for (int i=0; i < numKnots; i++)
	    if (!UTwrite(os, &(knots[i]))) return -1;
    }
    else
    {
	if (!(os << " " << GPD_BASIS_N << " " << order)) return -1;
	if (!(os << " " << ((endCondition) ? GPD_ENDS : GPD_NOENDS) << " "))
	    return -1;
	for (int i=0; i < numKnots; i++)
	    if (!(os << " " << knots[i])) return -1;
	if (!(os << " \n")) return -1;
    }
    return 0;
}

int
GPD_2DPrimNURBCurve::loadPrivate(istream &is, int binary) 
{
    int   i;
    short dummy;

    if (binary)
    {
	if (!UTread(is, &order))	return -1;
	if (!UTread(is, &endCondition)) return -1;
	if (!UTread(is, &dummy))	return -1; // for old bug's sake
    }
    else
    {
	char bname[GPD_BUFSIZ];
	if (!(is >> bname >> order)) return -1;
	char endcond[256];
	if (!(is >> endcond)) return -1;
	if	(!strcmp(endcond, GPD_ENDS))	endCondition = 1;
	else if (!strcmp(endcond, GPD_NOENDS))endCondition = 0;
	else return -1;
    }

    if (!endCondition)
    {
	numKnots = nVtx + order - 2;
    }
    else
    {
	if (!isClosed()) numKnots = nVtx - order + 2;
	else		 numKnots = nVtx - order + 3;
    }
    knots = new float[numKnots];

    if (binary)
    {
	for (i=0; i < numKnots; i++)
	    if (!UTread(is, &(knots[i]))) return -1;
    }
    else
    {
	for (i=0; i < numKnots; i++)
	    if (!(is >> knots[i])) return -1;
    }
    return 0;
}


unsigned
GPD_2DPrimNURBCurve::getProfileId() const
{
    return GPDPRIMNURBCURVE;
}


