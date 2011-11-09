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
 * GPD_Util.C
 *
 */



#include <iomanip.h>
#include <iostream.h>

#include "GPD_Util.h"
#include "GPD_Detail.h"

static const char	*gpd_useName	= "use";

int
GPDuseBreakpoints(istream &is, int binary)
{
    char	usec[2];
    int		useit;

    if (binary)
    {
	if (binary > 1) // recent version
	{
	    if (!is.get(usec[0])) return -1;
	    useit = (usec[0] != 0);
	}
	else 
	{
	    useit = 0;
	}
    }
    else
    {
	// Read until start of next keyword (ie. can't put back whole word!)
	if (!(is >> setw(2) >> usec)) return -1;	

	useit = (usec[0] == gpd_useName[0]);
	if (!is.putback(usec[0])) return -1; 

	/*
	// This works too, only it's not as pretty because of the while(). -CB
	int c;
	while (isspace(c=is.peek())) is.read((char*)&usec[0],sizeof(char));
	useit = (char(c) == gpd_useName[0]);
	*/

	if (useit)	// read entire keyword from stream
	{
	    char usebkpoints[GPD_BUFSIZ];
	    if (!(is >> setw(GPD_BUFSIZ) >> usebkpoints)) return -1;
	    if (strcmp(usebkpoints,gpd_useName))  return -1;
	}
    }
    
    return useit;
}
