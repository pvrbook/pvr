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
 * GPD_TrimSequence.C
 *
 */


#include <iostream.h>

#include "GPD_TrimSequence.h"
#include "GPD_PrimType.h"
#include "GPD_Profiles.h"

int
GPD_TrimLocator::save(ostream &os, int binary) const
{
    if (binary)
    {
	float pos[2]; pos[0] = u; pos[1] = v;
	if (!UTwrite(os, &trimmed)) return -1;
	if (!UTwrite(os, pos, sizeof(pos)/sizeof(float))) return -1;
    }
    else
    {
	const char tflag = trimmed ? '-' : '+';
	os << tflag << '(' << u << " " << v << ')';
	os << " ";
	if (!os) return -1;
    }

    return 0;
}

int
GPD_TrimLocator::load(istream &is, int binary)
{
    if (binary)
    {
	float	pos[2];

	if (!UTread(is, &trimmed)) return -1;
	if (!UTread(is, pos, sizeof(pos)/sizeof(float))) return -1;

	u = pos[0];
	v = pos[1];
    }
    else
    {
	char	tflag, lbrace, rbrace;

	is >> tflag >> lbrace >> u >> v >> rbrace;
	if (!is) return -1;

	trimmed = (tflag == '-');
    }

    return 0;
}

GPD_TrimSequence::GPD_TrimSequence(GPD_Profiles *d) : GPD_Profile(d)
{
    vtxList = 0;
    nVtx = 0;
}

GPD_TrimSequence::~GPD_TrimSequence()
{
    delete [] vtxList;
}

unsigned
GPD_TrimSequence::getProfileId(void) const
{
    return GPDTRIMSEQUENCE;
}

int
GPD_TrimSequence::save(ostream &os, int binary) const
{
    if (!nVtx) return 0;

    int i;

    if (binary)
    {
	if (!UTwrite(os, &nVtx)) return -1;
    }
    else
    {
	if (!(os << " " << nVtx)) return -1;
    }

    for (i = 0; i < nVtx; i++)
	if (vtxList[i].save(os,binary) < 0) return -1;

    if (!binary) os << endl;

    return (!os) ? -1 : 0;
}

int
GPD_TrimSequence::load(istream &is, int binary)
{
    int i;

    if (binary)
    {
	if (!UTread(is, &nVtx)) return -1;
    }
    else
    {
	if (!(is >> nVtx)) return -1;
    }
    if (nVtx <  0) return -1;
    if (nVtx == 0) return  0;

    vtxList = new GPD_TrimLocator[nVtx];

    for (i = 0; i < nVtx; i++)
	if (vtxList[i].load(is,binary) < 0) return -1;

    return 0;
}
