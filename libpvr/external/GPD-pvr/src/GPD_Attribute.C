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
 * GPD_Attribute.C
 *
 */



#include <iostream>
#include "GPD_Attribute.h"
//#include <stdio.h> // defines BUFSIZ
#include <string.h>
#include <ctype.h>
#include "GPD_Detail.h"

static const char GPD_AttribDelim[3][2] = {{	'(', ')'	},
					  {	'[', ']'	},
					  {	'{', '}'	},
					 };



//---------------- GPD_Attribute methods -----------------
GPD_Attribute::GPD_Attribute(const char *n, int s, GPD_AttribType t, 
			   const void *def)
: GPD_LinkNode()
{
    name = strdup(n);
    type = t;
    size = s;
    if (type == GPD_ATTRIB_INDEX)  size = sizeof(int);
    if (type == GPD_ATTRIB_VECTOR) size = sizeof(float)*3;
    if (type == GPD_ATTRIB_INDEX)
    {
	int		defInt = -1;

	size = sizeof(int);
	defValue = malloc(size);
	memcpy(defValue, (const char *)&defInt, sizeof(int));
	indexArray = 0;
	indexSize  = 0;
    }
    else
    {
	if ((size & 3) != 0) size += 4-(size & 3);	// Ensure word alignment
	defValue = malloc(s);

	if (def) memcpy(defValue, def, s);
	else	 memset(defValue, 0, s);
    }
}

GPD_Attribute::~GPD_Attribute()
{
    if (name)		free(name); // not "delete" because strdup calls malloc.
    if (defValue)	free(defValue);
    if (type == GPD_ATTRIB_INDEX)
    {
	int		i;

	for (i = 0; i < indexSize; i++)
	    free(indexArray[i]);
	if (indexArray) free(indexArray);
    }
}

int
GPD_Attribute::getIndex(const char *str) const
{
    int		i;

    for (i = 0; i < indexSize; i++)
    {
	if (!strcmp(str, indexArray[i]))
	    return i;
    }
    // Not found.
    return -1;
}

int
GPD_Attribute::addIndex(const char *str)
{
    int		 i;
    char	**newarray;

    i = getIndex(str);
    if (i >= 0) return i;	// Already present, don't need to add.

    // Add to the end of our list...
    i = indexSize++;

    if (indexArray)
	indexArray = (char **)realloc(indexArray, sizeof(char *)*indexSize);
    else indexArray = (char **)calloc(sizeof(char *), indexSize);
    
    indexArray[i] = strdup(str);
    
    return i;
}

int
GPD_Attribute::saveIndexValues(ostream &os, int binary) const
{
    int		i;
    char       *tmp; 

    if (binary)
    {
	UTwrite(os, &indexSize);
	for (i = 0; i < indexSize; i++)
	{
	    tmp = indexArray[i];
	    unsigned short len = strlen(tmp);
	    if (!UTwrite(os, &len)) return -1;
	    if (!UTwrite(os, tmp, len)) return -1;
	}
    }
    else
    {
	os << " " << indexSize;
	for (i = 0; i < indexSize; i++)
	{
	    tmp = indexArray[i];
	    os << " " << tmp;
	}
    }
    return (!os) ? -1 : 0;
}

