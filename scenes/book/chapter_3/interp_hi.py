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
uniformRes =    400 / reduceRes
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

primParams = {
    # Base controls
    "radius"         : 1.0,
    "density"        : V3f(4, 8, 12) * 2,
    "antialiased"    : 1,
    # Noise controls
    "scale"          : 1.5,
    "octaves"        : 8.0,
    "octave_gain"    : 0.5, 
    "lacunarity"     : 1.92,
    "amplitude"      : 0.8,
    "gamma"          : 1.0,
    "displace_2d"    : 1,
    "pyroclastic"    : 1,
    "absolute_noise" : 1
}

raymarcherParams = {
    "use_volume_step_length" : 1,
    "volume_step_length_multiplier" : 1.0, 
    "do_early_termination" : 1,
    "early_termination_threshold" : 0.001
}

RenderGlobals.setupMotionBlur(24.0, 0.5)

# ------------------------------------------------------------------------------
# Camera
# ------------------------------------------------------------------------------

# Default camera
camera = pvr.cameras.standard(camResMult)

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
prim = createRasterizationPrim("PyroclasticPoint")
prim.setParams(primParams)

# Create the Modeler
modeler = Modeler()
modeler.setMapping(Mapping.MatrixMappingType)
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
modeler.setResolution(uniformRes)

# Execute modeler
modeler.execute()

# ------------------------------------------------------------------------------
# Renderer
# ------------------------------------------------------------------------------

# Standard settings
renderer = pvr.renderers.standard(raymarcherParams)

# Camera
renderer.setCamera(camera)

# Volumes
volume = VoxelVolume()
volume.setBuffer(modeler.buffer())
volume.setInterpolation(InterpType.NoInterp)
renderer.addVolume(volume)

# Lights
lights = pvr.lights.standardThreePoint(renderer, lightResMult,
                                       raymarcherParams)
for light in lights:
    renderer.addLight(light)

# No interp
renderer.execute()
renderer.saveImage("no_interp_hi.exr")
renderer.saveImage("no_interp_hi.png")

# ------------------------------------------------------------------------------
# Render and output result
# ------------------------------------------------------------------------------

# Linear interp
volume.setInterpolation(InterpType.LinearInterp)
renderer = pvr.renderers.standard(raymarcherParams)
renderer.setCamera(camera)
renderer.addVolume(volume)
lights = pvr.lights.standardThreePoint(renderer, lightResMult,
                                       raymarcherParams)
for light in lights:
    renderer.addLight(light)
renderer.execute()
renderer.saveImage("linear_interp_hi.exr")
renderer.saveImage("linear_interp_hi.png")

# Cubic interp
volume.setInterpolation(InterpType.CubicInterp)
renderer = pvr.renderers.standard(raymarcherParams)
renderer.setCamera(camera)
renderer.addVolume(volume)
lights = pvr.lights.standardThreePoint(renderer, lightResMult,
                                       raymarcherParams)
for light in lights:
    renderer.addLight(light)
renderer.execute()
renderer.saveImage("cubic_interp_hi.exr")
renderer.saveImage("cubic_interp_hi.png")

# Monotonic cubic interp
volume.setInterpolation(InterpType.MonotonicCubicInterp)
renderer = pvr.renderers.standard(raymarcherParams)
renderer.setCamera(camera)
renderer.addVolume(volume)
lights = pvr.lights.standardThreePoint(renderer, lightResMult,
                                       raymarcherParams)
for light in lights:
    renderer.addLight(light)
renderer.execute()
renderer.saveImage("monotonic_cubic_interp_hi.exr")
renderer.saveImage("monotonic_cubic_interp_hi.png")

# Gaussian interp
volume.setInterpolation(InterpType.GaussianInterp)
renderer = pvr.renderers.standard(raymarcherParams)
renderer.setCamera(camera)
renderer.addVolume(volume)
lights = pvr.lights.standardThreePoint(renderer, lightResMult,
                                       raymarcherParams)
for light in lights:
    renderer.addLight(light)
renderer.execute()
renderer.saveImage("gaussian_interp_hi.exr")
renderer.saveImage("gaussian_interp_hi.png")

# Mitchell interp
volume.setInterpolation(InterpType.MitchellInterp)
renderer = pvr.renderers.standard(raymarcherParams)
renderer.setCamera(camera)
renderer.addVolume(volume)
lights = pvr.lights.standardThreePoint(renderer, lightResMult,
                                       raymarcherParams)
for light in lights:
    renderer.addLight(light)
renderer.execute()
renderer.saveImage("mitchell_interp_hi.exr")
renderer.saveImage("mitchell_interp_hi.png")

# ------------------------------------------------------------------------------

