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
frustumRes =    V3i(2048 / 2 / reduceRes, 1556 / 2 / reduceRes, 200 / reduceRes)
camResMult =    1 / float(reduceRes)
lightResMult =  1 / float(reduceRes)

primType = Prim.Rast.PyroclasticPoint

primParams = {
    # Base controls
    "v"              : V3f(0.0), 
    "radius"         : 1.0,
    "orientation"    : V3f(0, radians(5), radians(5)), 
    "density"        : V3f(5.0),
    "antialiased"    : 1,
    # Noise controls
    "scale"          : 1.5,
    "octaves"        : 8.0,
    "octave_gain"    : 0.5, 
    "lacunarity"     : 1.92,
    "amplitude"      : 1.0,
    "gamma"          : 1.0,
    "pyroclastic_2d" : 1,
    "pyroclastic"    : 1,
    "absolute_noise" : 1
}

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
# Modeler
# ------------------------------------------------------------------------------

modeler = Modeler()
modeler.setMapping(Mapping.FrustumMappingType)
modeler.setDataStructure(DataStructure.SparseBufferType)
modeler.setSparseBlockSize(SparseBlockSize.Size16)
modeler.setCamera(camera)

# Add input
input = ModelerInput()
parts = Particles()
parts.add(1)
geo = Geometry()
geo.setParticles(parts)
prim = primType()
prim.setParams(primParams)
input.setGeometry(geo)
input.setVolumePrimitive(prim)
modeler.addInput(input)

# Prep for modeling
modeler.updateBounds()
modeler.setResolution(frustumRes.x, frustumRes.y, frustumRes.z)

# Execute modeler
modeler.execute()

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

volume = VoxelVolume()
volume.setBuffer(modeler.buffer())
volume.addAttribute("scattering", V3f(1.0))

phaseFunction = Isotropic()
volume.setPhaseFunction(phaseFunction)

renderer.addVolume(volume)

# Lights

redParms = {
    "position" : V3f(10.0, 10.0, 10.0), 
    "intensity" : Color(1.0, 0.2, 0.2)
}
light = pvr.lights.makePointLight(renderer, redParms, lightResMult, 
                                  OtfTransmittanceMapOccluder)
renderer.addLight(light)
redParms = {
    "position" : V3f(-10.0, 0.0, 0.0), 
    "intensity" : Color(0.2, 1.0, 0.2)
}
light = pvr.lights.makePointLight(renderer, redParms, lightResMult, 
                                  OtfTransmittanceMapOccluder)
renderer.addLight(light)
redParms = {
    "position" : V3f(2.0, 0.0, -10.0), 
    "intensity" : Color(0.2, 0.2, 1.0)
}
light = pvr.lights.makePointLight(renderer, redParms, lightResMult, 
                                  OtfTransmittanceMapOccluder)
renderer.addLight(light)

# Execute render
renderer.printSceneInfo()
renderer.execute()
renderer.saveImage("phase_iso.png")

phaseFunction = HenyeyGreenstein(-0.75)
volume.setPhaseFunction(phaseFunction)
renderer.execute()
renderer.saveImage("phase_hg_1.png")

phaseFunction = HenyeyGreenstein(0.3)
volume.setPhaseFunction(phaseFunction)
renderer.execute()
renderer.saveImage("phase_hg_2.png")

phaseFunction = HenyeyGreenstein(0.9)
volume.setPhaseFunction(phaseFunction)
renderer.execute()
renderer.saveImage("phase_hg_3.png")

phase = DoubleHenyeyGreenstein(0.85, 0.0, 0.5)
volume.setPhaseFunction(phase)
renderer.execute()
renderer.saveImage("phase_dhg_1.png")

phase = DoubleHenyeyGreenstein(0.85, -0.85, 0.5)
volume.setPhaseFunction(phase)
renderer.execute()
renderer.saveImage("phase_dhg_2.png")

phase = DoubleHenyeyGreenstein(0.85, -0.85, 0.9)
volume.setPhaseFunction(phase)
renderer.execute()
renderer.saveImage("phase_dhg_3.png")

# ------------------------------------------------------------------------------

