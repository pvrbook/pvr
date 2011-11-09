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
 * GPD_StreamUtil.C
 *
 */



#ifdef WIN32
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(__APPLE__)
#  include <stdlib.h>
#else
#  include <malloc.h>
#endif

#include "GPD_StreamUtil.h"

#ifdef SESI_LITTLE_ENDIAN

#ifdef WIN32
	typedef __int64		 INT64;
	typedef unsigned __int64 UINT64;
#else
	typedef long long	   INT64;
	typedef unsigned long long UINT64;
#endif

typedef short 		ut_2byte;
typedef int 		ut_4byte;
typedef INT64		ut_8byte;

#define swap2(a)  ((ut_2byte) ( (((a)&0xff00) >> 8) | (((a)&0x00ff) << 8) ) )
#define swap4(a)  ((ut_4byte) ( (((a)&0x000000ff) << 24) |    \
				(((a)&0x0000ff00) <<  8) |    \
				(((a)&0x00ff0000) >>  8) |    \
				(((a)&0xff000000) >> 24) ) )
#define swap8(a)  ((ut_8byte) ( (((a      ) & 0xff) << 56) | \
				(((a >>  8) & 0xff) << 48) | \
				(((a >> 16) & 0xff) << 40) | \
				(((a >> 24) & 0xff) << 32) | \
				(((a >> 32) & 0xff) << 24) | \
				(((a >> 40) & 0xff) << 16) | \
				(((a >> 48) & 0xff) <<  8) | \
				(((a >> 56) & 0xff)      )))

static char *
utGetBuffer( int size )
{
    static char *buffer = 0;
    static int currsize = 0;
    
    if( currsize < size ) {
	if( buffer ) free( buffer );
	currsize = size;
	buffer = (char *)malloc( size );
    }

    return buffer;
}

static istream &
utPortableRead(istream &is, char *dest, int numelem, int elemsize)
{
    int 	 size = numelem*elemsize;	    
    char	*buf = utGetBuffer(size);
    int		 i;

    ut_2byte	*src2byte, *dest2byte;
    ut_4byte	*src4byte, *dest4byte;
    ut_8byte	*src8byte, *dest8byte;

    switch(elemsize)
    {
	case 1:
	    is.read(dest, numelem);
	    break;

	case 2:
	    src2byte = (ut_2byte *)buf;
	    dest2byte = (ut_2byte *)dest;
	    if (!buf) return is.read(dest, size);
	    is.read(buf, size);
	    for (i = 0; i < numelem; i++)
		dest2byte[i] = swap2(src2byte[i]);
	    break;

	case 4:
	    src4byte = (ut_4byte *)buf;
	    dest4byte = (ut_4byte *)dest;
	    if (!buf) return is.read(dest, size);
	    is.read(buf, size);
	    for (i = 0; i < numelem; i++)
		dest4byte[i] = swap4(src4byte[i]);
	    break;

	case 8:
	    src8byte = (ut_8byte *)buf;
	    dest8byte = (ut_8byte *)dest;
	    if (!buf) return is.read(dest, size);
	    is.read(buf, size);
	    for (i = 0; i < numelem; i++)
		dest8byte[i] = swap8(src8byte[i]);
	    break;
    }

    return is;
}

static ostream &
utPortableWrite(ostream &os, const char *src, int numelem, int elemsize)
{
    int 	 size = numelem*elemsize;	    
    char	*buf = utGetBuffer(size);
    int		 i;

    ut_2byte	*src2byte, *dest2byte;
    ut_4byte	*src4byte, *dest4byte;
    ut_8byte	*src8byte, *dest8byte;

    switch(elemsize)
    {
	case 1:
	    os.write(src, numelem);
	    break;

	case 2:
	    src2byte = (ut_2byte *)src;
	    dest2byte = (ut_2byte *)buf;
	    if (!buf) return os.write(src, size);
	    for (i = 0; i < numelem; i++)
		dest2byte[i] = swap2(src2byte[i]);
	    os.write(buf, size);
	    break;

	case 4:
	    src4byte = (ut_4byte *)src;
	    dest4byte = (ut_4byte *)buf;
	    if (!buf) return os.write(src, size);
	    for (i = 0; i < numelem; i++)
		dest4byte[i] = swap4(src4byte[i]);
	    os.write(buf, size);
	    break;

	case 8:
	    src8byte = (ut_8byte *)src;
	    dest8byte = (ut_8byte *)buf;
	    if (!buf) return os.write(src, size);
	    for (i = 0; i < numelem; i++)
		dest8byte[i] = swap8(src8byte[i]);
	    os.write(buf, size);
	    break;
    }

    return os;
}

