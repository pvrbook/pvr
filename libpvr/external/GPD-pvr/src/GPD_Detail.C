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
 * GPD_Detail.C
 *
 */



#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <strstream>
using namespace std;

#include "GPD_Primitive.h"
#include "GPD_PrimType.h"
#include "GPD_Detail.h"
#include "GPD_Vertex.h"
#include "GPD_PrimTube.h"
#include "GPD_PrimCircle.h"
#include "GPD_PrimSphere.h"
#include "GPD_PrimPoly.h"
#include "GPD_PrimMesh.h"
#include "GPD_PrimRBezCurve.h"
#include "GPD_PrimNURBCurve.h"
#include "GPD_PrimRBezSurf.h"
#include "GPD_PrimNURBSurf.h"
#include "GPD_PrimPart.h"
#include "GPD_Point.h"
#include "GPD_PrimMetaBall.h"
#include "GPD_PrimMetaSQuad.h"
#include "GPD_PrimTriFan.h"
#include "GPD_PrimTriStrip.h"

const char	*GPD_Detail::nPoints	 	("NPoints");
const char	*GPD_Detail::nPrims	 	("NPrims");
const char	*GPD_Detail::nPointGroups 	("NPointGroups");
const char	*GPD_Detail::nPrimGroups 	("NPrimGroups");
const char	*GPD_Detail::nPointAttrib 	("NPointAttrib");
const char	*GPD_Detail::nVertexAttrib	("NVertexAttrib");
const char	*GPD_Detail::nPrimAttrib	("NPrimAttrib");
const char	*GPD_Detail::nDetailAttrib	("NAttrib");
const char	*GPD_Detail::beginExtra		("beginExtra");
const char	*GPD_Detail::endExtra		("endExtra");
const char	 GPD_Detail::beginExtraMagic	= 0x00;
const char	 GPD_Detail::endExtraMagic	= 0xff;

static const char	 versionName	= 'V';


GPD_Detail::GPD_Detail()
			: ptGroups(), vtxAttrib(),
	                  primGroups(), primAttrib(),
			  detailAttrib(), points(0), prims(0)
{
    version = 5;
}

GPD_Detail::~GPD_Detail()
{
    int i;

    for( i = 0; i < fileNpt; i++ )
        delete points[i];
    delete[] points;

    for( i = 0; i < fileNprim; i++ )
        delete prims[i];
    delete[] prims;
} 

int  
GPD_Detail::saveVertex(ostream &os, const GPD_Vertex &vtx) const
{
    if (ascii())
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
    return vtxAttrib.save(os, vtx.attribData, !ascii());
}

int        
GPD_Detail::loadVertex(istream &is, GPD_Vertex &vtx)
{
    unsigned int	lidx;

    if (ascii())
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
    vtx.setPt(points[lidx]);
    return vtxAttrib.load(is, vtx.attribData, !ascii());
}

int
GPD_Detail::save(const char *name) const
{
    GPD_Detail		*me = (GPD_Detail*)this;

    // If the user chooses STDOUT_AUTO, we'll choose .bgeo for him :-)
    if (!strcmp(name, STDOUT_BINARY))
    {
	me->ascii(0);
	return save(cout);
    }
    else if (!strcmp(name, STDOUT_ASCII))
    {
	me->ascii(1);
	return save(cout);
    }
    if (!strcmp(name, STDOUT_AUTO))
    {
	me->ascii(0);
	return save(cout);
    }
    else if (name)
    {
	char		*ext = name ? strrchr(name, '.') : 0;
	int		 rcode;

	me->ascii(!strcmp(ext,".geo") ? 1 : 0);
	ofstream ofs((const char *)name); 
	if (!ofs) return -1;
	rcode = save(ofs);
	ofs.close();
	return rcode;
    }
    else
	return -1;
}

