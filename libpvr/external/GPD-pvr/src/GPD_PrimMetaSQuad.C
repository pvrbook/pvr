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
 * GPD_PrimMetaSQuad.C
 *
 */


#include "GPD_PrimMetaSQuad.h"
#include "GPD_Detail.h"
#include <iostream>

GPD_PrimMetaSQuad::~GPD_PrimMetaSQuad() {}

unsigned
GPD_PrimMetaSQuad::getPrimitiveId() const
{
    return GPDPRIMMETASQUAD;
}

int
GPD_PrimMetaSQuad::savePrivate(ostream &os, int binary) const
{
    if (binary)
    {
	if (!UTwrite(os, &xyExp))	return -1;
	if (!UTwrite(os, &zExp))	return -1;
    }
    else
    {
	os << " " << xyExp << " " << zExp;
	if (!os) return -1;
    }
    if (saveMeta(os, binary) < 0) return -1;

    return GPD_PrimSphere::savePrivate(os, binary);
}

int
GPD_PrimMetaSQuad::loadPrivate(istream &is, int binary)
{
    if (binary)
    {
	if (!UTread(is, &xyExp)) 	return -1;
	if (!UTread(is, &zExp))		return -1;
    }
    else
    {
	is >> xyExp >> zExp;
	if (!is) return -1;
    }
    if (loadMeta(is, binary) < 0) return -1;

    return GPD_PrimSphere::loadPrivate(is, binary);
}