static int 
utPortableFRead(FILE *f, void *dest, int numelem, int elemsize)
{
    int 	 size = numelem*elemsize;	    
    char	*buf = utGetBuffer(size);
    int		 i, retval;

    ut_2byte	*src2byte, *dest2byte;
    ut_4byte	*src4byte, *dest4byte;
    ut_8byte	*src8byte, *dest8byte;

    switch(elemsize)
    {
	case 1:
	    retval = fread(dest, elemsize, numelem, f);
	    break;

	case 2:
	    src2byte = (ut_2byte *)buf;
	    dest2byte = (ut_2byte *)dest;
	    if (!buf) return fread(dest, elemsize, numelem, f);
	    retval = fread(buf, elemsize, numelem, f);
	    for (i = 0; i < numelem; i++)
		dest2byte[i] = swap2(src2byte[i]);
	    break;

	case 4:
	    src4byte = (ut_4byte *)buf;
	    dest4byte = (ut_4byte *)dest;
	    if (!buf) return fread(dest, elemsize, numelem, f);
	    retval = fread(buf, elemsize, numelem, f);
	    for (i = 0; i < numelem; i++)
		dest4byte[i] = swap4(src4byte[i]);
	    break;

	case 8:
	    src8byte = (ut_8byte *)buf;
	    dest8byte = (ut_8byte *)dest;
	    if (!buf) return fread(dest, elemsize, numelem, f);
	    retval = fread(buf, elemsize, numelem, f);
	    for (i = 0; i < numelem; i++)
		dest8byte[i] = swap8(src8byte[i]);
	    break;
    }

    return retval;
}

static int 
utPortableFWrite(FILE *f, const void *src, int numelem, int elemsize)
{
    int 	 size = numelem*elemsize;	    
    char	*buf = utGetBuffer(size);
    int		 i, retval;

    ut_2byte	*src2byte, *dest2byte;
    ut_4byte	*src4byte, *dest4byte;
    ut_8byte	*src8byte, *dest8byte;

    switch(elemsize)
    {
	case 1:
	    retval = fwrite(src, elemsize, numelem, f);
	    break;

	case 2:
	    src2byte = (ut_2byte *)src;
	    dest2byte = (ut_2byte *)buf;
	    if (!buf) return fwrite(src, elemsize, numelem, f);
	    for (i = 0; i < numelem; i++)
		dest2byte[i] = swap2(src2byte[i]);
	    retval = fwrite(buf, elemsize, numelem, f);	
	    break;

	case 4:
	    src4byte = (ut_4byte *)src;
	    dest4byte = (ut_4byte *)buf;
	    if (!buf) return fwrite(src, elemsize, numelem, f);
	    for (i = 0; i < numelem; i++)
		dest4byte[i] = swap4(src4byte[i]);
	    retval = fwrite(buf, elemsize, numelem, f);	
	    break;

	case 8:
	    src8byte = (ut_8byte *)src;
	    dest8byte = (ut_8byte *)buf;
	    if (!buf) return fwrite(src, elemsize, numelem, f);
	    for (i = 0; i < numelem; i++)
		dest8byte[i] = swap8(src8byte[i]);
	    retval = fwrite(buf, elemsize, numelem, f);	
	    break;
    }

    return retval;
}

#else

static inline istream &
utPortableRead(istream &is, char *dest, int numelem, int elemsize)
{ return is.read(dest, numelem*elemsize); }

static inline ostream &
utPortableWrite(ostream &os, const char *src, int numelem, int elemsize)
{ return os.write(src, numelem*elemsize); }

static inline int 
utPortableFRead(FILE *f, void *dest, int numelem, int elemsize)
{ return fread(dest, elemsize, numelem, f); }

static inline int 
utPortableFWrite(FILE *f, const void *dest, int numelem, int elemsize) 
{ return fwrite(dest, elemsize, numelem, f); }

#endif

#define DEFINE_PORTABLE_RW(type, size) \
istream &UTread( istream &is, type *dest, int numelem ) \
{ return utPortableRead( is, (char *)dest, numelem, size ); } \
ostream &UTwrite( ostream &os, const type *src, int numelem ) \
{ return utPortableWrite( os, (const char *)src, numelem, size ); } \
int UTfread( FILE *f, type *dest, int numelem ) \
{ return utPortableFRead( f, (void *)dest, numelem, size ); } \
int UTfwrite( FILE *f, const type *src, int numelem ) \
{ return utPortableFWrite( f, (const void *)src, numelem, size ); }

DEFINE_PORTABLE_RW(char, 1)
DEFINE_PORTABLE_RW(unsigned char, 1)
DEFINE_PORTABLE_RW(short, 2)
DEFINE_PORTABLE_RW(unsigned short, 2)
DEFINE_PORTABLE_RW(int, 4)
DEFINE_PORTABLE_RW(unsigned int, 4)
DEFINE_PORTABLE_RW(float, 4)
DEFINE_PORTABLE_RW(double, 8)
