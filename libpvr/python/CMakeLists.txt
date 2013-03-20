
##############################################################################
# Dependencies

FIND_PACKAGE( PythonLibs)

# find boost again, this time with boost_python
FIND_PACKAGE( Boost COMPONENTS thread python)

##############################################################################
# Includes

INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_DIRS})

##############################################################################
# Pvr python module

FILE( GLOB_RECURSE PVR_PY_SOURCES ./pvr/*.py)

SET( PVR_CPP_SOURCES    Common.cpp
                        Common.h
                        PyAttrTable.cpp
                        PyCamera.cpp
                        PyDeepImage.cpp
                        PyField3D.cpp
                        PyGeometry.cpp
                        PyGlobals.cpp
                        PyImage.cpp
                        PyLights.cpp
                        PyLog.cpp
                        PyMath.cpp
                        PyModeler.cpp
                        PyNoise.cpp
                        PyOccluders.cpp
                        PyParticles.cpp
                        PyPhaseFunction.cpp
                        PyPolygons.cpp
                        PyPrimitive.cpp
                        PyPvr.cpp
                        PyRaymarchers.cpp
                        PyRaymarchSamplers.cpp
                        PyRenderer.cpp
                        PyTypes.cpp
                        PyVolumes.cpp
                        )

ADD_LIBRARY( _pvr SHARED ${PVR_CPP_SOURCES})

SET_TARGET_PROPERTIES( _pvr PROPERTIES
                        PREFIX ""
                        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libpvr/pvr)

TARGET_LINK_LIBRARIES( _pvr pvr
                            ${FIELD3D_LIBRARIES}
                            ${OPENIMAGEIO_LIBRARIES}
                            ${Boost_LIBRARIES}
                            ${HDF5_LIBRARIES}
                            ${PYTHON_LIBRARIES}
                            ${IMATH_LIBRARIES}
                            )

ADD_CUSTOM_COMMAND( TARGET _pvr POST_BUILD
                    COMMAND cp ${PVR_PY_SOURCES} ${PROJECT_BINARY_DIR}/libpvr/pvr
                    )
