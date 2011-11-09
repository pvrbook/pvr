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
 * GPD_Point.C
 *
 */


#include "GPD_Point.h"

GPD_Point::GPD_Point() 
: attribData() 
{
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    pos[3] = 1;
}

GPD_Point::~GPD_Point() {}

int
GPD_Point::saveASCII(ostream &os) const
{
    os << pos[0] << " " << pos[1] << " " << pos[2] << " " << pos[3];
    return (!os) ? -1 : 0;
}

int
GPD_Point::saveBinary(ostream &os) const
{
    UTwrite(os, pos, sizeof(pos)/sizeof(float));
    return (!os) ? -1 : 0;
}

int
GPD_Point::loadBinary(istream &is)
{
    UTread(is, pos, sizeof(pos)/sizeof(float));
    return (!is) ? -1 : 0;
}