int
GPD_Detail::save(ostream &os) const
{
    int fileNptGrp 	= ptGroups.numGroups();
    int fileNprimGrp 	= primGroups.numGroups();
    int fileNptAttrib 	= ptAttrib.numAttribs();
    int fileNvtxAttrib 	= vtxAttrib.numAttribs();
    int fileNprimAttrib = primAttrib.numAttribs();
    int fileNattrib 	= detailAttrib.numAttribs();

    // We always save with the current version, which is 5.
    int		saveversion = 5;

    int magic  = (!ascii()) ? binaryMagic() : asciiMagic();
    int binary = (!ascii()) ? 1+saveversion : 0;

    if (!UTwrite(os, &magic)) return -1;
    if (ascii()) os << "METRY ";	// Make ASCII look nice :-)


    // Save the total counts:
    if (binary)
    {
	// Write out the version number, eg. V1
	if (!UTwrite(os, &versionName))		return -1;
	if (!UTwrite(os, &saveversion))		return -1;

	if (!UTwrite(os, &fileNpt))		return -1;
	if (!UTwrite(os, &fileNprim))		return -1;
	if (!UTwrite(os, &fileNptGrp))		return -1;
	if (!UTwrite(os, &fileNprimGrp))	return -1;
	if (!UTwrite(os, &fileNptAttrib))	return -1;
	if (!UTwrite(os, &fileNvtxAttrib))	return -1;
	if (!UTwrite(os, &fileNprimAttrib))	return -1;
	if (!UTwrite(os, &fileNattrib))		return -1;
    }
    else
    {
	os << versionName << saveversion << endl;

	os << nPoints << " " << fileNpt << " " << nPrims << " " << fileNprim << endl;
	os << nPointGroups << " " << fileNptGrp << " " << nPrimGroups << " ";
	os << fileNprimGrp << endl;
	os << nPointAttrib << " " << fileNptAttrib << " " << nVertexAttrib << " ";
	os << fileNvtxAttrib << " " << nPrimAttrib << " " << fileNprimAttrib << " ";
	os << nDetailAttrib << " " << fileNattrib << endl;
	if (!os) return -1;
    }
    int i;

    // Save the points and their attributes:
    if (fileNptAttrib && ptAttrib.save(os, binary ? 0:"PointAttrib") < 0) return -1;
    if (binary)
    {
	for (i = 0; i < fileNpt; i++)
	    if (saveBinaryPoint(os, *points[i]) < 0) return -1;
    }
    else
    {
	for (i = 0; i < fileNpt; i++)
	    if (saveASCIIPoint(os, *points[i]) < 0) return -1;
    }

    //
    // Save the primitives and their attributes - we have to save the
    //	vertex attrib dictionary too.
    //
    if (fileNvtxAttrib && vtxAttrib.save(os, binary ? 0 : "VertexAttrib") < 0)
	return -1;

    if (fileNprimAttrib && primAttrib.save(os, !ascii() ?0:"PrimitiveAttrib") < 0) 
	return -1;

    if (binary)
    {
	for (i = 0; i < fileNprim; )
	{
	    unsigned		primType = prims[i]->getPrimitiveId();
	    unsigned short	runCount;

	    unsigned j = i+1;
	    for (runCount=1; j < fileNprim && runCount < USHRT_MAX;
			     j++,runCount++)
		if (primType != prims[j]->getPrimitiveId()) break;

	    if (runCount > 2)
	    {
		j = GPDPRIMRUN;

		if (!UTwrite(os, &j))		return -1;
		if (!UTwrite(os, &runCount))	return -1;
	    } else runCount = 1;

	    if (!UTwrite(os, &primType)) return -1;

	    do
	    {
		if (prims[i]->save(os, binary) < 0) return -1;
		runCount--;
		i++;
	    } while (runCount > 0);
	}
    }
    else
    {
	for (i = 0; i < fileNprim; )
	{
	    unsigned		 primType = prims[i]->getPrimitiveId();
	    unsigned short	 runCount;

	    unsigned j = i+1;
	    for (runCount=1; j < fileNprim && runCount < USHRT_MAX; j++,runCount++)
		if (primType != prims[j]->getPrimitiveId()) break;

	    if (runCount > 2)
	    {
		os << GPDPRIMRUNN << " " << runCount << " ";
		os << (const char *)GPD_Primitive::getPrimitiveName(primType);
		os << endl;
	    }
	    else
	    {
		os << (const char *)GPD_Primitive::getPrimitiveName(primType);
		runCount = 1;
	    }
	    if (!os) return -1;

	    do {
		if (prims[i]->save(os, binary) < 0) return -1;
		runCount--;
		i++;
	    } while (runCount > 0);
	}
    }

    // Now save the detail attributes:
    if (fileNattrib && detailAttrib.GPD_AttributeDict::save(os,
	   (char*)binary ? 0 : "DetailAttrib") < 0) return -1;
    if (detailAttrib.save(os, binary) < 0) return -1;
    if (detailAttrib.getHead() && !ascii()) os << endl;

    // Save the groups:
    if (ptGroups.save(os, binary) < 0) return -1;
    if (primGroups.save(os, binary) < 0) return -1;

    // Save the extra info, such as meta expressions and surface pasting
    if (!binary)
    {
	if (!(os << beginExtra << endl)) return -1;

	// ... this is where all the extra stuff will go...

	if (!(os << endExtra << endl)) return -1;
    }
    else
    {
	if (!UTwrite(os, &beginExtraMagic)) return -1;

	// ... this is where all the extra stuff will go...

	if (!UTwrite(os, &endExtraMagic)) return -1;
    }

    os.flush();

    return 0;
}

