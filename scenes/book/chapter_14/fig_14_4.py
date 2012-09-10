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

reduceRes  =    1
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

raymarcherParams = {
    "use_volume_step_length" : 1,
    "volume_step_length_multiplier" : 0.25, 
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

for i in range(0, 25):
    s = V3f(uniform(0.4, 0.8))
    r = Euler(random() * 180, random() * 180, random() * 180)
    rho = uniform(0.5, 1.0) * 1.6
    phi = uniform(-3.14, 3.14)
    theta = uniform(0.0, 3.14)
    t = V3f(rho * cos(phi) * sin(theta),
            rho * cos(theta),
            rho * sin(phi) * sin(theta))
    lsToWs = trsTransform(t, r, s)
    volume = ConstantVolume(lsToWs)
    volume.addAttribute("scattering", V3f(1, 2, 4) * 4)
    c.add(volume)

volume = ConstantVolume(trsTransform(V3f(0.0), Euler(), V3f(10.0)))
volume.addAttribute("scattering", V3f(0.1))
c.add(volume)

renderer.addVolume(c)

# Lights
lightParms = {
    "position" : V3f(0.0), 
    "intensity" : Color(2.0),
    "rotation" : V3f(0.0, 90.0, 0.0),
    "fov" : 170.0,
    "num_samples" : 128
}
light = pvr.lights.makePointLight(renderer, lightParms, lightResMult, 
                                  OtfTransmittanceMapOccluder)
light.setFalloffEnabled(True)
renderer.addLight(light)

# Execute render
renderer.printSceneInfo()
renderer.execute()

# Save result
renderer.saveImage("pointlight.png")
renderer.saveImage("pointlight.exr")

# ------------------------------------------------------------------------------

