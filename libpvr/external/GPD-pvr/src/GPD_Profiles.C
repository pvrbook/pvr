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
 * GPD_Profiles.C
 *
 */



#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <iostream>
#include <strstream>
using namespace std;

#include "GPD_PrimType.h"
#include "GPD_Profile.h"
#include "GPD_2DPoint.h"
#include "GPD_2DVertex.h"
#include "GPD_2DPrimPoly.h"
#include "GPD_2DPrimRBezCurve.h"
#include "GPD_2DPrimNURBCurve.h"
#include "GPD_TrimSequence.h"
#include "GPD_Util.h"
#include "GPD_Profiles.h"

const char	*GPD_Profiles::nPoints	 	("NPoints");
const char	*GPD_Profiles::nPrims	 	("NPrims");
const char	*GPD_Profiles::nPointGroups 	("NPointGroups");
const char	*GPD_Profiles::nPrimGroups 	("NPrimGroups");

GPD_Profiles::GPD_Profiles() : ptGroups(), primGroups()
{
}

GPD_Profiles::~GPD_Profiles() 
{
    delete[] points;
    delete[] prims;
}

int  
GPD_Profiles::saveVertex(ostream &os, const GPD_2DVertex &vtx, int binary) const
{
    if (!binary)
    {
	os << " " << vtx.getPt()->getNum();
	if (!os) return -1;
    }
    else
    {
	if (fileNpt <= USHRT_MAX)
	{
	    unsigned short sidx = vtx.getPt()->getNum();
	    UTwrite(os, &sidx);
	    if (!os) return -1;
	}
	else
	{
	    unsigned int lidx = vtx.getPt()->getNum();
	    UTwrite(os, &lidx);
	    if (!os) return -1;
	}
    }
    return 0;
}

int        
GPD_Profiles::loadVertex(istream &is, GPD_2DVertex &vtx, int binary)
{
    unsigned int	lidx;

    if (!binary)
    {
	is >> lidx;
    }
    else
    {
	if (fileNpt <= USHRT_MAX)
	{
	    unsigned short	sidx;
	    UTread(is, &sidx);
	    lidx = sidx;
	}
	else UTread(is, &lidx);
    }
    if (!is) return -1;
    if (lidx >= fileNpt) return -1;

    vtx.setPt(&points[lidx]);

    return 0;
}

int
GPD_Profiles::save(ostream &os, int binary) const
{
    int i;
    int fileNptGrp 	= ptGroups.numGroups();
    int fileNprimGrp 	= primGroups.numGroups();

    // Save the total counts:
    if (binary)
    {
	if (!UTwrite(os, &fileNpt))		return -1;
	if (!UTwrite(os, &fileNprim))		return -1;
	if (!UTwrite(os, &fileNptGrp))		return -1;
	if (!UTwrite(os, &fileNprimGrp))	return -1;
    }
    else
    {
	os << nPoints << " " << fileNpt << " ";
	os << nPrims  << " " << fileNprim << endl;
	os << nPointGroups << " " << fileNptGrp << " " << nPrimGroups << " ";
	os << fileNprimGrp << endl;
	if (!os) return -1;
    }

    for (i = 0; i < fileNpt; i++)
	if (points[i].save(os,binary) < 0) return -1;

    if (binary)
    {
	for (i = 0; i < fileNprim; )
	{
	    unsigned		primType = prims[i]->getProfileId();
	    unsigned short	runCount;

	    unsigned j = i+1;
	    for (runCount=1; j < fileNprim && runCount < USHRT_MAX;
			     j++,runCount++)
		if (primType != prims[j]->getProfileId()) break;

	    if (runCount > 2)
	    {
		j = GPDPRIMRUN;

		if (!UTwrite(os, &j))		return -1;
		if (!UTwrite(os, &runCount))	return -1;
		    return -1;
	    } else runCount = 1;

	    if (!UTwrite(os, &primType)) return -1;

	    do
	    {
		if (prims[i]->save(os,binary) < 0) return -1;
		runCount--;
		i++;
	    } while (runCount > 0);
	}
    }
    else
    {
	for (i = 0; i < fileNprim; )
	{
	    unsigned		 primType = prims[i]->getProfileId();
	    unsigned short	 runCount;

	    unsigned j = i+1;
	    for (runCount=1; j<fileNprim && runCount<USHRT_MAX; j++,runCount++)
		if (primType != prims[j]->getProfileId()) break;

	    if (runCount > 2)
	    {
		os << GPDPRIMRUNN << " " << runCount << " ";
		os << (const char *)GPD_Profile::getProfileName(primType);
		os << endl;
	    }
	    else
	    {
		os << (const char *)GPD_Profile::getProfileName(primType);
		runCount = 1;
	    }
	    if (!os) return -1;

	    do {
		if (prims[i]->save(os,binary) < 0) return -1;
		runCount--;
		i++;
	    } while (runCount > 0);
	}
    }

    // Save the groups:
    if (ptGroups.save(os, binary) < 0) return -1;
    if (primGroups.save(os, binary) < 0) return -1;

    os.flush();

    return 0;
}