int
GPD_Attribute::loadIndexValues(istream &is, int binary)
{
    static const char *protectedChars = "\"\\";

    int		i, sz;
    char       *tmp;  //was previosly a UT_String

    // Load the number of entries in the index
    if (binary) UTread(is, &sz);
    else	is >> sz;

    // Load each index entry
    for (i = 0; i < sz; i++)
    {
	if (binary)
	{
	    unsigned short len;
	    if (!UTread(is, &len)) return -1;
	    tmp = (char*)malloc((unsigned)len+1);
	    if (!UTread(is, tmp, len)) return -1;
	    tmp[len] = 0;
	}
	else
	{
	    int len;

	    char tmpbuf[GPD_BUFSIZ];
	    is >> tmpbuf[0];
	    int doQuote = (tmpbuf[0] == '"') ? 1 : 0;
	    for (len = doQuote ? 0 : 1; len < GPD_BUFSIZ -1; len++)
	    {
		UTread(is, &tmpbuf[len]);
		if (doQuote)
		{
		    if (tmpbuf[len] == '"') break;
		}
		else
		{
		    if (isspace(tmpbuf[len]))
		    {
			is.putback(tmpbuf[len]);
			break;
		    }
		}
		if (!is) break;

		if (tmpbuf[len] == '\\')
		    if (strchr(protectedChars, is.peek()))
			UTread(is, &tmpbuf[len]);
	    }
	    tmpbuf[len] = 0;
	    tmp = strdup(tmpbuf);
	}
	indexSize++;
	if (indexArray)
	    indexArray = (char **)realloc(indexArray, sizeof(char *)*indexSize);
	else indexArray = (char **)calloc(sizeof(char *), indexSize);
	indexArray[i] = tmp;
    }
    return (!is) ? -1 : 0;
}


//---------------- GPD_AttributeDict methods -----------------
GPD_AttributeDict::GPD_AttributeDict() 
: GPD_LinkList() 
{ 
    usedSize = allocSize = 0; 
}

GPD_AttributeDict::~GPD_AttributeDict()
{
}

GPD_Attribute *
GPD_AttributeDict::find(const char *name, int size, GPD_AttribType type) const
{
    for (GPD_Attribute *atr = (GPD_Attribute*)head(); atr; 
		       atr = (GPD_Attribute*)atr->next())
	if (atr->getSize() == size && atr->getType() == type &&
	    !strcmp(atr->getName(), name)) return atr;
    return 0;
}

int
GPD_AttributeDict::getOffset(const char *name,  int size, 
						GPD_AttribType type)const
{
    int offset = 0;
    for (GPD_Attribute *atr = getHead(); atr; atr = (GPD_Attribute*)atr->next())
    {
	if (atr->getSize() == size && atr->getType() == type &&
	    !strcmp(atr->getName(), name)) return offset;
	offset += atr->getSize();
    }
    return -1;
}

GPD_Attribute *
GPD_AttributeDict::append(const char *n, int s, GPD_AttribType type,  
                         const void *defValue, int *offset)
{
    if (find(n, s, type))
    {
        *offset = getOffset(n, s, type);
	return 0;
    }

    GPD_Attribute *atr = new GPD_Attribute(n, s, type, defValue);
    GPD_LinkList::append(atr);
    usedSize += s;
    return atr;
}

int
GPD_AttributeDict::save(ostream &os, const GPD_AttributeData &data,
		       int binary, char attrDelim) const
{
    char endCh;
    if (!binary)
    {
	if   (attrDelim==GPD_AttribDelim[0][0]) endCh=GPD_AttribDelim[0][1];
        else if (attrDelim==GPD_AttribDelim[1][0]) endCh=GPD_AttribDelim[1][1];
	else if (attrDelim==GPD_AttribDelim[2][0]) endCh=GPD_AttribDelim[2][1];
	else return -1;
    }

    int			 i, n = 0;
    char		 ch = attrDelim;
    GPD_Attribute	*atr = getHead();
    GPD_AttribType	 lastType = GPD_ATTRIB_MIXED;

    if (!atr) return 0;

    if (!binary) os << ' ';		// White space separation
    while (atr)
    {
	i = saveData(os, data[n], atr->getSize(), atr->getType(), binary, ch);
	if (i < 0) return -1;
	if (atr->getType() != GPD_ATTRIB_MIXED) lastType = atr->getType();
	ch = ' ';		// Clear out the open brace
	n += i;
	atr = (GPD_Attribute *)atr->next();
    }
    if (!binary && lastType != GPD_ATTRIB_MIXED)
    {
	// We have to be clever here and put out a ' ' after a string!
	if (lastType == GPD_ATTRIB_STRING) os << ' ';
	os << endCh;
    }
    return 0;
}

