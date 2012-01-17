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
renderer.setNumPixelSamples(4)

# ------------------------------------------------------------------------------
# Scene setup
# ------------------------------------------------------------------------------

# Camera

renderer.setCamera(camera)

# Volumes

Tp = Matrix()
Tp.translate(V3f(-0.5))
R = Matrix()
S = Matrix()
S.scale(V3f(2.0))
T = Matrix()
T.translate(V3f(0.5, 0.0, 0.0))
samples = MatrixCurve()
samples.addSample(0.0, Tp * S * R)
R.rotate(V3f(0.0, 0.0, radians(20.0)))
samples.addSample(1.0, Tp * S * R * T)
volume = ConstantVolume(samples)
volume.addAttribute("scattering", V3f(2, 4, 8) * 2.0)
renderer.addVolume(volume)

# Lights

#lights = pvr.lights.standardThreePoint(renderer, 1.0 / reduceRes)
#for light in lights:
#    renderer.addLight(light)
renderer.addLight(pvr.lights.standardKey(renderer, lightResMult, 
                                         #occlType = TransmittanceMapOccluder))
                                         occlType = OtfTransmittanceMapOccluder))
                                         #occlType = RaymarchOccluder))

# Execute render
renderer.printSceneInfo()
renderer.execute()

# Save result
if not os.path.exists("out"):
    os.mkdir("out")
renderer.saveImage("out/image.exr")
renderer.saveImage("out/image.png")

# ------------------------------------------------------------------------------