int
GPD_Profiles::load(istream &is, int binary)
{
    int i;
    int fileNptGrp, fileNprimGrp;

    if (binary)
    {
	if (!UTread(is, &fileNpt))	return -1;
	if (!UTread(is, &fileNprim))	return -1;
	if (!UTread(is, &fileNptGrp))	return -1;
	if (!UTread(is, &fileNprimGrp))	return -1;
    }
    else
    {
	char keyWord[GPD_BUFSIZ];
        char tmpbuf [GPD_BUFSIZ];

	if (!is.getline(tmpbuf, GPD_BUFSIZ-1)) return -1;
	istrstream pis(tmpbuf);
	pis >> keyWord; if (!pis || strcmp(keyWord,nPoints))	return -1;
	pis >> fileNpt;
	pis >> keyWord; if (!pis || strcmp(keyWord,nPrims))	return -1;
	pis >> fileNprim;
	if (!pis) return -1;

	if (!is.getline(tmpbuf, GPD_BUFSIZ-1)) return -1;
	istrstream gis(tmpbuf);
	gis >> keyWord; if (!gis || strcmp(keyWord,nPointGroups))return -1;
	gis >> fileNptGrp;
	gis >> keyWord; if (!gis || strcmp(keyWord,nPrimGroups)) return -1;
	gis >> fileNprimGrp;
	if (!gis) return -1;
    }
    if (fileNpt < 0 || fileNprim < 0) return -1;

    points = new GPD_2DPoint[fileNpt]; //faster if whole allocated at once
    for (i=0; i<fileNpt; i++) 
    {
	if (points[i].load(is, binary) < 0) return -1;
	points[i].setNum(i);
    }

    prims = new GPD_Profile*[fileNprim];//faster if space is allocated once
    if (binary)
    {
	for (i=0; i<fileNprim; )
	{
	    unsigned		 primType;
	    unsigned short	 runCount;

	    if (!UTread(is, &primType)) return -1;
	    if (primType == GPDPRIMRUN)
	    {
		if (!UTread(is, &runCount)) return -1;
		if (runCount < 2) return -1;
		if (!UTread(is, &primType)) return -1;
	    } else runCount = 1;

	    do
	    {
		if (prims[i] = (GPD_Profile *)createProfile(primType, i))
		{
		    if (prims[i]->load(is,binary) < 0) return -1;
		}
		runCount--;
		i++;
	    } while (runCount > 0);
	}
    }
    else
    {
	for (i=0; i<fileNprim; )
	{
	    unsigned		 primType;
	    char		 primName[32];
	    unsigned		 irunCount;
	    unsigned short	 runCount;

	    is >> primName;
	    if (strcmp(primName, GPDPRIMRUNN) == 0)
	    {
		is >> irunCount;
		is >> primName;
		runCount = irunCount;
	    }
	    else runCount = 1;

	    if (!is) return -1;
	    if (!(primType = GPD_Profile::getProfileType(primName))) 
		return -1;

	    do
	    {
		if (prims[i] = createProfile(primType, i))
		{
		    if (prims[i]->load(is,binary) < 0) return -1;
		}
		runCount--;
		i++;
	    } while (runCount > 0);
	}
    }

    // Now load the groups:
    if (ptGroups.load  (is, fileNptGrp, binary)<0) return -1;
    if (primGroups.load(is, fileNprimGrp, binary)<0) return -1;

    return 0;
}

GPD_Profile *
GPD_Profiles::createProfile(unsigned int type, int idx)
{
    GPD_Profile *prim;

    switch(type)
    {
	case GPDPRIMPOLY:
	    prim = (GPD_Profile *)new GPD_2DPrimPoly(this);
	    break;
	case GPDPRIMBEZCURVE:
	    prim = (GPD_Profile *)new GPD_2DPrimRBezCurve(this);
	    break;
	case GPDPRIMNURBCURVE:
	    prim = (GPD_Profile*)new GPD_2DPrimNURBCurve(this);
	    break;
	case GPDTRIMSEQUENCE:
	    prim = (GPD_Profile*)new GPD_TrimSequence(this);
	default:
	    return 0;
    }
    prim->num = idx;
    return prim;
}
