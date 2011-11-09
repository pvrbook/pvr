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
 * GPD_Group.C
 *
 */


#include "GPD_Detail.h"
#include "GPD_Group.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(__APPLE__)
#  include <stdlib.h>
#else
#  include <malloc.h>
#endif


typedef unsigned int    uint;

#define BITS_PER_WORD	(sizeof(uint) * 8)

short	GPD_Group::wordShift = 0;
short	GPD_Group::wordMask = 0;

static inline int
numWords(int size)
{
    return (size + (BITS_PER_WORD - 1)) / BITS_PER_WORD;
}

//
// ------------- GPD_GroupList methods -----------------
//
GPD_GroupList::~GPD_GroupList()
{
}

GPD_Group *
GPD_GroupList::newGroup(istream &is, int binary)
{
    GPD_Group *grp = new GPD_Group();

    if (grp->load(is, binary) < 0)
	return 0;

    append(grp);
    return grp;
}

int
GPD_GroupList::save(ostream &os, int binary) const
{
    for (GPD_Group *grp = head(); grp; grp = grp->next())
	if (grp->save(os, binary) < 0) return -1;
    return 0;
}

int
GPD_GroupList::load(istream &is, int howMany, int binary)
{
    GPD_Group *grp;

    for (int i = 0; i < howMany; i++)
    {
	grp = newGroup(is, binary);
	if (!grp) return -1;
    }
    return 0;
}

//
// ------------- GPD_Group methods -----------------
//
GPD_Group::~GPD_Group()
{
    if (name)  free(name);  // not "delete" because strdup calls malloc

    delete[] ordered;
    delete[] profiles;
    delete[] flags;
}

GPD_Group::GPD_Group(void)
{
    name = 0;
    numPts = 0;
    flags = 0;
    ordered = 0;
    nordered = 0;

    setWordMask();
}

int
GPD_Group::save(ostream &os, int binary) const
{
    int i;

    if (binary)
    { 
	if (nordered) os.put(1);

	if (binary < 3) // before version 2
	{
	    os << name << endl;
	}
	else
	{
	    unsigned short len;
	    len = (unsigned short)strlen(name);
	    UTwrite(os, &len);
	    UTwrite(os, name, len);
	}

	int nbits = numPts;
	if (!UTwrite(os, &nbits)) return -1;

	int	 nwords = numWords(nbits);
	uint	*bits   = (uint*)calloc(sizeof(uint), nwords);
	uint	*word;
	uint	 mask;

	for (i = 0; i < nbits; i++)
	{
	    word = bits + (i >> wordShift);
	    mask = (1 << (i & wordMask));

	    if (flags[i]) *word |= mask;
	    else	  *word &= ~mask;
	}

	if (!UTwrite(os, bits, nwords)) return -1;

	free(bits);

	if (nordered)
	{
	    if (!UTwrite(os, &nordered)) return -1;

	    for (i=0; i<nordered; i++)
	    {
		if (profiles[i] >= 0)	// make the primary index negative
		{			// to signal a secondary component
		    int	neg = -ordered[i] -1;	// watch for zero!

		    if (!UTwrite(os, &neg))		return -1;
		    if (!UTwrite(os, &profiles[i]))	return -1;
		}
		else
		{
		    if (!UTwrite(os, &ordered[i]))	return -1;
		}
	    }
	}

    }

    else
    {
	os << name << ( nordered ? " ordered" : " unordered") << endl;
	os << numPts << " ";

	for (i = 0; i < numPts; i++)
	{
	    if (!os.put((flags[i] ? '1' : '0'))) return -1;
	    if (i != 0 && i % BITS_PER_LINE == 0 ) os << endl;
	}
	os << endl;

	if (nordered)
	{
	    os << nordered;
	    for (i=0; i<nordered; i++)
	    {
		os << " " << ordered[i];
		if (profiles[i]>=0)
		    os << "." << profiles[i];
	    }
	    os << endl;
	}
    }
    return 0;
}

#include <iomanip.h>	// needed for setw function


