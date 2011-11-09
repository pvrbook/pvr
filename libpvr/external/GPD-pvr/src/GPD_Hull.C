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
 * GPD_Hull.C
 *
 */


#include "GPD_Detail.h"
#include "GPD_Hull.h"

GPD_Hull::GPD_Hull(GPD_Detail *d) : GPD_Primitive(d) 
{
    vtxMatx = 0;
}

GPD_Hull::~GPD_Hull() 
{
    delete[] vtxMatx;
}

int
GPD_Hull::save(ostream &os, int binary) const
{
    int		row, col;
    int		temp;
    unsigned	flags = 0;

    if (binary)
    {
	if (isWrappedU()) flags |= 0x80000000;
	if (isWrappedV()) flags |= 0x40000000;

	temp = (UT_EnumType) surfaceType;

	if (!UTwrite(os, &nCols))	return -1;
	if (!UTwrite(os, &nRows))	return -1;
	if (!UTwrite(os, &flags))	return -1;
	if (!UTwrite(os, &temp))	return -1;
    }
    else
    {
	os << " ";
	os << nCols << " " << nRows << " " << (isWrappedU() ? "wrap" : "open");
	os << " " << (isWrappedV() ? "wrap" : "open") << " ";
	if (!os) return -1;
	switch (surfaceType)
	{
	    case GPD_PATCH_ROWS:	os << "rows";	break;
	    case GPD_PATCH_COLS:	os << "cols";	break;
	    case GPD_PATCH_ROWCOL:	os << "rowcol";	break;
	    case GPD_PATCH_QUADS:	os << "quad";	break;
	    case GPD_PATCH_TRIANGLE:	os << "tri";	break;
	    case GPD_PATCH_ALTTRIANGLE:	os << "atri";	break;
	    case GPD_PATCH_REVTRIANGLE:	os << "revtri";	break;
	}
	os << endl; if (!os) return -1;
    }

    if (savePrivate(os, binary) < 0) return -1;

    for (row = 0; row < nRows; row++)
	for (col = 0; col < nCols; col++)
	    if (getParent()->saveVertex(os, vtxMatx[nCols*row + col]) < 0)
		return -1;

    if (getParent()->primitiveAttribs().save(os, attribData, binary, '[' ) < 0)
	return -1;

    if (!binary) os << endl; 

    if (saveExtra(os, binary) < 0) return -1;

    return (!os) ? -1 : 0;
}

int
GPD_Hull::load(istream &is, int binary)
{
    int		row, col;
    unsigned	flgs = 0;
    UT_EnumType	temp;

    if (binary)
    {
	if (!UTread(is, &nCols)) return -1;
	if (!UTread(is, &nRows)) return -1;
	if (!UTread(is, &flgs))	 return -1;
	if (!UTread(is, &temp))  return -1;

	if ((flgs & 0x80000000) != 0) GPD_Hull::wrapU();
	if ((flgs & 0x40000000) != 0) GPD_Hull::wrapV();

	surfaceType = (GPD_SurfaceType)temp;
    }
    else
    {
	char	wrapu[64], wrapv[64], ptype[64];

	is >> nCols >> nRows >> wrapu >> wrapv >> ptype;
	if (!is) return -1;
	     if (!strcmp(wrapu, "wrap")) GPD_Hull::wrapU();
	else if (!strcmp(wrapu, "open")) GPD_Hull::openU();
	else return -1;
	     if (!strcmp(wrapv, "wrap")) GPD_Hull::wrapV();
	else if (!strcmp(wrapv, "open")) GPD_Hull::openV();
	else return -1;

	     if (!strcmp(ptype, "rows")) surfaceType = GPD_PATCH_ROWS;
	else if (!strcmp(ptype, "cols")) surfaceType = GPD_PATCH_COLS;
	else if (!strcmp(ptype, "rowcol")) surfaceType = GPD_PATCH_ROWCOL;
	else if (!strcmp(ptype, "quad")) surfaceType = GPD_PATCH_QUADS;
	else if (!strcmp(ptype, "tri"))  surfaceType = GPD_PATCH_TRIANGLE;
	else if (!strcmp(ptype, "atri")) surfaceType = GPD_PATCH_ALTTRIANGLE;
	else if (!strcmp(ptype, "revtri")) surfaceType = GPD_PATCH_REVTRIANGLE;
	else return -1;
    }

    vtxMatx = new GPD_Vertex[nRows*nCols];
    for (int i=0; i < (nRows*nCols); i++)
	vtxMatx[i].initAttributeData(getParent());

    if (loadPrivate(is, binary) < 0) return -1;

    // Loading the Bez basis with breakpoints fails if we call ignore(): -CB
    // if (!binary) if (!is.ignore(GPD_BUFSIZ-1, '\n')) return -1;

    for (row = 0; row < nRows; row++)
	for (col = 0; col < nCols; col++)
	    if (getParent()->loadVertex(is, vtxMatx[nCols*row + col]) < 0)
		return -1;

    if (getParent()->primitiveAttribs().load(is, attribData, binary) < 0)
	return -1;

    // Get junk at end of line
    if (!binary) is.ignore(GPD_BUFSIZ-1, '\n');
    if (!is) return -1;

    if (loadExtra(is, binary) < 0) return -1;

    return (!is) ? -1 : 0;
}

void        
GPD_Hull::wrapU() 
{ 
    flags.wrapu = 1; 
}

void        
GPD_Hull::openU() 
{ 
    flags.wrapu = 0; 
}

void        
GPD_Hull::wrapV()
{ 
    flags.wrapv = 1; 
}

void        
GPD_Hull::openV() 
{ 
    flags.wrapv = 0; 
}

int
GPD_Hull::loadExtra(istream &, int)
{
    return 0;
}

int
GPD_Hull::saveExtra(ostream &, int) const
{
    return 0;
}

void
GPD_Hull::setSize(int rows, int cols, int appendpt)
{
    delete [] vtxMatx;

    nRows = rows;
    nCols = cols;
    
    vtxMatx = new GPD_Vertex[rows * cols];
    for (int i=0; i < (nRows*nCols); i++)
    {
	vtxMatx[i].initAttributeData(getParent());
	if (appendpt)
	    vtxMatx[i].setPt(getParent()->appendPoint());
    }
}

GPD_Vertex *
GPD_Hull::getVertex(int row, int col)
{
    return &vtxMatx[row * nCols + col];
}
