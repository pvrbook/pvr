#! /usr/bin/env python

# ------------------------------------------------------------------------------
# Imports
# ------------------------------------------------------------------------------

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

primType = "PyroclasticPoint"

primParams = {
    # Base controls
    "v"              : V3f(0.0), 
    "radius"         : 1.0,
    "orientation"    : V3f(0, radians(5), radians(5)), 
    "density"        : V3f(5.0),
    "antialiased"    : 1,
    # Noise controls
    "scale"          : 1.5,
    "octaves"        : 8.0,
    "octave_gain"    : 0.5, 
    "lacunarity"     : 1.92,
    "amplitude"      : 1.0,
    "gamma"          : 1.0,
    "pyroclastic_2d" : 1,
    "pyroclastic"    : 1,
    "absolute_noise" : 1
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

camera = pvr.cameras.standard(1.0 / reduceRes)

# ------------------------------------------------------------------------------
# Modeler
# ------------------------------------------------------------------------------

modeler = Modeler()
modeler.setMapping(Mapping.FrustumMappingType)
modeler.setDataStructure(DataStructure.SparseBufferType)
modeler.setSparseBlockSize(SparseBlockSize.Size16)
modeler.setCamera(camera)

# Add input
input = ModelerInput()
parts = Particles()
parts.add(1)
geo = Geometry()
geo.setParticles(parts)
prim  = createRasterizationPrim(primType)
prim.setParams(primParams)
input.setGeometry(geo)
input.setVolumePrimitive(prim)
modeler.addInput(input)

# Prep for modeling
modeler.updateBounds()
modeler.setResolution(frustumRes.x, frustumRes.y, frustumRes.z)

# Execute modeler
modeler.execute()

# ------------------------------------------------------------------------------
# Renderer
# ------------------------------------------------------------------------------

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

phaseFunction = DoubleHenyeyGreenstein(0.8, 0.0, 0.5)
volume.setPhaseFunction(phaseFunction)

renderer.addVolume(volume)
renderer.printSceneInfo()

# Lights

lights = pvr.lights.standardThreePoint(renderer, 1.0 / reduceRes)
for light in lights:
    renderer.addLight(light)

# Execute render
renderer.execute()

# Save result
if not os.path.exists("out"):
    os.mkdir("out")
renderer.saveImage("out/image.exr")
renderer.saveImage("out/image.png")

# ------------------------------------------------------------------------------