int
GPD_Group::load(istream &is, int binary)
{
    char	text[GPD_BUFSIZ];
    char	ordtext[GPD_BUFSIZ];
    char	onOff;
    int		i, isOrdered;
    int		nbits;

    if (binary)
    {
	// get ordered flag
        char    ordf = is.peek();       
        isOrdered = (ordf == 1);
        if (isOrdered && !is.get(ordf)) return -1;

	// get label
	if (binary < 3)
	{
	    if (!is.getline(text, GPD_BUFSIZ-1)) // '\n' delim is not incl
		return -1;
	}
	else
	{
	    unsigned short len;
	    if (!UTread(is, &len)) return -1;
	    if (!UTread(is, &text[0], len)) return -1;
	    text[len] = 0;
	}
	if (!UTread(is, &nbits)) return -1;

	initialize((const char*)&text[0], nbits);

	// get bitmask
	int	 nwords = numWords(nbits);
	uint	*bits   = (uint*)calloc(sizeof(uint), nwords);
	uint	*word;

	if (!UTread(is, bits, nwords)) return -1;

	for (i = 0; i < nbits; i++)
	{
	    word = bits + (i >> wordShift);
	    flags[i] = ((*word & (1 << (i & wordMask))) != 0);
	}

	free(bits);

	// get ordered info
	if (isOrdered)
	{
	    if (!UTread(is, &nordered)) return -1;
	    for (i=0; i<nordered; i++)
	    {
		if (!UTread(is, &ordered[i])) return -1;
		if (ordered[i] < 0)
		{
		    ordered[i] = -ordered[i] - 1;
		    if (!UTread(is, &profiles[i])) return -1;
		}
		else
		    profiles[i] = -1;
	    }
	}
	else nordered = 0;
    }

    else
    {
	char	ordc[2];
	int	keyword;

	// get label
        if (!(is >> setw(GPD_BUFSIZ) >> text)) return -1;

	// get ordered keyword
        // read until start of next keyword (ie cant putback whole word!)

        if (!(is >> setw(2) >> ordc)) return -1;

        switch(ordc[0])
        {
            case 'o':
                isOrdered = 1;
                keyword = 1;
                break;

            case 'u':
                isOrdered = 0;
                keyword = 1;
                break;

            default:            // numeric data for bitarray
                isOrdered = 0;
                keyword = 0;
                break;
        }

        if (!is.putback(ordc[0])) return -1;

        if (keyword)    // read entire keyword from stream
        {
            if (!(is >> setw(GPD_BUFSIZ) >> ordtext)) return -1;
        }

	// read bit mask

	if (!(is >> nbits)) return -1;

	initialize((const char *)&text[0], nbits);

	i = 0;
	while (i < numPts)
	{
	    if (!is.get(onOff)) return -1;
	    if (onOff == ' ' || onOff == '\n' || onOff == '\r') continue;
	    switch (onOff)
	    {
		case '1':	flags[i] = 1; break;
		case '0':	flags[i] = 0; break;
		default :	return -1;
	    }
	    i++;
	}

	if (isOrdered)
	{
	    char	prof_char;

	    if (!(is >> nordered)) return -1;
	    for (i=0; i<nordered; i++)
	    {
		if (!(is >> ordered[i])) return -1;

		// check if there's a secondary component
		prof_char = is.peek();
		if (prof_char == '.')
		{
		    is.get(prof_char);
		    if (!(is >> profiles[i])) return -1;
		}
		else
		    profiles[i] = -1;
	    }

	} else nordered = 0;

	//ensure we skip to the next line
	for (;;)
	{
	    is.get(onOff);
	    if (onOff == '\n' || onOff == '\r') break;
	}
    }

    return 0;
}

void
GPD_Group::setWordMask()	// Pseudo machine independent mask and shift values:
{
    if( wordShift == 0 )
    {
	for( wordShift = wordMask = 1; 
	     (1 << wordShift) < BITS_PER_WORD; 
	     ++wordShift )
	{
	    wordMask |= (1 << wordShift);
	}
    }
}

void
GPD_Group::initialize(const char *n, int npt)
{
    int i;

    if (name) free(name);
    name  = n ? strdup(n) : 0;

    numPts = npt;
    flags = new char[npt];
    for (i=0; i<npt; i++)
	flags[i] = 0;

    ordered = new int[npt];	// just set to max
    for (i=0; i<npt; i++)
	ordered[i] = i;

    profiles = new int[npt];
    for (i=0; i<npt; i++)	// turn them all off
	profiles[i] = -1;
}

