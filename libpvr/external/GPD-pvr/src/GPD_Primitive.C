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
 * GPD_Primitive.C
 *
 */



#include "GPD_Detail.h"
#include "GPD_PrimType.h"
//
//	The primitive methods
//

GPD_Primitive::~GPD_Primitive() {}

unsigned 
GPD_Primitive::getPrimitiveType(const char *name)
{
    if (!strcmp(name, GPDPRIMPOLYN))		return GPDPRIMPOLY;
    // if (!strcmp(name, GPDPRIMCURVEN))	return GPDPRIMCURVE;
    if (!strcmp(name, GPDPRIMNURBCURVEN))	return GPDPRIMNURBCURVE;
    if (!strcmp(name, GPDPRIMBEZCURVEN))	return GPDPRIMBEZCURVE;

    if (!strcmp(name, GPDPRIMMESHN))		return GPDPRIMMESH;
    if (!strcmp(name, GPDPRIMNURBSURFN))	return GPDPRIMNURBSURF;
    if (!strcmp(name, GPDPRIMBEZSURFN))		return GPDPRIMBEZSURF;

    if (!strcmp(name, GPDPRIMCIRCLEN))		return GPDPRIMCIRCLE;

    if (!strcmp(name, GPDPRIMSPHEREN))		return GPDPRIMSPHERE;
    if (!strcmp(name, GPDPRIMTUBEN))		return GPDPRIMTUBE;
    if (!strcmp(name, GPDPRIMPARTN))		return GPDPRIMPART;
    if (!strcmp(name, GPDPRIMMETABALLN))	return GPDPRIMMETABALL;
    if (!strcmp(name, GPDPRIMMETASQUADN))	return GPDPRIMMETASQUAD;
    if (!strcmp(name, GPDPRIMMETALINEN))	return GPDPRIMMETALINE;
    if (!strcmp(name, GPDPRIMMETABEZN))		return GPDPRIMMETABEZ;
    if (!strcmp(name, GPDPRIMMETATRIN))		return GPDPRIMMETATRI;
    return 0;
}

const char *                            
GPD_Primitive::getPrimitiveName(unsigned type)
{
    switch (type)
    {
	case GPDPRIMPOLY:		return GPDPRIMPOLYN;
	// case GPDPRIMCURVE:		return GPDPRIMCURVEN;
	case GPDPRIMNURBCURVE:		return GPDPRIMNURBCURVEN;
	case GPDPRIMBEZCURVE:		return GPDPRIMBEZCURVEN;

	case GPDPRIMMESH:		return GPDPRIMMESHN;
	case GPDPRIMNURBSURF:		return GPDPRIMNURBSURFN;
	case GPDPRIMBEZSURF:		return GPDPRIMBEZSURFN;

	case GPDPRIMCIRCLE:		return GPDPRIMCIRCLEN;

	case GPDPRIMSPHERE:		return GPDPRIMSPHEREN;
	case GPDPRIMTUBE:		return GPDPRIMTUBEN;
	case GPDPRIMPART:		return GPDPRIMPARTN;
        case GPDPRIMMETABALL:		return GPDPRIMMETABALLN;
    	case GPDPRIMMETASQUAD:		return GPDPRIMMETASQUADN;
    	case GPDPRIMMETALINE:		return GPDPRIMMETALINEN;
    	case GPDPRIMMETABEZ:		return GPDPRIMMETABEZN;
    	case GPDPRIMMETATRI:		return GPDPRIMMETATRIN;
	default:			return 0;
    }
}
