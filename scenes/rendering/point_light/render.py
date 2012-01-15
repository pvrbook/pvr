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

seed(2)

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

camera = pvr.cameras.standard(camResMult)

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

Tp = Matrix()
Tp.translate(V3f(-0.5))

for i in range(0, 25):
    S = Matrix()
    S.scale(V3f(uniform(0.4, 0.8)))
    R = Matrix()
    R.setEulerAngles(V3f(random() * 180, random() * 180, random() * 180))
    T = Matrix()
    rho = uniform(0.5, 1.0) * 1.6
    phi = uniform(-3.14, 3.14)
    theta = uniform(0.0, 3.14)
    T.translate(V3f(rho * cos(phi) * sin(theta),
                    rho * cos(theta),
                    rho * sin(phi) * sin(theta)))
    volume = ConstantVolume(Tp * S * R * T)
    volume.addAttribute("scattering", V3f(1, 2, 4) * 4)
    c.add(volume)

S = Matrix()
S.scale(V3f(10.0))

volume = ConstantVolume(Tp * S)
volume.addAttribute("scattering", V3f(0.1))
c.add(volume)

renderer.addVolume(c)

# Lights
lightParms = {
    "position" : V3f(0.0), 
    "intensity" : Color(2.0),
    "rotation" : V3f(0.0, 90.0, 0.0),
    "fov" : 170.0
}
light = pvr.lights.makePointLight(renderer, lightParms, lightResMult, 
                                  OtfTransmittanceMapOccluder)
light.setFalloffEnabled(True)
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

