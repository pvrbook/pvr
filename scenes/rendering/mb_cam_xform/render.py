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

camera = PerspectiveCamera()

position    = VectorCurve()
position0   = V3f(0.0, 0.0, 10.0)
position1   = V3f(1.0, 0.0, 10.0)
position.addSample(0.0, position0)
position.addSample(1.0, position1)
angles0     = Euler(0.0, 0.0, 0.0)
angles1     = Euler(0.0, 0.0, radians(22.5))
orientation = QuatCurve()
orientation.addSample(0.0, angles0.toQuat())
orientation.addSample(1.0, angles1.toQuat())
resolution  = V2i(int(2048 * camResMult), int(1556 * camResMult))
fov         = degrees(calculateVerticalFOV(45.0, 24.0, resolution))

camera.setPosition(position)
camera.setOrientation(orientation)
camera.setVerticalFOV(fov)
camera.setClipPlanes(0.1, 1.0)
camera.setResolution(resolution)

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

Tp      = Matrix()
R       = Matrix()
S       = Matrix()
samples = MatrixCurve()

Tp.translate(V3f(-0.5))
S.scale(V3f(2.0))
samples.addSample(0.0, Tp * S * R)

volume  = ConstantVolume(samples)
volume.addAttribute("scattering", V3f(2, 4, 8) * 2.0)
renderer.addVolume(volume)

# Lights

#lights = pvr.lights.standardThreePoint(renderer, 1.0 / reduceRes)
#for light in lights:
#    renderer.addLight(light)
renderer.addLight(pvr.lights.standardKey(renderer, lightResMult, 
                                         occlType = OtfTransmittanceMapOccluder))

# Execute render
renderer.printSceneInfo()
renderer.execute()

# Save result
if not os.path.exists("out"):
    os.mkdir("out")
renderer.saveImage("out/image.exr")
renderer.saveImage("out/image.png")

# ------------------------------------------------------------------------------

