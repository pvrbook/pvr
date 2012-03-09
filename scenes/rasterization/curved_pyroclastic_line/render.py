#! /usr/bin/env python

# ------------------------------------------------------------------------------
# Imports
# ------------------------------------------------------------------------------

import math
import os
import sys

from pvr import *

import pvr.cameras
import pvr.lights
import pvr.renderers

# ------------------------------------------------------------------------------
# Settings
# ------------------------------------------------------------------------------

reduceRes  =    4
frustumRes =    V3i(2048 / reduceRes, 1556 / reduceRes, 200 / reduceRes)
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

primParams = {
    # Base per-point
    "density"        : V3f(4.0, 8.0, 16.0),
    "radius"         : 0.23,
    # Noise per-point
    "amplitude"      : 0.5,
    "gamma"          : 1.0, 
    # Base per-prim
    "antialiased"    : 1, 
    # Noise per-prim
    "scale"          : V3f(0.75, 0.75, 0.1),
    "octaves"        : 8.0, 
    "octave_gain"    : 0.5,
    "lacunarity"     : 1.92,
    "displace_2d"    : 1,
    "absolute_noise" : 1,
    "pyroclastic"    : 1
}

raymarcherParams = {
    "use_volume_step_length" : 1,
    "volume_step_length_multiplier" : 1.0, 
    "do_early_termination" : 1,
    "early_termination_threshold" : 0.01
}

RenderGlobals.setupMotionBlur(24.0, 0.5)

# ------------------------------------------------------------------------------
# Camera
# ------------------------------------------------------------------------------

# Default camera
camera = pvr.cameras.standard(1.0 / reduceRes)

# ------------------------------------------------------------------------------
# Modeler
# ------------------------------------------------------------------------------

# Create a single line in a Polygons object
numPoints = 100
lines = Polygons()
lineIdx = lines.addPolygon(numPoints)
lines.setIsClosed(lineIdx, False)
for i in range(0, numPoints):
    pIdx = lines.addPoint()
    lines.setVertex(lineIdx, i, pIdx)

# Create attributes on line
pointAttrs = lines.pointAttrs()
pRef       = pointAttrs.vectorAttrRef("P")
tRef       = pointAttrs.addVectorAttr("T", V3f(0.0))
nRef       = pointAttrs.addVectorAttr("N", V3f(0.0))
uRef       = pointAttrs.addFloatAttr("u", 1, [0.0])

def pos(x):
    return V3f(x, math.sin(math.pi * 2 * frac) * 0.5, 0)
def tangent(x):
    dx = 0.00001
    Pnext = pos(x + dx)
    P = pos(x)
    return (Pnext - P).normalized()
def normal(x):
    T = tangent(x)
    return V3f(0, 0, 1).cross(T)
    
# Set per-point attributes
for i in range(0, numPoints):
    frac = i / float((numPoints - 1))
    x = -1.5 + frac * 3.0
    P = pos(x)
    N = normal(x)
    T = tangent(x)
    # Compute attribute values
    # radius = 0.05 + (1 + math.cos(math.pi * 4 * frac)) * 0.1
    density = (1 + math.sin(0.5 + math.pi * 16 * frac))
    # Set attribute values
    pointAttrs.setVectorAttr(pRef, i, P)
    pointAttrs.setVectorAttr(tRef, i, T)
    pointAttrs.setVectorAttr(nRef, i, N)
    pointAttrs.setFloatAttr (uRef, i, 0, frac)

# Add the particles to a Geometry object
geo = Geometry()
geo.setPolygons(lines)

# Create the volumetric primitive
prim = Prim.Rast.PyroclasticLine()
prim.setParams(primParams)

# Create the Modeler
modeler = Modeler()
modeler.setMapping(Mapping.FrustumMappingType)
modeler.setDataStructure(DataStructure.SparseBufferType)
modeler.setSparseBlockSize(SparseBlockSize.Size16)
modeler.setCamera(camera)

# Add input to modeler
input = ModelerInput()
input.setGeometry(geo)
input.setVolumePrimitive(prim)
modeler.addInput(input)

# Prep for execution
modeler.updateBounds()
modeler.setResolution(frustumRes.x, frustumRes.y, frustumRes.z)

# Execute modeler
modeler.execute()

# ------------------------------------------------------------------------------
# Renderer
# ------------------------------------------------------------------------------

# Standard settings
renderer = pvr.renderers.standard(raymarcherParams)

# ------------------------------------------------------------------------------
# Scene setup
# ------------------------------------------------------------------------------

# Camera
renderer.setCamera(camera)

# Volumes
volume = VoxelVolume()
volume.setBuffer(modeler.buffer())
volume.addAttribute("scattering", V3f(1.0))
renderer.addVolume(volume)

# Lights
lights = pvr.lights.standardThreePoint(renderer, 1.0 / reduceRes)
for light in lights:
    renderer.addLight(light)

# ------------------------------------------------------------------------------
# Render and output result
# ------------------------------------------------------------------------------

# Execute render
renderer.printSceneInfo()
renderer.execute()

# Save result
if not os.path.exists("out"):
    os.mkdir("out")
renderer.saveImage("out/image.exr")
renderer.saveImage("out/image.png")

# ------------------------------------------------------------------------------

