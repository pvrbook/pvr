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
 * GPD_StreamUtil.h
 *
 */



#ifndef __GPD_StreamUtil_H__
#define __GPD_StreamUtil_H__

#include <stdio.h>

typedef int UT_EnumType;

#include <iosfwd>
using namespace std;

#ifdef WIN32
    #include <strstrea.h>
    #define NTBinaryMode ios::binary
#else
    #include <strstream>
    using namespace std;
    #define NTBinaryMode 0
#endif

#define DECLARE_PORTABLE_RW(lcltype) \
    istream &UTread( istream &is, lcltype *dest, int numelem = 1 ); \
    ostream &UTwrite( ostream &os, const lcltype *src, int numelem = 1); \
    int UTfread( FILE *f, lcltype *dest, int numelem = 1 ); \
    int UTfwrite( FILE *f, const lcltype *src, int numelem = 1 );

DECLARE_PORTABLE_RW(char)
DECLARE_PORTABLE_RW(unsigned char)
DECLARE_PORTABLE_RW(short)
DECLARE_PORTABLE_RW(unsigned short)
DECLARE_PORTABLE_RW(int)
DECLARE_PORTABLE_RW(unsigned int)
DECLARE_PORTABLE_RW(float)
DECLARE_PORTABLE_RW(double)

#endif 	// __GPD_StreamUtil_H__
