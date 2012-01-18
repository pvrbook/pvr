#! /usr/bin/env python

# ------------------------------------------------------------------------------
# Imports
# ------------------------------------------------------------------------------

import os
import sys

from random import random, seed, uniform

from pvr import *

import pvr.cameras
import pvr.lights
import pvr.renderers

# ------------------------------------------------------------------------------
# Settings
# ------------------------------------------------------------------------------

seed(1)

reduceRes  =    8
camResMult =    1 / float(reduceRes)
lightResMult =  1

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

c = CompositeVolume()

for i in range(0, 10):
    s = V3f(uniform(0.1, 1.0))
    r = Euler(random() * 180, random() * 180, random() * 180)
    t = V3f(random() - 0.5, random() - 0.5, random() - 0.5) * 2
    volume = ConstantVolume(trsTransform(t, r, s))
    volume.addAttribute("scattering", V3f(uniform(0.5,1), uniform(0.5,1), uniform(0.5,1)) * 10)
    c.add(volume)

renderer.addVolume(c)

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

