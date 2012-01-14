#! /usr/bin/env python

# ------------------------------------------------------------------------------
# Imports
# ------------------------------------------------------------------------------

import os
import sys

from random import random, seed, uniform
from math import sin, cos

from pvr import *

import pvr.cameras
import pvr.lights
import pvr.renderers

# ------------------------------------------------------------------------------
# Settings
# ------------------------------------------------------------------------------

seed(1)

reduceRes  =    2
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

camera = SphericalCamera()
camera.setResolution(V2i(int(1024 * camResMult), int(512 * camResMult)))

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

for i in range(0, 20):
    Tp = Matrix()
    Tp.translate(V3f(-0.5))
    S = Matrix()
    S.scale(V3f(uniform(0.2, 0.4)))
    R = Matrix()
    R.setEulerAngles(V3f(random() * 180, random() * 180, random() * 180))
    T = Matrix()
    rho = uniform(0.5, 1.0)
    phi = uniform(-3.14, 3.14)
    theta = uniform(0.0, 3.14)
    T.translate(V3f(rho * cos(phi) * sin(theta),
                    rho * sin(phi) * sin(theta),
                    rho * cos(theta)))
    volume = ConstantVolume(Tp * S * R * T)
    volume.addAttribute("scattering", V3f(10.0))
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

