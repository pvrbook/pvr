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
 * GPD_Profile.C
 *
 */



#include <iostream>

#include "GPD_Profiles.h"
#include "GPD_Profile.h"
#include "GPD_PrimType.h"

GPD_Profile::~GPD_Profile() {}

unsigned 
GPD_Profile::getProfileType(const char *name)
{
    if (!strcmp(name, GPDPRIMPOLYN))		return GPDPRIMPOLY;
    if (!strcmp(name, GPDPRIMNURBCURVEN))	return GPDPRIMNURBCURVE;
    if (!strcmp(name, GPDPRIMBEZCURVEN))	return GPDPRIMBEZCURVE;
    if (!strcmp(name, GPDTRIMSEQUENCEN))	return GPDTRIMSEQUENCE;

    return 0;
}

const char *                            
GPD_Profile::getProfileName(unsigned type)
{
    switch (type)
    {
	case GPDPRIMPOLY:		return GPDPRIMPOLYN;
	case GPDPRIMNURBCURVE:		return GPDPRIMNURBCURVEN;
	case GPDPRIMBEZCURVE:		return GPDPRIMBEZCURVEN;
	case GPDTRIMSEQUENCE:		return GPDTRIMSEQUENCEN;
	default:			return 0;
    }
}

