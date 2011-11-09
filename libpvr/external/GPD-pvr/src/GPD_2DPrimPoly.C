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
 * GPD_2DPrimPoly.C
 *
 */


#include "GPD_Profiles.h"
#include "GPD_2DPrimPoly.h"
#include "GPD_PrimType.h"
#include "GPD_Util.h"

GPD_2DPrimPoly::GPD_2DPrimPoly(GPD_Profiles *d) 
: GPD_Profile(d)
{
    // Nothing, but cannot be inlined.
}

GPD_2DPrimPoly::~GPD_2DPrimPoly()
{
    delete[] vtxList;
}

int
GPD_2DPrimPoly::save(ostream &os, int binary) const
{
    int		cnt;

    if (binary)
    {
	const char tflag = isTrimmed() ? 1 : 0;
	if (!UTwrite(os, &tflag))    return -1;

	if (!UTwrite(os, &nVtx)) return -1;
	const char cflag = isClosed() ? '<' : ':';
	if (!UTwrite(os, &cflag))	return -1;
    }
    else
    {
	os << " " << (isTrimmed() ? "trim" : "idle");
	os << " " << nVtx << " " << (isClosed() ? '<' : ':');
	if (!os) return -1;
    }
    if (savePrivate(os,binary) < 0) return -1;
    for (cnt = 0; cnt < nVtx; cnt++)
	if (getParent()->saveVertex(os, vtxList[cnt], binary) < 0) return -1;

    if (!binary) os << endl;
    return (!os) ? -1 : 0;
}

int
GPD_2DPrimPoly::load(istream &is, int binary)
{
    int		 cnt;
    char	 cflag;

    if (binary)
    {
	char tflag;

	if (!UTread(is, &tflag))	return -1;
	if (!UTread(is, &nVtx))		return -1;
	if (!UTread(is, &cflag))	return -1;

	if (tflag == 1)	trim();
	else		untrim();
    }
    else
    {
	char ttoken[GPD_BUFSIZ];

	is >> ttoken;
	is >> nVtx >> cflag; 
	if (!is) return -1;

	if (!strcmp(ttoken, "trim"))	trim();
	else				untrim();
    }

    if (cflag == '<')	close();
    else		open();

    vtxList = new GPD_2DVertex[nVtx]; 

    if (loadPrivate(is,binary) < 0) return -1;

    for (cnt = 0; cnt < nVtx; cnt++)
	if (getParent()->loadVertex(is, vtxList[cnt], binary) < 0) return -1;

    if (!binary) // Get junk at end of line:
	is.ignore(GPD_BUFSIZ-1, '\n');
    if (!is) return -1;
    return 0;
}

unsigned
GPD_2DPrimPoly::getProfileId() const
{
    return GPDPRIMPOLY;
}

int
GPD_2DPrimPoly::savePrivate(ostream &, int) const
{
    return 0;
}

int
GPD_2DPrimPoly::loadPrivate(istream &, int)
{
    return 0;
}

