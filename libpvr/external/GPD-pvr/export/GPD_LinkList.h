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
 * GPD_LinkList.h
 *
 */


#ifndef __GPD_LinkList__
#define __GPD_LinkList__

#include <iostream.h>


class GPD_LinkNode {
public:
    GPD_LinkNode(void) {predNode = succNode = 0; }
    virtual ~GPD_LinkNode(void);

    GPD_LinkNode *&prev() {return predNode;}
    GPD_LinkNode *&next() {return succNode;}
    const GPD_LinkNode *prev() const {return predNode;}
    const GPD_LinkNode *next() const {return succNode;}

protected:

private:
    GPD_LinkNode *predNode;
    GPD_LinkNode *succNode;
};


class GPD_LinkList {
public:
    GPD_LinkList(void)   {first=last=0; count=0;}
    virtual 	~GPD_LinkList(void);

    void 	append(GPD_LinkNode *node);

    GPD_LinkNode *head() const {return first; }

    void 	clear();

protected:
    GPD_LinkNode 	*first;
    GPD_LinkNode 	*last;
    int  		count;    //number of nodes in list

};

#endif
