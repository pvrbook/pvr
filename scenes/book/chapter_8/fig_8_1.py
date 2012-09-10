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

reduceRes  =    1
frustumRes =    V3i(2048 / reduceRes, 1556 / reduceRes, 200 / reduceRes)
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

primType = Prim.Inst.Sphere

primParams = {
    # Base controls
    "radius"                         : 1.4,
    "instance_radius"                : 0.002,
    "density"                        : V3f(1, 2, 4) * 10,
    "num_points"                     : 10000000, 
    "fill"                           : 1, 
    "seed"                           : 1, 
    # Noise controls
    "density_noise_scale"            : 0.75,
    "density_noise_octaves"          : 8.0,
    "density_noise_octave_gain"      : 0.75, 
    "density_noise_lacunarity"       : 1.92,
    "displacement_noise_scale"       : 1.0,
    "displacement_noise_octaves"     : 8.0,
    "displacement_noise_octave_gain" : 0.5, 
    "displacement_noise_lacunarity"  : 1.92,
    "displacement_noise_amplitude"   : 0.65,
    "density_noise"                  : 1, 
    "displacement_noise"             : 1  
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

# Create a single point in a Particles object
particles = Particles()
particles.add(1)

# Add the particles to a Geometry object
geo = Geometry()
geo.setParticles(particles)

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
renderer.saveImage(baseName.replace("py", "exr"))
renderer.saveImage(baseName.replace("py", "png"))

# ------------------------------------------------------------------------------

