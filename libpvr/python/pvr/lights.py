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

__stdRaymarcherParams = {
    "use_volume_step_length" : 1,
    "volume_step_length_multiplier" : 1.0, 
    "do_early_termination" : 1,
    "early_termination_threshold" : 0.01
    }

# ------------------------------------------------------------------------------

def __baseLight(renderer, parms, resMult, rmParams):
    light = PointLight()
    # Position
    light.setPosition(parms["position"])
    # Orientation
    angles = parms["rotation"]
    euler = Euler(radians(angles.x), radians(angles.y), radians(angles.z))
    quat = euler.toQuat()
    # Intensity
    light.setIntensity(parms["intensity"])
    # Transmittance map
    resolution = V2i(int(1024 * resMult), int(1024 * resMult))
    raymarcherType = "Naive"
    samplerType = "ScatteringSampler"
    setupTransmittanceMap(renderer, light, resolution, quat, parms["fov"], 
                          raymarcherType, rmParams, samplerType)
    return light
    
# ------------------------------------------------------------------------------

def standardKey(renderer, resMult, rmParams = __stdRaymarcherParams):
    return __baseLight(renderer, __stdKeyLight, resMult, rmParams)

# ------------------------------------------------------------------------------

def standardFill(renderer, resMult, rmParams = __stdRaymarcherParams):
    return __baseLight(renderer, __stdFillLight, resMult, rmParams)

# ------------------------------------------------------------------------------

def standardRim(renderer, resMult, rmParams = __stdRaymarcherParams):
    return __baseLight(renderer, __stdRimLight, resMult, rmParams)

# ------------------------------------------------------------------------------

def standardThreePoint(renderer, resMult, rmParams = __stdRaymarcherParams):
    return [standardKey(renderer, resMult, rmParams), 
            standardFill(renderer, resMult, rmParams), 
            standardRim(renderer, resMult, rmParams)]

# ------------------------------------------------------------------------------
