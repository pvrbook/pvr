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
 * GPD_Attribute.h
 *
 *	This contains the class definition for attributes.
 *	The size passed in is the size in bytes of the attribute.
 *	STRING types are stored as a fixed length array, not as pointers
 *	to strings.  However, the string must still be null terminated.
 *
 *      The classes defined here are:
 *	1. GPD_Attribute
 *	2. GPD_AttributeDict
 *	3. GPD_AttributeData
 *	4. GPD_AttributeTable (which contains a Dict and the Data)
 *
 *
 * COMMON ATTRIBUTE NAMES:
 *	N	3*float		Normals (point or primitive)
 *	Dclr	3*float		Display color (for wireframe)
 *	Ca	3*float		Ambient color
 *	Cd	3*float		Diffuse color
 *	Cs	3*float		Specular color
 *	Ct	3*float		Transmit color
 *	Ce	3*float		Emittence color
 *	Ct	1*float		Transparency
 *	Mat	32*char		Material reference (name of material)
 *	UV	2*float		Texture coordinates (tuple)
 *	UVW	3*float		Texture coordinates (triple)
 *	Dopt 	GEO_DrawOpt	Type of feedback, eg w/ or w/o hull, CVs, wire
 *	Lod	1*float		Level of detail
 *	v	3*float		Velocity of point/primitive
 *	Rest	3*float		Rest position for a point: X Y Z
 */

#ifndef __GPD_Attribute_h__
#define __GPD_Attribute_h__

#if defined(__APPLE__)
#  include <stdlib.h>
#else
#  include <malloc.h>
#endif
#include "GPD_LinkList.h"
//#include "GPD_Detail.h"

#include <iosfwd>
using namespace std;



//////////////////////////////////////////////////////////
//
//  Make sure that the attribute type ID's remain constant from compile
//	to compile since they are saved with the detail
//
enum GPD_AttribType {
	GPD_ATTRIB_FLOAT	= 0,
	GPD_ATTRIB_INT	= 1,
	GPD_ATTRIB_STRING	= 2,
	GPD_ATTRIB_MIXED	= 3,	// Impossible to save and load
	GPD_ATTRIB_INDEX	= 4,	// Index of strings
	GPD_ATTRIB_VECTOR	= 5,	// Vector of 3 floats - gets transformed
					//   when the detail is xformed.
};

//
// --------- GPD_Attribute ----------
//
class GPD_Attribute : public GPD_LinkNode {
public:
     GPD_Attribute(const char *n, int s, GPD_AttribType t, const void *def);
    ~GPD_Attribute();

    const char		*getName()	const { return name; } 
    int		 	 getSize()	const { return size; }
    GPD_AttribType	 getType()	const { return type; }
    const void		*getDefault()	const { return defValue; }

    // Gets the string from the index value.
    const char		*getIndex(int idx) const
			 {
			     return (idx < indexSize && idx >= 0) ?
				    indexArray[idx] : 0;
			 }
    // Gets the index value from a string.  Returns -1 if not present.
    int			 getIndex(const char *str) const;
    // Adds the index if not present, returns the index value.
    int			 addIndex(const char *str);

//
//  Methods to add/get the string associated with an index
    int			 saveIndexValues(ostream &os, int binary) const;
    int			 loadIndexValues(istream &is, int binary);

private:
    char		 *name;
    int			  size;
    GPD_AttribType	  type;
    void		 *defValue;	// Default value for attribute
    char		**indexArray;
    int			  indexSize;
};


//
// --------- GPD_AttributeData ----------
//
class GPD_AttributeData {
public:
    GPD_AttributeData() { data=0; }  
   ~GPD_AttributeData() { if (data) free(data); }

    //
    //	Get a pointer to the data desired
    //
    void	*getData() const { return data; } 
    void	*getData(int n) const { return (char *)data + n; } 

    void	 changeSize(int s)            
		 {
		    if (data)	data = realloc(data, s);
		    else	data = malloc(s);
		 }

    void	*operator[](unsigned n) const  
    {
	return (void *)((char *)data + n);
    }

private:
    void	*data;
};

//
// --------- GEO_AttributeDict ----------
// Attribute Dictionary class:
//
class GPD_AttributeDict : public GPD_LinkList {
public:
//
// Constructor and destructor:
     GPD_AttributeDict();
    ~GPD_AttributeDict();

//?    GEO_Attribute	*find(const char *name, int s, GEO_AttribType t) const;
    GPD_Attribute *getHead(void) const	{return (GPD_Attribute*)head();}
    int			 getSize(void) const	{ return usedSize;  }
    int			 getAlloc(void) const	{ return allocSize; }
    int			 getOffset(const char *name, int size, 
				   GPD_AttribType type) const;

//
//  I/O methods:
//  a) Save or load some attribute data (formerly saveAttribute())
//  The attribute delimiter must be one of '(', '[' and '{'.
    int	 save(ostream &os, const GPD_AttributeData &data, int binary,
				char attribDelim='(') const;
    int	 load(istream &is, GPD_AttributeData &data, int binary);

//
// b) Save or load the table (formerly saveAttributeTable())
    int			 save(ostream &os, const char *label) const;
    GPD_Attribute	*load(istream &is, int tblSize, int binary);

    GPD_Attribute *find(const char *name, int size, GPD_AttribType type) const;

    void useDefaults(GPD_AttributeData &adata, int alloc) const; 

    int numAttribs() const {return count;}

protected:
// Append an attribute and return a pointer to it if the operation was 
// necessary. If the pointer is zero, it means that an attribute was found
// and either it was OK (offset >= 0) or not (offset = -1).
    GPD_Attribute	*append(const char *n, int s, GPD_AttribType type, 
				const void *defValue, int *offset);

// The guts of the I/O methods:
    int		saveData(ostream &os, const void *d, int size, int type, 
		         int binary, char leadChar = ' ') const; 
    int		loadData(istream &is, void *d, int size, int type, int binary);

private:
    int		usedSize;	// size of attribute array being used
    int		allocSize;	// size of allocated array; allocSize>=usedSize
};


// --------- GEO_AttributeTable ----------
//
class GPD_AttributeTable : public GPD_AttributeDict {
public:
     GPD_AttributeTable()      {}
    ~GPD_AttributeTable();
    int          append(const char *n, int s, GPD_AttribType type,
                        const void *defValue);
    void         destroy(const char *name, int size, GPD_AttribType type);

    void	 changeDataSize(int s)	    { attribData.changeSize(s);}
    void	*getAttribData(int n) const { return attribData.getData(n); }

    void	 sortAs(const GPD_AttributeDict &table);

    // Save or load the attribute data (similar to saveAttribute())
    int		 save(ostream &os, int binary) const
		 {
		     return GPD_AttributeDict::save(os, attribData, binary); 
		 }
    int		 load(istream &is, int binary) 
		 {
		     return GPD_AttributeDict::load(is, attribData, binary); 
		 }

private:
    GPD_AttributeData            attribData;
};

#endif
