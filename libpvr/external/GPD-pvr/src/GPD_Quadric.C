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
 * GPD_Quadric.C
 *
 */


#include "GPD_Detail.h"
#include "GPD_Quadric.h"

GPD_Quadric::~GPD_Quadric() {}

int
GPD_Quadric::savePrivate(ostream &, int) const
{
    return 0;
}

int
GPD_Quadric::loadPrivate(istream &, int)
{
    return 0;
}

int  
GPD_Quadric::save(ostream &os, int binary) const
{
    int i, j;

    if (getParent()->saveVertex(os, myVertex) < 0) return -1;
    if (savePrivate(os, binary) < 0) return -1;

    if (binary)
    {
	for (i=0; i < 3; i++)
	    for (j=0; j < 3; j++)
		UTwrite(os, &myXform[i][j]);
	if (!os) return -1;
    }
    else
    {
	os <<" "<< myXform[0][0] <<" " << myXform[0][1] << " " << myXform[0][2];
	os <<" "<< myXform[1][0] <<" " << myXform[1][1] << " " << myXform[1][2];
	os <<" "<< myXform[2][0] <<" " << myXform[2][1] << " " << myXform[2][2];
	if (!os) return -1;
    }

    if (getParent()->primitiveAttribs().save(os, attribData, binary, '[' ) < 0)
	return -1;

    if (!binary) os << endl;

    return (!os) ? -1 : 0;
}

int
GPD_Quadric::load(istream &is, int binary)
{
    int 	i, j;

    if (getParent()->loadVertex(is, myVertex) < 0 ) return -1;
    if (loadPrivate(is, binary) < 0) return -1;

    if (binary)
    {
	for (i = 0; i < 3; i++)
	    for (j = 0; j < 3; j++)
		if (!UTread(is, &myXform[i][j])) return -1;
    }
    else
    {
	is >> myXform[0][0] >> myXform[0][1] >> myXform[0][2];
	is >> myXform[1][0] >> myXform[1][1] >> myXform[1][2];
	is >> myXform[2][0] >> myXform[2][1] >> myXform[2][2];
	if (!is) return -1;
    }

    if (getParent()->primitiveAttribs().load(is, attribData, binary) < 0)
	return -1;

    if (!binary) // Get junk at end of line
	is.ignore(GPD_BUFSIZ-1, '\n');
    return (!is) ? -1 : 0;
}

