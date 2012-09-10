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

primParams = {
    # Base controls
    "radius"         : 1.0,
    "density"        : V3f(8, 16, 32),
    "antialiased"    : 1,
    # Noise controls
    "scale"          : 1.0,
    "octaves"        : 8.0,
    "octave_gain"    : 0.55, 
    "lacunarity"     : 1.92,
    "amplitude"      : 0.8,
    "gamma"          : 1.0,
    "displace_2d"    : 0,
    "pyroclastic"    : 0,
    "absolute_noise" : 0
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
prim = Prim.Rast.PyroclasticPoint()
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
renderer.saveImage(baseName.replace(".py", ".exr"))
renderer.saveImage(baseName.replace(".py", ".png"))

# ------------------------------------------------------------------------------

