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
 * GPD_2DPoint.h
 *
 */


#ifndef __GPD_2DPoint__
#define __GPD_2DPoint__

#include <iostream.h>
#include "GPD_StreamUtil.h"

class GPD_2DPoint
{
public:
     GPD_2DPoint()	{ pos[0] = 0; pos[1] = 0; pos[2] = 1; }
    ~GPD_2DPoint()	{}

    int save(ostream &os, int binary) const
    {
	if (binary)
	    UTwrite(os, pos, sizeof(pos)/sizeof(float));
	else
	    os << pos[0] << " " << pos[1] << " " << pos[2] << endl;

	return (!os) ? -1 : 0;
    }
    
    int load(istream &is, int binary)
    {
	if (binary)
	    UTread(is, pos, sizeof(pos)/sizeof(float));
	else
	    is >> pos[0] >> pos[1] >> pos[2];

	return (!is) ? -1 : 0;
    }

    int getNum()	{ return num; }
    void setNum(int no) { num = no; }

    float 			pos[3];

private:
    int 			num;
    friend class 		GPD_Profiles;
};

#endif
