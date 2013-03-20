
# Author : Esteban Tovagliari (ramenhdr@gmail.com)

##############################################################################
# Project

CMAKE_MINIMUM_REQUIRED( VERSION 2.8)
PROJECT( pvr)

##############################################################################
# CMake FindModule search path
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules)

##############################################################################
# Dependencies
FIND_PACKAGE( HDF5 REQUIRED)
FIND_PACKAGE( Boost COMPONENTS thread REQUIRED)
FIND_PACKAGE( Imath REQUIRED)
FIND_PACKAGE( FIELD3D REQUIRED)
FIND_PACKAGE( OpenImageIO REQUIRED)

##############################################################################
# Includes

# Duplicate the export directory to GPD-pvr
FILE( REMOVE_RECURSE ${PROJECT_BINARY_DIR}/GPD-pvr)
FILE( COPY libpvr/external/GPD-pvr/export/ DESTINATION ${PROJECT_BINARY_DIR}/GPD-pvr)

# Duplicate the export directory to libpvr
FILE( REMOVE_RECURSE ${PROJECT_BINARY_DIR}/pvr)
FILE( COPY libpvr/export/ DESTINATION ${PROJECT_BINARY_DIR}/pvr)

INCLUDE_DIRECTORIES(    ${PROJECT_BINARY_DIR}
                        ${PROJECT_BINARY_DIR}/GPD-pvr
                        ${IMATH_INCLUDE_DIRS}
                        ${HDF5_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIR}
                        ${FIELD3D_INCLUDE_DIRS}
                        )

##############################################################################
# Compile flags