void          
GPD_AttributeDict::useDefaults(GPD_AttributeData &adata, int alloc) const
{
    if (alloc) adata.changeSize(allocSize);

    int size = 0;
    for (GPD_Attribute *a = getHead(); a; a = (GPD_Attribute*)a->next())
    {
	memcpy(adata.getData(size), a->getDefault(), a->getSize());
	size += a->getSize();
    }
}

int
GPD_AttributeDict::load(istream &is, GPD_AttributeData &data, int binary)
{
    int			 n = 0;
    char		 ch;
    GPD_Attribute	*atr = getHead();

    if (!atr) return 0;

    char endChar;
    if (!binary)
    {
	do {
	    ch = is.get();
	    if (!is) return -1;
	} while (ch != GPD_AttribDelim[0][0] && ch != GPD_AttribDelim[1][0]
					    && ch != GPD_AttribDelim[2][0]);

	if      (ch == GPD_AttribDelim[0][0]) endChar=GPD_AttribDelim[0][1];
	else if (ch == GPD_AttribDelim[1][0]) endChar=GPD_AttribDelim[1][1];
	else if (ch == GPD_AttribDelim[2][0]) endChar=GPD_AttribDelim[2][1];
	else return -1;
    }

    while (atr)
    {
	int i = loadData(is, data[n], atr->getSize(), atr->getType(), binary);
	if (i < 0) return -1;
	n += i;
	atr = (GPD_Attribute *)atr->next();
    }

    if (!binary)
    {
	do {
	    ch = is.get();
	    if (!is) return -1;
	} while (ch != endChar);
    }

    return 0;
}

int                
GPD_AttributeDict::save(ostream &os, const char *label) const
{
    short		 blen, size, ssize;
    GPD_AttribType	 type;
    GPD_Attribute	*atr;
    UT_EnumType		 temp;

    if (!label)
    {
	for (atr = getHead(); atr; atr = (GPD_Attribute*)atr->next())
	{
	    type = atr->getType();
	    if (type == GPD_ATTRIB_MIXED) continue;

	    size = atr->getSize();
	    switch (type)
	    {
		case GPD_ATTRIB_FLOAT:
		case GPD_ATTRIB_VECTOR:	ssize = size/sizeof(float); break;
		case GPD_ATTRIB_INT:	ssize = size/sizeof(int); break;
		case GPD_ATTRIB_STRING: ssize = size/sizeof(char); break;
		case GPD_ATTRIB_INDEX:  ssize = 1; break;
		default: break;
	    }

	    blen = strlen(atr->getName());
	    if (!UTwrite(os, &blen))   return -1;
	    if (!UTwrite(os, atr->getName(), blen))	return -1;
	    if (!UTwrite(os, &ssize))	return -1;
	    temp = (UT_EnumType) type;
	    if (!UTwrite(os, &temp)) return -1;
	    if (type == GPD_ATTRIB_INDEX)
	    {
		if (atr->saveIndexValues(os, 1) < 0)
		    return -1;
	    }
	    else
	    {
		if (saveData(os, atr->getDefault(), size, type, 1) < 0)
		    return -1;
	    }
	}
    }
    else
    {
	unsigned short savedLabel = 0;
	for (atr = getHead(); atr; atr = (GPD_Attribute*)atr->next())
	{
	    type = atr->getType();
	    if (type == GPD_ATTRIB_MIXED) continue;
	    if (!savedLabel)
	    {
	        os << label << endl; // save the label only if not only mixed
		savedLabel = 1;
	    }
	    size = atr->getSize();
	    switch (type)
	    {
		case GPD_ATTRIB_FLOAT:
		case GPD_ATTRIB_VECTOR:	ssize = size/sizeof(float); break;
		case GPD_ATTRIB_INT:	ssize = size/sizeof(int); break;
		case GPD_ATTRIB_STRING:	ssize = size/sizeof(char); break;
		case GPD_ATTRIB_INDEX:  ssize = 1;
		default:		break;
	    }

	    os << atr->getName() << " " << ssize << " ";
	    switch (type)
	    {
		case  GPD_ATTRIB_FLOAT:	os << "float"; break;
		case GPD_ATTRIB_VECTOR: os << "vector"; break;
		case    GPD_ATTRIB_INT:	os << "int";   break;
		case GPD_ATTRIB_STRING:	os << "char";  break;
		case  GPD_ATTRIB_INDEX:	os << "index"; break;
		case  GPD_ATTRIB_MIXED:	break;
	    }
	    if (!os) return -1;
	    if (type == GPD_ATTRIB_INDEX)
	    {
		if (atr->saveIndexValues(os, 0) < 0)
		    return -1;
	    }
	    else
	    {
		saveData(os, atr->getDefault(), size, type, 0);
	    }
	    os << endl;
	}
    }
    return (!os) ? -1 : 0;
}

