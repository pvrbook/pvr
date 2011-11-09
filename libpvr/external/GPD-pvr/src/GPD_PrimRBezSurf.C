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
 * GPD_PrimRBezSurf.C
 *
 */


#include "GPD_PrimRBezSurf.h"
#include "GPD_PrimType.h"
#include "GPD_Util.h"
#include "GPD_Detail.h"

GPD_PrimRBezSurf::GPD_PrimRBezSurf(GPD_Detail *d) : GPD_TPSurf(d)
{
}

GPD_PrimRBezSurf::~GPD_PrimRBezSurf()
{
}

int
GPD_PrimRBezSurf::savePrivate(ostream &os, int binary) const
{
    int i;

    if (binary)
    {
	char useflagb = 1;
	if (!UTwrite(os, &uOrder))	return -1;
	if (!UTwrite(os, &useflagb))	return -1;
	for (i=0; i < uNumKnots; i++)
	    if (!UTwrite(os, &(uKnots[i]))) return -1;

	if (!UTwrite(os, &vOrder))	return -1;
	if (!UTwrite(os, &useflagb))	return -1;
	for (i=0; i < vNumKnots; i++)
	    if (!UTwrite(os, &(vKnots[i])))	return -1;
    }
    else
    {
	if (!(os << GPD_BASIS_N << " " << uOrder << " use ")) return -1;
	for (i=0; i < uNumKnots; i++)
	    if (!(os << " " << uKnots[i])) return -1;
	if (!(os << " \n")) return -1;

	if (!(os << GPD_BASIS_N << " " << vOrder << " use ")) return -1;
	for (i=0; i < vNumKnots; i++)
	    if (!(os << " " << vKnots[i])) return -1;
	if (!(os << " \n")) return -1;
    }
    return 0;
}

int
GPD_PrimRBezSurf::loadPrivate(istream &is, int binary)
{
    int i;
    int usebkp;

    if (binary)
    {
	if (!UTread(is, &uOrder)) return -1;
    }
    else
    {
	char bname[GPD_BUFSIZ];
	if (!(is >> bname >> uOrder)) return -1;
    }
    if (uOrder < 2) return -1;

    if ((usebkp = GPDuseBreakpoints(is,binary)) < 0) return -1;

    uNumKnots = (nCols-!isWrappedU()) / (uOrder-1) + !isWrappedU();
    uKnots    = new float[uNumKnots];

    if (usebkp)
    {
	if (binary)
	{
	    for (i=0; i < uNumKnots; i++)
		if (!UTread(is, &(uKnots[i]))) return -1;
	}
	else
	{
	    for (i=0; i < uNumKnots; i++)
		if (!(is >> uKnots[i])) return -1;
	}
    }
    else
    {
	for (i = 0; i < uNumKnots; i++) uKnots[i] = i;
    }

    if (binary)
    {
	if (!UTread(is, &vOrder)) return -1;
    }
    else
    {
	char bname[GPD_BUFSIZ];
	if (!(is >> bname >> vOrder)) return -1;
    }
    if (vOrder < 2) return -1;

    if ((usebkp = GPDuseBreakpoints(is,binary)) < 0) return -1;

    vNumKnots = (nRows-!isWrappedV()) / (vOrder-1) + !isWrappedV();
    vKnots    = new float[vNumKnots];

    if (usebkp)
    {
	if (binary)
	{
	    for (i=0; i < vNumKnots; i++)
		if (!UTread(is, &(vKnots[i]))) return -1;
	}
	else
	{
	    for (i=0; i < vNumKnots; i++)
		if (!(is >> vKnots[i])) return -1;
	}
    }
    else
    {
	for (i = 0; i < vNumKnots; i++) vKnots[i] = i;
    }

    return 0;
}

unsigned
GPD_PrimRBezSurf::getPrimitiveId() const
{
    return GPDPRIMBEZSURF;
}

