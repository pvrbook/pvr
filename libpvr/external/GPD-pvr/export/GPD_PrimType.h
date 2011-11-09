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
 * GPD_PrimType.h
 *
 *	This simply contains a list of token identifiers for primitive
 *	types.  Currently, there may be up to 32 types of primitives
 *	supported.  This may change (but not easily).  From the list
 *	below, it is apparent that this should be lots of primitives
 *	to work with.
 *
 *	Each primitive type must have a unique bit so that code can
 *	quickly search through the primitives it knows about.
 * 
 */

#ifndef __GPD_PrimType_h__
#define __GPD_PrimType_h__

//
//  Standard face types
#define GPDFACE			0x0000000f
#define GPDPRIMPOLY		0x00000001	// Polygon
#define GPDPRIMPOLYN		"Poly"
#define GPDPRIMNURBCURVE	0x00000002	// NURB Curve
#define GPDPRIMNURBCURVEN	"NURBCurve"
#define GPDPRIMBEZCURVE		0x00000004	// Rational Bezier Curve
#define GPDPRIMBEZCURVEN	"BezierCurve"

//
//  Standard hull types
#define GPDHULL			0x000000f0
#define GPDPRIMMESH		0x00000010	// Linear patch
#define GPDPRIMMESHN		"Mesh"
#define GPDPRIMNURBSURF		0x00000020	// NURB Surface
#define GPDPRIMNURBSURFN	"NURBMesh"
#define GPDPRIMBEZSURF		0x00000040	// Rational Bezier Surface
#define GPDPRIMBEZSURFN		"BezierMesh"

//
//  Standard paste types
#define GPDPRIMPASTESURF	0x00000f00	// TPsurf Paste Hierarchy
#define GPDPRIMPASTESURFN	"PasteSurf"

//
//  Standard quadric types
#define GPDPRIMQUADRIC		0x00007000	// All Quadrics
#define GPDPRIMCIRCLE		0x00001000	// Ellipse/Circle
#define GPDPRIMCIRCLEN		"Circle"
#define GPDPRIMSPHERE		0x00002000	// Ellipsoid/Sphere
#define GPDPRIMSPHEREN		"Sphere"
#define GPDPRIMTUBE		0x00004000	// Cylinder
#define GPDPRIMTUBEN		"Tube"

//
//  Standard particle types
#define GPDPRIMPART		0x00008000	// Particle
#define GPDPRIMPARTN		"Part"

//
// Standard meta primitive types
#define GPDPRIMMETA		0x00f00000	// All meta primitives
#define GPDPRIMMETABALL		0x00100000	// Meta-ball
#define GPDPRIMMETABALLN	"MetaBall"
#define GPDPRIMMETASQUAD	0x00200000	// Super quadric
#define GPDPRIMMETASQUADN	"MetaSQuad"
#define GPDPRIMMETALINE		0x00300000	// Meta-line
#define GPDPRIMMETALINEN	"MetaLine"
#define GPDPRIMMETABEZ		0x00400000	// Meta-bezier
#define GPDPRIMMETABEZN		"MetaBezier"
#define GPDPRIMMETATRI		0x00500000	// Meta-triangle
#define GPDPRIMMETATRIN		"MetaTriangle"

//
// Miscelleneous primitive types.
// These guys don't get & flags, you need to == directly.
#define GPDPRIMOTHER		0xff000000	// All triangle meshes
#define GPDPRIMTRIFAN		0x10000000	// Triangle fans.
#define GPDPRIMTRIFANN		"TriFan"
#define GPDPRIMTRISTRIP		0x20000000	// Generalized triangle strips
#define GPDPRIMTRISTRIPN	"TriStrip"
#define GPDPRIMTRIBEZIER	0x30000000	// Triangular beziers
#define GPDPRIMTRIBEZIERN	"TriBezier"

//
//  Standard trim sequance types
#define GPDTRIMSEQUENCE		0x10000000	// Trim override
#define GPDTRIMSEQUENCEN	"TrimSequence"


#define GPDPRIMALL		0xffffffff	// All primitive types

#define GPDPRIMRUN		GPDPRIMALL	// Run of prims for file format
#define GPDPRIMRUNN		"Run"

#define GPD_BASIS_N             "Basis"
#define GPD_BASIS_BEZSPLINE_N   "Bezier"
#define GPD_BASIS_NUBSPLINE_N   "NURB"


#define GDPRIMALL		GBPRIMALL	// All primitive types

#endif
