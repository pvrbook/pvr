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

__stdRightLight = {
    "position" : V3f(10, 0, 0),
    "rotation" : V3f(0.0, 90.0, 0.0),
    "fov" : 20.0, 
    "intensity" : Color(1.0)
    }

__defaultOccluder = OtfTransmittanceMapOccluder

__defaultLight = SpotLight

# ------------------------------------------------------------------------------

def makePointLight(renderer, parms, resMult, occlType):
    light = PointLight()
    cam = SphericalCamera()
    # Position
    light.setPosition(parms["position"])
    cam.setPosition(parms["position"])
    # Resolution
    resolution = V2i(int(2048 * resMult), int(1024 * resMult))
    cam.setResolution(resolution)
    # Intensity
    light.setIntensity(parms["intensity"])
    # Number of samples
    numSamples = 32
    if "num_samples" in parms.keys():
        numSamples = parms["num_samples"]
    # Occluder
    occluder = None
    if occlType == TransmittanceMapOccluder:
        occluder = TransmittanceMapOccluder(renderer, cam, numSamples)
    elif occlType == OtfTransmittanceMapOccluder:
        occluder = OtfTransmittanceMapOccluder(renderer, cam, numSamples)
    elif occlType == VoxelOccluder:
        occluder = VoxelOccluder(renderer, parms["position"], 
                                 int(256 * resMult))
    elif occlType == OtfVoxelOccluder:
        occluder = OtfVoxelOccluder(renderer, parms["position"], 
                                    int(256 * resMult))
    elif occlType == RaymarchOccluder:
        occluder = RaymarchOccluder(renderer)
    else:
        occluder = NullOccluder()
    light.setOccluder(occluder)
    return light

# ------------------------------------------------------------------------------

def makeSpotLight(renderer, parms, resMult, occlType):
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
    # Number of samples
    numSamples = 32
    if "num_samples" in parms.keys():
        numSamples = parms["num_samples"]
    # Occluder
    occluder = None
    if occlType == TransmittanceMapOccluder:
        occluder = TransmittanceMapOccluder(renderer, cam, numSamples)
    elif occlType == OtfTransmittanceMapOccluder:
        occluder = OtfTransmittanceMapOccluder(renderer, cam, numSamples)
    elif occlType == VoxelOccluder:
        occluder = VoxelOccluder(renderer, parms["position"], 
                                 int(256 * resMult))
    elif occlType == OtfVoxelOccluder:
        occluder = OtfVoxelOccluder(renderer, parms["position"], 
                                    int(256 * resMult))
    elif occlType == RaymarchOccluder:
        occluder = RaymarchOccluder(renderer)
    else:
        occluder = NullOccluder()
    light.setOccluder(occluder)
    return light

# ------------------------------------------------------------------------------

def makeLight(renderer, parms, resMult, occlType, lightType):
    if lightType == SpotLight:
        return makeSpotLight(renderer, parms, resMult, occlType)
    elif lightType == PointLight:
        return makePointLight(renderer, parms, resMult, occlType)
    else:
        print "Unrecognized light type in makeLight()"

# ------------------------------------------------------------------------------

def standardKey(renderer, resMult, occlType = __defaultOccluder,
                lightType = __defaultLight):
    return makeLight(renderer, __stdKeyLight, resMult, occlType, lightType)

# ------------------------------------------------------------------------------

def standardFill(renderer, resMult, occlType = __defaultOccluder,
                lightType = __defaultLight):
    return makeLight(renderer, __stdFillLight, resMult, occlType, lightType)

# ------------------------------------------------------------------------------

def standardRim(renderer, resMult, occlType = __defaultOccluder,
                lightType = __defaultLight):
    return makeLight(renderer, __stdRimLight, resMult, occlType, lightType)

# ------------------------------------------------------------------------------

def standardThreePoint(renderer, resMult, occlType = __defaultOccluder,
                lightType = __defaultLight):
    return [standardKey(renderer, resMult, occlType, lightType), 
            standardFill(renderer, resMult, occlType, lightType), 
            standardRim(renderer, resMult, occlType, lightType)]

# ------------------------------------------------------------------------------

def standardBehind(renderer, resMult, occlType = __defaultOccluder,
                lightType = __defaultLight):
    return makeLight(renderer, __stdBehindLight, resMult, occlType, lightType)

# ------------------------------------------------------------------------------

def standardRight(renderer, resMult, occlType = __defaultOccluder,
                lightType = __defaultLight):
    return makeLight(renderer, __stdRightLight, resMult, occlType, lightType)

# ------------------------------------------------------------------------------
