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
 * GPD_TPSurf.C
 *
 */


#include <iomanip>

#include "GPD_TPSurf.h"
#include "GPD_Profiles.h"
#include "GPD_Detail.h"

static const char	*gpd_profileHeader	= "Profiles:";
static const char	*gpd_profileNone	= "none";

GPD_TPSurf::GPD_TPSurf(GPD_Detail *d) : GPD_Hull(d)
{
    profiles = 0;

    uNumKnots = vNumKnots = 0;
    uKnots = vKnots = 0;
}

GPD_TPSurf::~GPD_TPSurf()
{
    delete profiles;

    delete [] uKnots;
    delete [] vKnots;
}

void
GPD_TPSurf::setUBasis(int order, int numknots, float *knots)
{
    uOrder = order;
    delete [] uKnots;
    if (!knots) uKnots = knots;
    else
    {
	uKnots = new float[numknots];
	memcpy(uKnots, knots, numknots * sizeof(float));
    }
    uNumKnots = numknots;
}

void
GPD_TPSurf::setVBasis(int order, int numknots, float *knots)
{
    vOrder = order;
    delete [] vKnots;
    if (!knots) vKnots = knots;
    else
    {
	vKnots = new float[numknots];
	memcpy(vKnots, knots, numknots * sizeof(float));
    }
    vNumKnots = numknots;
}

int
GPD_TPSurf::saveExtra(ostream &os, int binary) const
{
    int pflag;

    if (pflag = (profiles != 0))
    {
	if (binary)	UTwrite(os, &pflag);
	else		os << gpd_profileHeader	<< endl;

	if (!os) return -1;

	if (profiles->save(os, binary) < 0) return -1;
    }
    else
    {
	if (binary)
	{
	    UTwrite(os, &pflag);
	}
	else
	{
	    os << gpd_profileHeader << " " << gpd_profileNone << endl;
	}
    }

    return (!os) ? -1 : 0;
}

int
GPD_TPSurf::loadExtra(istream &is, int binary)
{
    // Clear the current profiles first:
    if (profiles)
    {
	delete profiles;
	profiles = 0;
    }

    if (getParent()->getVersion() < 1) return 0;

    int pflag;

    if (binary)
    {
	if (!UTread(is, &pflag)) return -1;
    }
    else
    {
	char tmpbuf[GPD_BUFSIZ];
	char usep[2]; usep[0] = usep[1] = 0;

	if (!(is >> tmpbuf)) return -1; // read the header

	// Read until start of next keyword (ie. can't put back whole word!)
	if (!(is >> setw(2) >> usep)) return -1;	

	pflag = (usep[0] != gpd_profileNone[0]);
	if (!is.putback(usep[0])) return -1; 

	if (!pflag)
	{
	    if (!(is >> tmpbuf)) return -1; // read the "none";
	    if (strcmp(tmpbuf,gpd_profileNone)) return -1;
	}
    }

    if (pflag)
    {
	profiles = new GPD_Profiles();
	if (profiles->load(is, binary) < 0) return -1;
    }

    return (!is) ? -1 : 0;
}
