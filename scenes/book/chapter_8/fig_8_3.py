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

primType = Prim.Inst.Surface

primParams = {
    # Base per-point
    "v"                              : V3f(0.0),
    "density"                        : V3f(1, 2, 4) * 100,
    "thickness"                      : 0.3,
    # Base per-prim
    "instance_radius"                : 0.001, 
    "num_points"                     : 80000000, 
    "fill"                           : 1, 
    # Noise per-prim
    "density_noise_scale"            : V3f(0.2, 0.2, 0.4),
    "density_noise_fade"             : V3f(0.5, 0.5, 0.5),
    "density_noise_octaves"          : 8.0,
    "density_noise_octave_gain"      : 0.75, 
    "density_noise_lacunarity"       : 1.92,
    "displacement_noise_scale"       : V3f(0.3, 0.3, 0.3),
    "displacement_noise_octaves"     : 8.0,
    "displacement_noise_octave_gain" : 0.5, 
    "displacement_noise_lacunarity"  : 1.92,
    "displacement_noise_amplitude"   : 1.5,
    "density_noise"                  : 1,    # Turns on density noise
    "displacement_noise"             : 1     # Turns on displacement noise
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
nCols = 10
nRows = 10
meshes = Meshes()
meshIdx = meshes.addMesh(nRows, nCols)
        
# Create attributes on line
pointAttrs = meshes.pointAttrs()
pRef       = pointAttrs.vectorAttrRef("P")
nRef       = pointAttrs.addVectorAttr("N", V3f(0.0))

def pos(x, y):
    return V3f(x - 0.5, y - 0.5, 0) * 3
def normal(x, y):
    d = 0.001
    c = pos(x, y)
    dx = pos(x + d, y)
    dy = pos(x, y + d)
    return (dx - c).cross(dy - c).normalized()
    
# Set per-point attributes
for j in range(0, nRows):
    for i in range(0, nCols):
        idx = meshes.pointForVertex(meshIdx, j, i)
        iFrac = i / float(nCols - 1)
        jFrac = j / float(nRows - 1)
        P = pos(iFrac, jFrac)
        N = normal(iFrac, jFrac)
        pointAttrs.setVectorAttr(pRef, idx, P)
        pointAttrs.setVectorAttr(nRef, idx, N)

# Update surface derivatives
meshes.computeDerivatives()

# Add the particles to a Geometry object
geo = Geometry()
geo.setMeshes(meshes)

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
renderer.printSceneInfo()
renderer.execute()

# Save result
baseName = os.path.basename(sys.argv[0])
renderer.saveImage(baseName.replace(".py", ".exr"))
renderer.saveImage(baseName.replace(".py", ".png"))

# ------------------------------------------------------------------------------

