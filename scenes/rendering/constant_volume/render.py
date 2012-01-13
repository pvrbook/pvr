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

T = Matrix()
T.translate(V3f(-0.5))
R = Matrix()
R.setEulerAngles(V3f(5.0, 15.0, 5.0))
S = Matrix()
S.scale(V3f(2.0))
volume = ConstantVolume(T * R * S)
volume.addAttribute("scattering", V3f(10.0))
renderer.addVolume(volume)

# Lights

#lights = pvr.lights.standardThreePoint(renderer, 1.0 / reduceRes)
#for light in lights:
#    renderer.addLight(light)
renderer.addLight(pvr.lights.standardKey(renderer, lightResMult))

# Execute render
renderer.printSceneInfo()
renderer.execute()

# Save result
if not os.path.exists("out"):
    os.mkdir("out")
renderer.saveImage("out/image.exr")
renderer.saveImage("out/image.png")

# ------------------------------------------------------------------------------

