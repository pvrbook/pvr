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
 * GPD_PrimTube.h
 *
 *   This is a cylinder.  There are two ways of storing a cylinder/tube:
 *	a) By setting two point references for the base and top
 *		of the cylinder.  In this method, the height of the
 *		cylinder is locked.  As well, the axes are fixed.
 *		This is the least preferred method since the X and Y
 *		axes are undefined (too many degrees of freedom).
 *	b) The preferred way is to specify a single point.  The tube
 *		will extend in the Z axis according to the height
 *		specified.  The tube may then be rotated or whatever
 *		and the X and Y axes will always be well defined.
 *
 */

#ifndef __GPD_PrimTube_H__
#define __GPD_PrimTube_H__

#include "GPD_Quadric.h"
#include "GPD_PrimType.h"
#include "GPD_Vertex.h"

class GPD_PrimTube : public GPD_Quadric {
public:
     GPD_PrimTube(GPD_Detail *d);
    virtual ~GPD_PrimTube();

    virtual unsigned	 getPrimitiveId() const; 

    unsigned		 endcaps(void) const { return caps; }
    void		 endcaps(int onOff) { caps = onOff; }


protected:
    // Save and load the privates of this class.
    virtual int	 	 savePrivate(ostream &os, int binary) const;
    virtual int	 	 loadPrivate(istream &is, int binary); 

private:
    char		caps;
    float		myTaper;

};
#endif
