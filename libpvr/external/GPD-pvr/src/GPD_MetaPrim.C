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
 * GPD_MetaPrim.C
 *
 */


#include <iostream>
#include "GPD_MetaPrim.h"
#include "GPD_StreamUtil.h"

GPD_MetaPrim::GPD_MetaPrim()
{
    weight = 2; kernel = GPD_WYVILL_KERNEL;
}

GPD_MetaPrim::~GPD_MetaPrim() {}

void
GPD_MetaPrim::setMetaKernel(const char *name) 
{
    switch (*name)
    {
	case 'q':	kernel = GPD_QUARTIC_KERNEL; break;
	case 'b':	kernel = GPD_BLINN_KERNEL; break;
	case 'l':	kernel = GPD_LINKS_KERNEL; break;
	case 'w':	kernel = GPD_WYVILL_KERNEL; break;
    }
}

const char *
GPD_MetaPrim::getMetaKernel(GPD_META_KERNEL_TYPE type) const
{
    switch (type)
    {
	case GPD_WYVILL_KERNEL:		return "wyvill";
	case GPD_QUARTIC_KERNEL:	return "quartic";
	case GPD_BLINN_KERNEL: 		return "blinn";
	case GPD_LINKS_KERNEL:
	default:			return "links";
    }
}


int
GPD_MetaPrim::saveMeta(ostream &os, int binary) const
{
    const char *kname = getMetaKernel(kernel);

    if (binary)
    {
	if (!UTwrite(os, kname))	return -1;
	if (!UTwrite(os, &weight))	return -1;
    }
    else
    {
	os << " " << kname << " " << weight;
	if (!os) return -1;
    }
    return 0;
}

int
GPD_MetaPrim::loadMeta(istream &is, int binary)
{
    char	kname[64];

    if (binary)
    {
	if (!UTread(is, kname))		return -1;
	if (!UTread(is, &weight))	return -1;
	kname[1] = 0;
    }
    else
    {
	is >> kname >> weight;
	if (!is) return -1;
    }
    setMetaKernel(kname);
    return 0;
}

