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
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

raymarcherParams = {
    "use_volume_step_length" : 1,
    "volume_step_length_multiplier" : 1.0, 
    "do_early_termination" : 0,
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

renderer.setCamera(camera)

baseRenderer = renderer

for i in range(0, 4):

    p = pow(2, i)

    renderer = baseRenderer.clone()

    renderer.addLight(pvr.lights.standardRight(renderer, lightResMult))

    T = Matrix()
    T.translate(V3f(-0.5))
    S = Matrix()
    S.scale(V3f(2.5))

    volume = ConstantVolume(T * S)
    if i == 0:
        volume.addAttribute("scattering", V3f(1.0, 1.0, 1.0) * 0.5)
    elif i == 1:
        volume.addAttribute("scattering", V3f(0.5, 2.0, 2.0) * 0.5)
    elif i == 2:
        volume.addAttribute("scattering", V3f(2.0, 0.5, 2.0) * 0.5)
    else:
        volume.addAttribute("scattering", V3f(2.0, 2.0, 0.5) * 0.5)

    renderer.addVolume(volume)
    renderer.printSceneInfo()
    renderer.execute()
    renderer.saveImage("wavelength_colors_" + str(i) + ".png")
    renderer.saveImage("wavelength_colors_" + str(i) + ".exr")

# ------------------------------------------------------------------------------