# default to release builds
IF( NOT CMAKE_BUILD_TYPE)
  SET( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF()

IF( CMAKE_COMPILER_IS_GNUCXX)
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -fPIC")
ENDIF()

##############################################################################
# Definitions

ADD_DEFINITIONS( -DREQUIRE_IOSTREAM)

INCLUDE( TestBigEndian)
TEST_BIG_ENDIAN( WORDS_BIGENDIAN)

IF( NOT WORDS_BIGENDIAN)
    ADD_DEFINITIONS("-DSESI_LITTLE_ENDIAN")
ENDIF()

##############################################################################
# GPD-pvr

FILE( GLOB_RECURSE GPD_PVR_HEADERS libpvr/external/GPD-pvr/export/*.h)

ADD_LIBRARY( GPD-pvr STATIC ${GPD_PVR_HEADERS}
                            libpvr/external/GPD-pvr/src/GPD_2DPrimNURBCurve.C
                            libpvr/external/GPD-pvr/src/GPD_2DPrimPoly.C
                            libpvr/external/GPD-pvr/src/GPD_2DPrimRBezCurve.C
                            libpvr/external/GPD-pvr/src/GPD_Attribute.C
                            libpvr/external/GPD-pvr/src/GPD_Detail.C
                            libpvr/external/GPD-pvr/src/GPD_Face.C
                            libpvr/external/GPD-pvr/src/GPD_Group.C
                            libpvr/external/GPD-pvr/src/GPD_Hull.C
                            libpvr/external/GPD-pvr/src/GPD_LinkList.C
                            libpvr/external/GPD-pvr/src/GPD_MetaPrim.C
                            libpvr/external/GPD-pvr/src/GPD_Point.C
                            libpvr/external/GPD-pvr/src/GPD_PrimCircle.C
                            libpvr/external/GPD-pvr/src/GPD_Primitive.C
                            libpvr/external/GPD-pvr/src/GPD_PrimMesh.C
                            libpvr/external/GPD-pvr/src/GPD_PrimMetaBall.C
                            libpvr/external/GPD-pvr/src/GPD_PrimMetaSQuad.C
                            libpvr/external/GPD-pvr/src/GPD_PrimNURBCurve.C
                            libpvr/external/GPD-pvr/src/GPD_PrimNURBSurf.C
                            libpvr/external/GPD-pvr/src/GPD_PrimPart.C
                            libpvr/external/GPD-pvr/src/GPD_PrimPoly.C
                            libpvr/external/GPD-pvr/src/GPD_PrimRBezCurve.C
                            libpvr/external/GPD-pvr/src/GPD_PrimRBezSurf.C
                            libpvr/external/GPD-pvr/src/GPD_PrimSphere.C
                            libpvr/external/GPD-pvr/src/GPD_PrimTriBezier.C
                            libpvr/external/GPD-pvr/src/GPD_PrimTriFan.C
                            libpvr/external/GPD-pvr/src/GPD_PrimTriStrip.C
                            libpvr/external/GPD-pvr/src/GPD_PrimTube.C
                            libpvr/external/GPD-pvr/src/GPD_Profile.C
                            libpvr/external/GPD-pvr/src/GPD_Profiles.C
                            libpvr/external/GPD-pvr/src/GPD_Quadric.C
                            libpvr/external/GPD-pvr/src/GPD_StreamUtil.C
                            libpvr/external/GPD-pvr/src/GPD_TPSurf.C
                            libpvr/external/GPD-pvr/src/GPD_TriMesh.C
                            libpvr/external/GPD-pvr/src/GPD_TrimSequence.C
                            libpvr/external/GPD-pvr/src/GPD_Util.C
                            libpvr/external/GPD-pvr/src/GPD_Vertex.C
                            )

##############################################################################
# GPD-pvr

FILE( GLOB_RECURSE PVR_HEADERS libpvr/export/*.h)

ADD_LIBRARY( pvr SHARED ${PVR_HEADERS}
                        libpvr/src/AttrTable.cpp
                        libpvr/src/AttrUtil.cpp
                        libpvr/src/Camera.cpp
                        libpvr/src/DeepImage.cpp
                        libpvr/src/Geometry.cpp
                        libpvr/src/Globals.cpp
                        libpvr/src/Image.cpp
                        libpvr/src/Interrupt.cpp
                        libpvr/src/Lights/Light.cpp
                        libpvr/src/Lights/PointLight.cpp
                        libpvr/src/Lights/SpotLight.cpp
                        libpvr/src/Log.cpp
                        libpvr/src/Math.cpp
                        libpvr/src/Meshes.cpp
                        libpvr/src/Modeler.cpp
                        libpvr/src/ModelerInput.cpp
                        libpvr/src/Noise/Noise.cpp
                        libpvr/src/Occluders/OtfTransmittanceMapOccluder.cpp
                        libpvr/src/Occluders/OtfVoxelOccluder.cpp
                        libpvr/src/Occluders/RaymarchOccluder.cpp
                        libpvr/src/Occluders/TransmittanceMapOccluder.cpp
                        libpvr/src/Occluders/VoxelOccluder.cpp
                        libpvr/src/Particles.cpp
                        libpvr/src/PhaseFunction.cpp
                        libpvr/src/Polygons.cpp
                        libpvr/src/Primitives/RasterizationPrim.cpp
                        libpvr/src/Primitives/Instantiation/Line.cpp
                        libpvr/src/Primitives/Instantiation/Sphere.cpp
                        libpvr/src/Primitives/Instantiation/Surface.cpp
                        libpvr/src/Primitives/Rasterization/LineBase.cpp
                        libpvr/src/Primitives/Rasterization/Line.cpp
                        libpvr/src/Primitives/Rasterization/Point.cpp
                        libpvr/src/Primitives/Rasterization/PyroclasticLine.cpp
                        libpvr/src/Primitives/Rasterization/PyroclasticPoint.cpp
                        libpvr/src/Raymarchers/AdaptiveRaymarcher.cpp
                        libpvr/src/Raymarchers/Raymarcher.cpp
                        libpvr/src/Raymarchers/UniformRaymarcher.cpp
                        libpvr/src/RaymarchSamplers/DensitySampler.cpp
                        libpvr/src/RaymarchSamplers/PhysicalSampler.cpp
                        libpvr/src/Renderer.cpp
                        libpvr/src/RenderGlobals.cpp
                        libpvr/src/Strings.cpp
                        libpvr/src/VolumeAttr.cpp
                        libpvr/src/Volumes/CompositeVolume.cpp
                        libpvr/src/Volumes/ConstantVolume.cpp
                        libpvr/src/Volumes/FractalCloud.cpp
                        libpvr/src/Volumes/Volume.cpp
                        libpvr/src/Volumes/VoxelVolume.cpp
                        )

TARGET_LINK_LIBRARIES( pvr  GPD-pvr
                            ${FIELD3D_LIBRARIES}
                            ${OPENIMAGEIO_LIBRARIES}
                            ${Boost_LIBRARIES}
                            ${HDF5_LIBRARIES}
                            ${IMATH_LIBRARIES}
                            )

##############################################################################
# Python module

ADD_SUBDIRECTORY( libpvr/python)

