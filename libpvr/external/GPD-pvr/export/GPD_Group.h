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
 * GPD_Group.h
 *
 */



#ifndef __GPD_Group_h__
#define __GPD_Group_h__

#define BITS_PER_LINE	72

#include "GPD_LinkList.h"
#include <string.h>

class GPD_Group : public GPD_LinkNode {
public:
     // Constructor and destructor.
             GPD_Group(void);
    virtual ~GPD_Group();

//  Return the maximum number of entries in this group
    unsigned	 maxEntries(void) const { return numPts; } 

    //Query the name:
    char	*getName(void) const	{ return name; }

    // I/O methods:
    int		 save(ostream &os, int binary) const;
    int		 load(istream &is, int binary);

    	  GPD_Group	*next() {return (GPD_Group*)GPD_LinkNode::next();}
    const GPD_Group	*next() const
			 {
			     return (const GPD_Group*)GPD_LinkNode::next();
			 }


private:

    void 	 initialize(const char *n, int npt);
    void	 setWordMask(void);


    char  	*flags;			// Elements which belong
    int		*ordered;		// ordering information 
    int		*profiles;		// profile information

    char       	*name;			// Name of the group
    int 	numPts;

    int		nordered;		// number of ordered pts

    static short	wordShift;
    static short	wordMask;
};


class GPD_GroupList : public GPD_LinkList {
public:
    // Trivial constructor and destructor.
     GPD_GroupList()	{}
    ~GPD_GroupList();

    // Query and manipulation methods:
    GPD_Group	*newGroup(istream &is, int binary);
    GPD_Group  *head() const
			{
			    return (GPD_Group*)GPD_LinkList::head();
			}

    int numGroups() const { return count;}

    // I/O methods:
    int			 save(ostream &os, int binary) const;
    int			 load(istream &is, int howMany, int binary);
};


class GPD_PrimGroupList : public GPD_GroupList
{ };

class GPD_PointGroupList : public GPD_GroupList
{ };

#endif

