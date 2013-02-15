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
 * GPD_Detail.h
 *
 */



#ifndef __GPD_Detail_h__
#define __GPD_Detail_h__

#include "GPD_Util.h"
#include "GPD_StreamUtil.h"
#include "GPD_Attribute.h"
#include "GPD_Point.h"
#include "GPD_Primitive.h"
#include "GPD_Group.h"

class GPD_Vertex;

#define STDIN_BINARY		"stdin.bgeo"
#define STDIN_ASCII		"stdin.geo"
#define STDIN_AUTO		"stdin"
#define STDOUT_BINARY		"stdout.bgeo"
#define STDOUT_ASCII		"stdout.geo"
#define STDOUT_AUTO		"stdout"

#include <iosfwd>
using namespace std;

class 
#ifdef GPD_EXPORTS
__declspec(dllexport)
#endif
GPD_Detail {
public:
    GPD_Detail();
    virtual ~GPD_Detail();

    //
    //  Stream I/O METHODS
    //
    int		 save(ostream& os) const;	 // Save given ostream
    int		 save(const char *) const;	 // Save given a file name
    int		 load(istream &is);
    int		 load(const char *);

    //
    //		These use the Ascii flag to determine whether to save
    //		as binary or as ascii.
    //
    int		 saveVertex(ostream &os, const GPD_Vertex &vtx) const;
    int		 loadVertex(istream &is, GPD_Vertex &vtx);

    int		 loadBinaryPoint(istream &is, GPD_Point &ppt);
    int		 saveBinaryPoint(ostream &os, const GPD_Point &pt) const;
    int		 loadASCIIPoint (istream &is, GPD_Point &pt);
    int		 saveASCIIPoint (ostream &os, const GPD_Point &pt) const;


    GPD_AttributeDict	&pointAttribs()		{ return ptAttrib;   }
    GPD_AttributeDict	&primitiveAttribs()	{ return primAttrib;   }
    GPD_AttributeDict	&vertexAttribs()	{ return vtxAttrib;    }

    // Use these to create new geometry in a GPD.
    GPD_Point		*appendPoint();
    GPD_Primitive 	*buildPrimitive(unsigned int type);

    // Use these methods to query a built GPD
    GPD_Point		*point(int idx)		{ return points[idx]; }
    GPD_Primitive	*prim(int idx)		{ return prims[idx]; }
    int			 numpoint() const	{ return fileNpt; }
    int			 numprim() const	{ return fileNprim; }

    // Query or set the ascii flag to 0 or 1.
    int 	ascii(void) const	{ return asciiFlag;  }
    void	ascii(int a)		{ asciiFlag = a; }

    int		getVersion(void)	{ return version; }

protected:

//
//  Renumber the lists, optionally, the renumbering can start at the
//  point/primitive specified.
//
    void		   renumberPoints(int start = 0);
    void		   renumberPrimitives(int start = 0);

private:
    int			  checkMagic(istream &is) const;
    int			  asciiMagic(void) const
			  {
			      return (('P'<<24) + ('G'<<16) + ('E'<<8) + 'O');
			  }
    int			  binaryMagic(void) const
			  {
                              return (('B'<<24) + ('g'<<16) + ('e'<<8) + 'o');
			  }

    int			 loadVersion(istream &is, int binary);

    GPD_Primitive	*createPrimitive(unsigned int type, int idx);

    int 				fileNpt, fileNprim;

    GPD_Point			      **points;	   // list of points
    GPD_PointGroupList 		        ptGroups;  // list of groups of points
    GPD_AttributeDict			ptAttrib;  // list of point attributes

    GPD_Primitive		      **prims;     // list of primitives
    GPD_PrimGroupList			primGroups;// list of primitive groups
    GPD_AttributeDict	 		primAttrib;// list of primitive attributes

    GPD_AttributeDict		vtxAttrib;	// list of vertex attributes

    GPD_AttributeTable 		detailAttrib;	// list of detail attribs & data

    int				asciiFlag;
    int				version;

    // Text constants used in the data file.
    static const char	 *nPoints;
    static const char	 *nPrims;
    static const char	 *nPointGroups;
    static const char	 *nPrimGroups;
    static const char	 *nPointAttrib;
    static const char	 *nVertexAttrib;
    static const char	 *nPrimAttrib;
    static const char	 *nDetailAttrib;
    static const char	 *beginExtra;
    static const char	 *endExtra;
    static const char	  beginExtraMagic;
    static const char	  endExtraMagic;

};

#endif
