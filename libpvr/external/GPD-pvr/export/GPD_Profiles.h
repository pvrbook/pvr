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
 * GPD_Profiles.h
 *
 */



#ifndef __GPD_Profiles_h__
#define __GPD_Profiles_h__

#include "GPD_StreamUtil.h"
#include "GPD_Group.h"

#include <iosfwd>
using namespace std;

class GPD_2DVertex;
class GPD_2DPoint;
class GPD_Profile;

class GPD_Profiles {
public:
     GPD_Profiles();
    ~GPD_Profiles();

    int		 save(ostream& os, int binary = 0) const;
    int		 load(istream &is, int binary = 0);

    int		 saveVertex(ostream &os, const GPD_2DVertex &vtx, int b) const;
    int		 loadVertex(istream &is, GPD_2DVertex &vtx, int b);


protected:
private:
    int fileNpt, fileNprim;

    GPD_Profile			*createProfile(unsigned int type, int idx);

    GPD_2DPoint			*points;   // list of points
    GPD_PointGroupList 		 ptGroups; // list of groups of points

    GPD_Profile			**prims;     // list of primitives
    GPD_PrimGroupList		  primGroups;// list of primitive groups

    // Text constants used in the data file.
    static const char	 *nPoints;
    static const char	 *nPrims;
    static const char	 *nPointGroups;
    static const char	 *nPrimGroups;
};

#endif
