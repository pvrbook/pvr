#!/bin/make -f
#

LIB	= GPD
VERSION	= 1.0


#-----------------------------------------------------------------------
#   Please keep the list of sources sorted alphabetically
#

C++FILES =  \
	GPD_2DPrimNURBCurve.C \
	GPD_2DPrimPoly.C \
	GPD_2DPrimRBezCurve.C \
	GPD_Attribute.C \
	GPD_Quadric.C \
	GPD_Detail.C \
	GPD_Group.C \
	GPD_PrimTube.C \
	GPD_Vertex.C \
	GPD_LinkList.C \
	GPD_Primitive.C \
	GPD_Point.C \
	GPD_PrimCircle.C \
	GPD_Face.C \
	GPD_PrimPoly.C \
	GPD_Profile.C \
	GPD_Profiles.C \
	GPD_Hull.C \
	GPD_PrimMesh.C \
	GPD_PrimRBezCurve.C \
	GPD_PrimNURBCurve.C \
	GPD_PrimRBezSurf.C \
	GPD_PrimNURBSurf.C \
	GPD_PrimPart.C \
	GPD_PrimSphere.C \
	GPD_MetaPrim.C \
	GPD_PrimMetaBall.C \
	GPD_PrimMetaSQuad.C \
	GPD_PrimTriBezier.C \
	GPD_PrimTriFan.C \
	GPD_PrimTriStrip.C \
	GPD_StreamUtil.C \
	GPD_TPSurf.C \
	GPD_TriMesh.C \
	GPD_TrimSequence.C \
	GPD_Util.C


OBJECTS=$(C++FILES:.C=.o)

#ifdef LINUX
ENDIAN = -DSESI_LITTLE_ENDIAN
%.o : %.C
	g++ -g -Wno-deprecated -c -DSESI_LITTLE_ENDIAN $< -o $@
#else
#ifdef WINDOWS_NT
ENDIAN = -DSESI_LITTLE_ENDIAN
#else
ENDIAN =
#endif
#endif

C++FLAGS = $(ENDIAN) -I/usr/include/malloc



#-----------------------------------------------------------------------

default:	lib$(LIB).a

lib$(LIB).a:	$(OBJECTS)
	ar crv $@ $(OBJECTS)

clean:
	rm -f *.o core a.out

clobber:	clean
	rm -f *.a

production:	default