GPD_Attribute * 
GPD_AttributeDict::load(istream &is, int tblsize, int binary)
{
    char		 name[256];
    char		 tmpbuf[GPD_BUFSIZ];
    GPD_Attribute	*atr;
    GPD_AttribType	 type;
    short		 blen, ssize;
    int			 size;
    UT_EnumType		 temp;

    if (!binary)
    {
	do {
	    if (!is.getline(name, 255)) return 0;
	} while (strlen(name) < 2);
    }

    // TODO: The binary comparison should be taken outside the loop
    for (int cnt = 0; cnt < tblsize; cnt++)
    {
	if (binary)
	{
	    if (!UTread(is, &blen))		goto rerror;
	    if (!UTread(is, name, blen))	goto rerror;
	    if (!UTread(is, &ssize))		goto rerror;
	    if (!UTread(is, &temp))		goto rerror;
	    type = (GPD_AttribType)temp;
	    name[blen] = 0;
	    size = ssize;
	}
	else
	{
	    char	stype[64];

	    is >> name >> size >> stype;
	    if (!is) goto rerror;

	         if (strcmp(stype, "float") == 0) type = GPD_ATTRIB_FLOAT;
	    else if (strcmp(stype, "vector") == 0)type = GPD_ATTRIB_VECTOR;
	    else if (strcmp(stype, "int")   == 0) type = GPD_ATTRIB_INT;
	    else if (strcmp(stype, "char")  == 0) type = GPD_ATTRIB_STRING;
	    else if (strcmp(stype, "index") == 0) type = GPD_ATTRIB_INDEX;
	    else goto rerror;
	}

	switch (type)
	{
	    case GPD_ATTRIB_FLOAT:
	    case GPD_ATTRIB_VECTOR:	size *= sizeof(float); break;
	    case GPD_ATTRIB_INT:	size *= sizeof(int); break;
	    case GPD_ATTRIB_STRING:	size *= sizeof(char); break;
	    case GPD_ATTRIB_INDEX:	if (size != 1) goto rerror;
					size = sizeof(int);
					break;
	    default: goto rerror;
	}

	if (type == GPD_ATTRIB_INDEX)
	{
    	    atr = new GPD_Attribute(name, size, type, (const void *)tmpbuf);
	    if (atr->loadIndexValues(is, binary) < 0)
		goto rerror;
	    GPD_LinkList::append(atr);
	}
	else
	{
	    if (loadData(is, (void *)tmpbuf, size, type, binary) < 0)
		goto rerror;
	    GPD_LinkList::append(new GPD_Attribute(name, size, type, (const void *)tmpbuf));
	}
    }
    usedSize	= 0;
    atr		= getHead();
    while (atr)
    {
	usedSize += atr->getSize();
	atr       = (GPD_Attribute*)atr->next();
    }
    allocSize = usedSize;

    return getHead();

rerror:		// Read error
    return 0;
}

