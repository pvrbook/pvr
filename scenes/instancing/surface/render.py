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

primType = "SurfaceInstancer"

primParams = {
    # Base per-point
    "N"                              : V3f(0.0, 1.0, 0.0),
    "T"                              : V3f(1.0, 0.0, 0.0), 
    "v"                              : V3f(0.0),
    "density"                        : V3f(200.0),
    "thickness"                      : 0.1,
    # Base per-prim
    "instance_radius"                : 0.002, 
    "num_points"                     : 800000, 
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
    "displacement_noise_amplitude"   : 0.6,
    "density_noise"                  : 1,    # Turns on density noise
    "displacement_noise"             : 0     # Turns on displacement noise
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
geo   = Geometry.read("surface.bgeo")
# geo.meshes().debugPrint()
prim  = createInstantiationPrim(primType)
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

