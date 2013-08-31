# ------------------------------------------------------------------------------
# lights.py
# ------------------------------------------------------------------------------

from math import radians

import pvr, imath

# ------------------------------------------------------------------------------

__stdKeyLight = {
    "position" : imath.V3f(-10, 10, 10),
    "rotation" : imath.V3f(-35.0, -45.0, 0.0),
    "fov" : 20.0, 
    "intensity" : pvr.Color(0.5)
    }

__stdFillLight = {
    "position" : imath.V3f(10, 5, 10),
    "rotation" : imath.V3f(-19, 45.0, 0.0),
    "fov" : 20.0, 
    "intensity" : pvr.Color(0.06)
    }

__stdRimLight = {
    "position" : imath.V3f(10, 10, -10),
    "rotation" : imath.V3f(-35, 135, 0.0),
    "fov" : 20.0, 
    "intensity" : pvr.Color(0.125)
    }

__stdBehindLight = {
    "position" : imath.V3f(0, 0, -20),
    "rotation" : imath.V3f(0, 180, 0.0),
    "fov" : 20.0, 
    "intensity" : pvr.Color(1.0)
    }

__stdRightLight = {
    "position" : imath.V3f(10, 0, 0),
    "rotation" : imath.V3f(0.0, 90.0, 0.0),
    "fov" : 20.0, 
    "intensity" : pvr.Color(1.0)
    }

__defaultOccluder = pvr.OtfTransmittanceMapOccluder

__defaultLight = pvr.SpotLight

# ------------------------------------------------------------------------------

OCCLUDER_MAP = {
    pvr.TransmittanceMapOccluder : lambda renderer, cam, numSamples, _, __: 
        pvr.TransmittanceMapOccluder(renderer, cam, numSamples),
    pvr.OtfTransmittanceMapOccluder : lambda renderer, cam, numSamples, _, __:
        pvr.OtfTransmittanceMapOccluder(renderer, cam, numSamples),
    pvr.VoxelOccluder: lambda renderer, _, __, parms, resMult:
        pvr.VoxelOccluder(renderer, parms['position'], int(256* resMult)),
    pvr.RaymarchOccluder : lambda renderer, _, __, ___, ____:
        pvr.RaymarchOccluder(renderer),
}

def makePointLight(renderer, parms, resMult, occlType):
    light = pvr.PointLight()
    cam = pvr.SphericalCamera()
    # Position
    light.setPosition(parms["position"])
    cam.setPosition(parms["position"])
    # Resolution
    resolution = imath.V2i(int(2048 * resMult), int(1024 * resMult))
    cam.setResolution(resolution)
    # Intensity
    light.setIntensity(parms["intensity"])

    # Number of samples
    numSamples = parms.get("num_samples", 32)

    # Occluder
    occluder = OCCLUDER_MAP.get(occlType, lambda *args: pvr.NullOccluder())(
            renderer, cam, numSamples, parms, resMult)

    light.setOccluder(occluder)
    return light

# ------------------------------------------------------------------------------

def makeSpotLight(renderer, parms, resMult, occlType):
    light = pvr.SpotLight()
    cam = pvr.PerspectiveCamera()
    # Position
    cam.setPosition(parms["position"])
    # Orientation
    angles = parms["rotation"]
    pvr.euler = pvr.Euler(radians(angles.x), radians(angles.y), radians(angles.z))
    cam.setOrientation(pvr.euler.toQuat())
    # FOV
    cam.setVerticalFOV(parms["fov"])
    # Resolution
    resolution = imath.V2i(int(1024 * resMult), int(1024 * resMult))
    cam.setResolution(resolution)
    # Intensity
    light.setIntensity(parms["intensity"])
    # Camera
    light.setCamera(cam)
    # Number of samples
    numSamples = parms.get("num_samples", 32)

    # Occluder
    occluder = OCCLUDER_MAP.get(occlType, lambda *args: pvr.NullOccluder())(
            renderer, cam, numSamples, parms, resMult)

    light.setOccluder(occluder)
    return light

# ------------------------------------------------------------------------------
LIGHT_MAP = {
    pvr.SpotLight : makeSpotLight,
    pvr.PointLight: makePointLight,
}

def makeLight(renderer, parms, resMult, occlType, lightType):
    try:
        LIGHT_MAP[lightType](renderer, parms, resMult, occlType)
    except KeyError:
        print "Unrecognized light type (%s) in makeLight()" % lightType

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
