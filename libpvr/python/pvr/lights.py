# ------------------------------------------------------------------------------
# lights.py
# ------------------------------------------------------------------------------

from math import radians, degrees

from pvr import *

# ------------------------------------------------------------------------------

__stdKeyLight = {
    "position" : V3f(-10, 10, 10),
    "rotation" : V3f(-35.0, -45.0, 0.0),
    "fov" : 20.0, 
    "intensity" : Color(0.5)
    }

__stdFillLight = {
    "position" : V3f(10, 5, 10),
    "rotation" : V3f(-19, 45.0, 0.0),
    "fov" : 20.0, 
    "intensity" : Color(0.06)
    }

__stdRimLight = {
    "position" : V3f(10, 10, -10),
    "rotation" : V3f(-35, 135, 0.0),
    "fov" : 20.0, 
    "intensity" : Color(0.125)
    }

__stdBehindLight = {
    "position" : V3f(0, 0, -20),
    "rotation" : V3f(0, 180, 0.0),
    "fov" : 20.0, 
    "intensity" : Color(1.0)
    }

# ------------------------------------------------------------------------------

def makeLight(renderer, parms, resMult):
    light = SpotLight()
    cam = PerspectiveCamera()
    # Position
    cam.setPosition(parms["position"])
    # Orientation
    angles = parms["rotation"]
    euler = Euler(radians(angles.x), radians(angles.y), radians(angles.z))
    cam.setOrientation(euler.toQuat())
    # FOV
    cam.setVerticalFOV(parms["fov"])
    # Resolution
    resolution = V2i(int(1024 * resMult), int(1024 * resMult))
    cam.setResolution(resolution)
    # Intensity
    light.setIntensity(parms["intensity"])
    # Camera
    light.setCamera(cam)
    tmap = TransmittanceMapOccluder(renderer, cam)
    light.setOccluder(tmap)
    return light
    
# ------------------------------------------------------------------------------

def standardKey(renderer, resMult):
    return makeLight(renderer, __stdKeyLight, resMult)

# ------------------------------------------------------------------------------

def standardFill(renderer, resMult):
    return makeLight(renderer, __stdFillLight, resMult)

# ------------------------------------------------------------------------------

def standardRim(renderer, resMult):
    return makeLight(renderer, __stdRimLight, resMult)

# ------------------------------------------------------------------------------

def standardThreePoint(renderer, resMult):
    return [standardKey(renderer, resMult), 
            standardFill(renderer, resMult), 
            standardRim(renderer, resMult)]

# ------------------------------------------------------------------------------

def standardBehind(renderer, resMult):
    return makeLight(renderer, __stdBehindLight, resMult)

# ------------------------------------------------------------------------------
