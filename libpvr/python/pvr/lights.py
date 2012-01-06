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

__defaultOccluder = OtfTransmittanceMapOccluder

# ------------------------------------------------------------------------------

def makeLight(renderer, parms, resMult, occlType):
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
    # Occluder
    occluder = None
    if occlType == TransmittanceMapOccluder:
        occluder = TransmittanceMapOccluder(renderer, cam)
    elif occlType == OtfTransmittanceMapOccluder:
        occluder = OtfTransmittanceMapOccluder(renderer, cam)
    elif occlType == VoxelOccluder:
        occluder = VoxelOccluder(renderer, parms["position"], 
                                 int(256 * resMult))
    elif occlType == OtfVoxelOccluder:
        occluder = OtfVoxelOccluder(renderer, parms["position"], 
                                    int(256 * resMult))
    else:
        occluder = NullOccluder()
    light.setOccluder(occluder)
    return light

# ------------------------------------------------------------------------------

def standardKey(renderer, resMult, occlType = __defaultOccluder):
    return makeLight(renderer, __stdKeyLight, resMult, occlType)

# ------------------------------------------------------------------------------

def standardFill(renderer, resMult, occlType = __defaultOccluder):
    return makeLight(renderer, __stdFillLight, resMult, occlType)

# ------------------------------------------------------------------------------

def standardRim(renderer, resMult, occlType = __defaultOccluder):
    return makeLight(renderer, __stdRimLight, resMult, occlType)

# ------------------------------------------------------------------------------

def standardThreePoint(renderer, resMult, occlType = __defaultOccluder):
    return [standardKey(renderer, resMult, occlType), 
            standardFill(renderer, resMult, occlType), 
            standardRim(renderer, resMult, occlType)]

# ------------------------------------------------------------------------------

def standardBehind(renderer, resMult, occlType = __defaultOccluder):
    return makeLight(renderer, __stdBehindLight, resMult, occlType)

# ------------------------------------------------------------------------------
