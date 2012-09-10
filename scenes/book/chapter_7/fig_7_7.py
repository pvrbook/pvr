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

reduceRes  =    1
frustumRes =    V3i(2048 / reduceRes, 1556 / reduceRes, 200 / reduceRes)
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

primType = Prim.Rast.Line

primParams = {
    # Per-point
    "density"     : V3f(4.0, 8.0, 16.0),
    # "radius"      : 0.1,
    # Per-prim
    "antialiased" : 1
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
radiusRef  = pointAttrs.addFloatAttr("radius", 1, [0.1])
ampRef     = pointAttrs.addFloatAttr("amplitude", 1, [0.1])

# Set per-point attributes
for i in range(0, numPoints):
    frac = i / float((numPoints - 1))
    x = -1.5 * 1.4 + frac * 3.0 * 1.4
    y = math.sin(math.pi * 2 * frac) * 0.5 * 1.4
    # Compute attribute values
    radius = 0.05 + (1 + math.cos(math.pi * 4 * frac)) * 0.1
    density = (1 + math.sin(0.5 + math.pi * 16 * frac))
    amp = 0.1 + frac * 0.9
    # Set attribute values
    pointAttrs.setVectorAttr(pRef, i, V3f(x, y, 0.0))
    pointAttrs.setFloatAttr (radiusRef, i, 0, radius)
    pointAttrs.setFloatAttr (ampRef, i, 0, amp)

# Add the particles to a Geometry object
geo = Geometry()
geo.setPolygons(lines)

# Create the volumetric primitive
prim = primType()
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
renderer.execute()

# Save result
baseName = os.path.basename(sys.argv[0])
renderer.saveImage(baseName.replace("py", "exr"))
renderer.saveImage(baseName.replace("py", "png"))

# ------------------------------------------------------------------------------