int
GPD_Detail::checkMagic(istream &is) const
{
    int		magic;

    if (!UTread(is, &magic)) return -1;
    if (magic == binaryMagic()) return 1;
    if (magic == asciiMagic())  return 0;
    return -1;		// Not a valid file
}

int
GPD_Detail::load(const char *name)
{
    if (!strcmp(name, STDIN_BINARY) || !strcmp(name, STDIN_ASCII)
				    || !strcmp(name, STDIN_AUTO))
    {
	return load(cin);
    }
    else
    {
	ifstream is(name);
	if (!is) return -1;
	int rcode = load(is);
	is.close();
	return rcode;
    }
}

int
GPD_Detail::load(istream &is)
{
    int fileNptGrp, fileNprimGrp;
    int fileNptAttrib, fileNvtxAttrib, fileNprimAttrib, fileNattrib;

    int			binary;
    unsigned		magic;
    char		tmpbuf [GPD_BUFSIZ];


    if (!UTread(is, &magic)) {
      return -1;
    }
    
    if (magic == binaryMagic()) {
      binary = 1;
    } else if (magic == asciiMagic()) {
      binary = 0;
    } else {
      return -1;
    }

    ascii(!binary);

    if (loadVersion(is,binary) < 0) return -1;
    if (binary) binary += getVersion();

    if (binary)
    {
	if (!UTread(is, &fileNpt))		return -1;
	if (!UTread(is, &fileNprim))		return -1;
	if (!UTread(is, &fileNptGrp))		return -1;
	if (!UTread(is, &fileNprimGrp))		return -1;
	if (!UTread(is, &fileNptAttrib))	return -1;
	if (!UTread(is, &fileNvtxAttrib))	return -1;
	if (!UTread(is, &fileNprimAttrib))	return -1;
	if (!UTread(is, &fileNattrib))		return -1;
    }
    else
    {
	char keyWord[GPD_BUFSIZ];

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

	if (!is.getline(tmpbuf, GPD_BUFSIZ-1)) return -1;
	istrstream ais(tmpbuf);
	ais >> keyWord; if (!ais || strcmp(keyWord,nPointAttrib)) return -1;
	ais >> fileNptAttrib;
	ais >> keyWord; if (!ais || strcmp(keyWord,nVertexAttrib)) return -1;
	ais >> fileNvtxAttrib;
	ais >> keyWord; if (!ais || strcmp(keyWord,nPrimAttrib))  return -1;
	ais >> fileNprimAttrib;
	ais >> keyWord; if (!ais || strcmp(keyWord,nDetailAttrib))return -1;
	ais >> fileNattrib;
	if (!ais) return -1;
    }
    if (fileNpt < 0 || fileNprim < 0) return -1;

    int		 i;

    // Load point attribute table and the point list:
    if (fileNptAttrib > 0 && !ptAttrib.load(is,fileNptAttrib,binary)) return -1;

    int ptAttribSize = ptAttrib.getAlloc();

    points = new GPD_Point*[fileNpt]; //faster if whole allocated at once
    for (i = 0; i < fileNpt; i++)
	points[i] = new GPD_Point;
    
    if (ptAttribSize)
    {
        for (i=0; i<fileNpt; i++)
	    ptAttrib.useDefaults(points[i]->attribData, 1);
    }

    if (binary)                       //load the points
    {
	for (i=0; i<fileNpt; i++) 
	    {
	        if (loadBinaryPoint(is, *points[i]) < 0) return -1;
	        points[i]->setNum(i);
	    }
    }
    else
    {
	for (i=0; i<fileNpt; i++)
	    {
		if (loadASCIIPoint(is, *points[i]) < 0) return -1;
		points[i]->setNum(i);
	    }
    }
    // Load primitive attribute table and the primitive list:
    if (fileNvtxAttrib > 0 && !vtxAttrib.load(is, fileNvtxAttrib, binary))
	return -1;

    if (fileNprimAttrib > 0 && !primAttrib.load(is,fileNprimAttrib,binary))
	return -1;

    prims = new GPD_Primitive*[fileNprim];//faster if space is allocated once
    if (binary)
    {
	for (i=0; i<fileNprim; )
	{
	    unsigned		 primType;
	    unsigned short	 runCount;

	    if (!UTread(is, &primType)) return -1;
	    if (primType == GPDPRIMRUN)
	    {
		if (!UTread(is, &runCount))	return -1;
		if (runCount < 2) return -1;
		if (!UTread(is, &primType))	return -1;
	    } else runCount = 1;

	    do
	    {
		if (prims[i] = (GPD_Primitive *)createPrimitive(primType, i))
		{
		    if (prims[i]->load(is, binary) < 0) return -1;
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
	    if (!(primType = GPD_Primitive::getPrimitiveType(primName))) 
		return -1;

	    do
	    {
		if (prims[i] = createPrimitive(primType, i))
		{
		    if (prims[i]->load(is, binary) < 0) return -1;
		}
		runCount--;
		i++;
	    } while (runCount > 0);
	}
    }

    // Load detail attributes:
    if (fileNattrib > 0)
    {
        if (!detailAttrib.GPD_AttributeDict::load(is, fileNattrib, binary))
	    return -1;
	detailAttrib.changeDataSize(detailAttrib.getAlloc());
	if (detailAttrib.load(is, binary) < 0) return -1;
	if (!binary) if (!is.ignore(GPD_BUFSIZ-1, '\n')) return -1;
    }

    // Now load the groups:
    if (ptGroups.load  (is, fileNptGrp, binary) < 0)
	return -1;
    if (primGroups.load(is, fileNprimGrp, binary) < 0)
	return -1;

#if 0

    //
    // If-ed it all out because in GPD we don't support anything between
    // beginExtra and endExtra anyway. -CB
    //

    // Load the extra info, such as meta expressions and surface pasting:
    char extras;
    if (!binary)
    {
	if (!(is >> tmpbuf)) return -1;
	if (strcmp(tmpbuf, beginExtra)) return -1;
	if (!is.ignore(GPD_BUFSIZ-1, '\n')) return -1;

	// ... this is where the extra stuff goes...

	if (!(is >> tmpbuf)) return -1;
	if (strcmp(tmpbuf, endExtra )) return -1;
    }
    else
    {
	if (!UTread(is, &extras)) return -1;
	if (extras != beginExtraMagic) return -1;

	// ... this is where the extra stuff goes ...

	if (!UTread(is, &extras)) return -1;
	if (extras != endExtraMagic ) return -1;
    }
#endif
    return 0;
}

int
GPD_Detail::saveBinaryPoint(ostream &os, const GPD_Point &ppt) const
{
    if (ppt.saveBinary(os) < 0)				return -1;
    if (ptAttrib.save(os, ppt.attribData, 1) < 0)	return -1;
    return 0;
}

int  
GPD_Detail::saveASCIIPoint(ostream &os, const GPD_Point &ppt) const
{
    if (ppt.saveASCII(os) < 0)				return -1;
    if (ptAttrib.save(os, ppt.attribData, 0) < 0)      return -1;
    os << endl; 
    return (!os) ? -1 : 0;
}

int
GPD_Detail::loadBinaryPoint(istream &is, GPD_Point &ppt)
{
    if (ppt.loadBinary(is) < 0)				return -1;
    if (ptAttrib.load(is, ppt.attribData, 1) < 0)	return -1;
    return 0;
}

int
GPD_Detail::loadASCIIPoint(istream &is, GPD_Point &ppt)
{
    if (ppt.loadASCII(is) < 0)				return -1;
    if (ptAttrib.load(is, ppt.attribData, 0) < 0)	return -1;
    return 0;
}

GPD_Point *
GPD_Detail::appendPoint()
{
    GPD_Point		*pt;
    GPD_Point		**ptlist;

    // Realloc the point list array.
    ptlist = new GPD_Point *[fileNpt+1];
    if (points)
    {
	memcpy(ptlist, points, fileNpt * sizeof(GPD_Point *));
	delete [] points;
    }
    points = ptlist;

    pt = new GPD_Point;
    pt->setNum(fileNpt);
    ptlist[fileNpt] = pt;
    fileNpt++;

    return pt;
}

GPD_Primitive *
GPD_Detail::buildPrimitive(unsigned int type)
{
    GPD_Primitive	*prim;
    GPD_Primitive	**primlist;

    // Realloc the primlist array...
    primlist = new GPD_Primitive *[fileNprim+1];
    if (prims)
    {
    	memcpy(primlist, prims, fileNprim*sizeof(GPD_Primitive *));
	delete [] prims;
    }
    prims = primlist;

    prim = createPrimitive(type, fileNprim);
    prims[fileNprim] = prim;
    fileNprim++;

    return prim;
}

GPD_Primitive *
GPD_Detail::createPrimitive(unsigned int type, int idx)
{
    GPD_Primitive *prim;

    switch(type)
    {
	case GPDPRIMPOLY:    prim = (GPD_Primitive*)new GPD_PrimPoly(this);
			     break;
	case GPDPRIMCIRCLE:  prim = (GPD_Primitive*)new GPD_PrimCircle(this);
			     break;
	case GPDPRIMMESH:    prim = (GPD_Primitive*)new GPD_PrimMesh(this); 
			     break;
	case GPDPRIMBEZCURVE:prim = (GPD_Primitive*)new GPD_PrimRBezCurve(this);
			     break;
	case GPDPRIMNURBCURVE:prim= (GPD_Primitive*)new GPD_PrimNURBCurve(this);
			     break;
	case GPDPRIMBEZSURF: prim = (GPD_Primitive*)new GPD_PrimRBezSurf(this); 
			     break;
	case GPDPRIMNURBSURF:prim = (GPD_Primitive*)new GPD_PrimNURBSurf(this); 
			     break;
	case GPDPRIMSPHERE:  prim = (GPD_Primitive*)new GPD_PrimSphere(this); 
			     break;
	case GPDPRIMTUBE:    prim = (GPD_Primitive*)new GPD_PrimTube(this); 
			     break;
	case GPDPRIMPART:    prim = (GPD_Primitive*)new GPD_PrimParticle(this); 
			     break;
	case GPDPRIMMETABALL:prim = (GPD_Primitive*)new GPD_PrimMetaBall(this);
			     break;
	case GPDPRIMMETASQUAD:prim= (GPD_Primitive*)new GPD_PrimMetaSQuad(this);
			     break;
	case GPDPRIMTRIFAN:  prim = (GPD_Primitive*)new GPD_PrimTriFan(this);
			     break;
	case GPDPRIMTRISTRIP:prim = (GPD_Primitive*)new GPD_PrimTriStrip(this);
			     break;
	default:	     prim = 0; 
			     break;
    }
    prim->num = idx;
    primAttrib.useDefaults(prim->attribData, 1);
    return prim;
}

int
GPD_Detail::loadVersion(istream &is, int binary)
{
    char vname;

    if (binary)
    {
	if (!UTread(is, &vname)) return -1;
	if (vname == versionName)
	{
	    if (!UTread(is, &version)) return -1;
	}
	else
	{
	    if (!is.putback(vname)) return -1;
	    version = 0;
	}
    }
    else
    {
	char		*ver;
	char		 tmpbuf[GPD_BUFSIZ];

	is >> tmpbuf; if (!is) return -1; // This reads in "METRY"

	// Read in the rest of the line
	if (!is.getline(tmpbuf, GPD_BUFSIZ-1)) return -1;

	ver = tmpbuf;
	while (*ver && isspace(*ver))
	    ver++;

	if (ver[0] == versionName)
	    version = atoi(&ver[1]);
	else
	    version = 0; // initial version
    }

    if (version < 0) version = 0;

    return 0;
}


#ifdef ERROR
void
GPD_Detail::error(GPD_ERROR errno, const char *errmsg)
{
    geoError = errno;
    if (errno == GPD_ERR_NONE)  geoLocalError.harden("");
    else                        geoLocalError.harden(errmsg);
}

void
GPD_Detail::error(ostream &os) const
{
    os << geoGetErrorMessage(geoError);
    if (geoLocalError.isstring()) os << " " << geoLocalError;
}

#endif

