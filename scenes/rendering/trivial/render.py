#! /usr/bin/env python

import os

from pvr import *

# Modeling classes
modeler = Modeler()
parts   = Particles()
geo     = Geometry()
prim    = Prim.Rast.PyroclasticPoint()
input   = ModelerInput()

# Create a modeling primitive with a single input point
parts.add(1)
geo.setParticles(parts)
input.setGeometry(geo)
input.setVolumePrimitive(prim)

# Add input to modeler and rasterize
modeler.addInput(input)
modeler.updateBounds()
modeler.setResolution(200)
modeler.execute()

# Rendering classes
renderer        = Renderer()
camera          = PerspectiveCamera()
raymarchSampler = PhysicalSampler()
raymarcher      = UniformRaymarcher()
occluderCamera  = SphericalCamera()
occluder        = OtfTransmittanceMapOccluder(renderer, occluderCamera, 8)
light           = PointLight()
volume          = VoxelVolume()

# Configure render camera
camera.setPosition(V3f(0.0, 0.25, 6.0))
camera.setResolution(V2i(320, 240))

# Set the raymarcher's raymarch sampler
raymarcher.setRaymarchSampler(raymarchSampler)

# Move the light into position
light.setPosition(V3f(10.0, 10.0, 10.0))
light.setIntensity(Color(1.5))

# Set up the occluder to cast shadows
occluderCamera.setPosition(light.position())
light.setOccluder(occluder)

# Configure the voxel volume 
volume.addAttribute("scattering", V3f(4.0, 6.0, 8.0))
volume.setBuffer(modeler.buffer())

# Connect the renderer with the raymarcher, camera, volume and light
renderer.setRaymarcher(raymarcher)
renderer.setCamera(camera)
renderer.addVolume(volume)
renderer.addLight(light)

# Print scene structure and start render
renderer.printSceneInfo()
renderer.execute()

# Save image to disk
if not os.path.exists("out"):
    os.mkdir("out")
renderer.saveImage("out/image.exr")
renderer.saveImage("out/image.png")

