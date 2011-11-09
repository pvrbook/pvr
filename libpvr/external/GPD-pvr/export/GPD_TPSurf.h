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
 * GPD_TPSurf.h
 *
 */


#ifndef __GPD_TPSurf_h__
#define __GPD_TPSurf_h__

#include "GPD_Hull.h"

class GPD_Profiles;

class GPD_TPSurf : public GPD_Hull {
public:
    GPD_TPSurf(GPD_Detail *d);
    virtual ~GPD_TPSurf();

    void		 setUBasis(int order, int numknots, float *knots);
    void		 setVBasis(int order, int numknots, float *knots);

    float		*getUBasis(int &order, int &numknots)
			 { 
			     order = uOrder; numknots = uNumKnots; 
			     return uKnots; 
			 }
    float		*getVBasis(int &order, int &numknots)
			 { 
			     order = vOrder; numknots = vNumKnots; 
			     return vKnots; 
			 }

protected:
    GPD_Profiles	*profiles;
    float		*uKnots, *vKnots;
    int 		 uNumKnots, vNumKnots;
    unsigned int	 uOrder, vOrder;

    virtual int		loadExtra(istream &is, int binary);
    virtual int 	saveExtra(ostream &is, int binary) const;

};


#endif

