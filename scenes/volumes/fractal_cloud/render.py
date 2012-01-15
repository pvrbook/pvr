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

reduceRes  =    8
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

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
# Renderer
# ------------------------------------------------------------------------------

renderer = pvr.renderers.standard(raymarcherParams)

# ------------------------------------------------------------------------------
# Scene setup
# ------------------------------------------------------------------------------

# Camera

renderer.setCamera(camera)

# Volumes

noise = PerlinNoise()
fractal = fBm(noise, 1.0, 6.0, 0.5, 2.04);

cloud = FractalCloud()
cloud.setFractal(fractal)
cloud.setDensity(10.0);
renderer.addVolume(cloud)

# Lights

lights = pvr.lights.standardThreePoint(renderer, lightResMult)
for light in lights:
    renderer.addLight(light)

# Execute render
renderer.printSceneInfo()
renderer.execute()

# Save result
if not os.path.exists("out"):
    os.mkdir("out")
renderer.saveImage("out/image.exr")
renderer.saveImage("out/image.png")

# ------------------------------------------------------------------------------