int     
GPD_AttributeDict::saveData(ostream &os, const void *d, int size, int type, 
		           int binary, char leadChar) const
{
    if (type == GPD_ATTRIB_MIXED) return size;

    if (binary)
    {
        switch( type )
        {
            case GPD_ATTRIB_FLOAT:
	    case GPD_ATTRIB_VECTOR:
                if (!UTwrite(os, (const float *)d, size/sizeof(float)))
		    return -1;
                break;

            case GPD_ATTRIB_INT:
                if (!UTwrite(os, (const int *)d, size/sizeof(int))) return -1;
                break;

            case GPD_ATTRIB_STRING:
                if (!UTwrite(os, (const char *)d, size)) return -1;
                break;

	        case GPD_ATTRIB_INDEX:
                if (!UTwrite(os, (const int *)d, size/sizeof(int))) return -1;
		        break;

	    default: break;
        }
    }
    else
    {
        int		 i, first = 1;
	const float	*fptr;
	const int	*iptr;

	os << leadChar;
        switch (type)
        {
            case GPD_ATTRIB_FLOAT:
	    case GPD_ATTRIB_VECTOR:
                fptr = (const float *)d;
                for (i = size/sizeof(float); i > 0; i--, fptr++)
		{
		    if (first)	first = 0;
		    else	os << ' ';
                    os << *fptr;
		}
                break;

            case GPD_ATTRIB_INT:
                iptr = (const int *)d;
                for (i = size/sizeof(int); i > 0; i--, iptr++)
		{
		    if (first)	first = 0;
		    else	os << ' ';
                    os << *iptr;
		}
                break;

            case GPD_ATTRIB_STRING:
                os << (const char *)d;
                break;

	    case GPD_ATTRIB_INDEX:
		iptr = (const int *)d;
		os << *iptr;
		break;

	    default: break;
        }
    }
    return (!os) ? -1 : size;
}

int
GPD_AttributeDict::loadData(istream &is, void *d, int size, int type,
			    int binary)
{
    if (type == GPD_ATTRIB_MIXED) return size;

    if (binary)
    {
        switch( type )
        {
            case GPD_ATTRIB_FLOAT:
	    case GPD_ATTRIB_VECTOR:
                if (!UTread(is, (float *)d, size/sizeof(float))) return -1;
                break;
            case GPD_ATTRIB_INT:
                if (!UTread(is, (int *)d, size/sizeof(int))) return -1;
                break;

            case GPD_ATTRIB_STRING:
                if (!UTread(is, (char *)d, size)) return -1;
                break;

	        case GPD_ATTRIB_INDEX:
                if (!UTread(is, (int *)d, size/sizeof(int))) return -1;
		        break;

	    default: break;
        }
    }
    else
    {
        int	 i;
        int	*iptr;
        float	*fptr;
	char	 tmpbuf[GPD_BUFSIZ];

        switch (type)
        {
            case GPD_ATTRIB_FLOAT:
	    case GPD_ATTRIB_VECTOR:
                fptr = (float *)d;
                for (i = size/sizeof(float); i > 0; i--, fptr++)
                    is >> *fptr;
                break;

            case GPD_ATTRIB_INT:
                iptr = (int *)d;
                for (i = size/sizeof(int); i > 0; i--, iptr++)
                    is >> *iptr;
                break;

            case GPD_ATTRIB_STRING:
		is >> tmpbuf; if (!is) return -1;
                if (strlen(tmpbuf) >= size)  return -1;
                strcpy((char *)d, tmpbuf);
                break;

	    case GPD_ATTRIB_INDEX:
		iptr = (int *)d;
		is >> *iptr;
		break;

	    default: break;
        }
    }
    return (!is) ? -1 : size;
}

//---------------- GPD_AttributeTable methods -----------------
GPD_AttributeTable::~GPD_AttributeTable()
{
}

int
GPD_AttributeTable::append(const char *n, int s, GPD_AttribType type,
                          const void *defValue)
{
    int offset;
    if (GPD_AttributeDict::append(n, s, type, defValue, &offset) == 0)
        return offset;

    if (defValue)
	memcpy(attribData[getSize()-s], defValue, s);
    else
	memset(attribData[getSize()-s], 0, s);

    return getSize()-s;
}

